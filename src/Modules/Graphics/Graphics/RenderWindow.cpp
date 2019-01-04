#include "pch.h"
#include "RenderWindow.h"

#include <future>
#include <libhelpers/ScopedValue.h>

const std::wstring RenderWindow::wndClassName = L"Graphics.RenderWindow";

RenderWindow::RenderWindow(std::wstring title)
    : hwnd(nullptr)
    , renderer(std::make_shared<GraphicsRenderer>())
    , renderThreadWork(true), renderThreadResize(true)
    , rendererChanged(true)
    , renderSize(1.f, 1.f)
{
    std::promise<void> prom;

    this->th = std::thread([&, this]() {
        try {
            this->InitWindow(title);
        }
        catch (const std::exception &e) {
            prom.set_exception(std::make_exception_ptr(e));
            return;
        }

        prom.set_value();

        this->MsgLoop();
        this->UninitWindow();
    });

    auto fut = prom.get_future();
    fut.get();
}

RenderWindow::~RenderWindow() {
    {
        auto lk = thread::scoped_lock(this->renderThreadCs);
        this->renderThreadWork = false;
    }

    this->renderThreadCv.notify();

    {
        auto lk = thread::scoped_lock(this->cs);
        PostMessage(this->hwnd, WM_CLOSE, 0, 0);
    }

    if (this->renderThread.joinable()) {
        this->renderThread.join();
    }

    if (this->th.joinable()) {
        this->th.join();
    }
}

std::shared_ptr<GraphicsRenderer> RenderWindow::GetRenderer() const {
    auto lk = thread::scoped_lock(this->renderThreadCs);
    return this->renderer;
}

void RenderWindow::SetRenderer(std::shared_ptr<GraphicsRenderer> v) {
    {
        auto lk = thread::scoped_lock(this->renderThreadCs);
        this->renderer = std::move(v);
        this->rendererChanged = true;
    }

    this->renderThreadCv.notify();
}

void RenderWindow::Render(RenderWindowFrame frame) {
    {
        auto lk = thread::scoped_lock(this->renderThreadCs);

        while (this->renderQueue.size() != 0) {
            this->renderDoneCv.wait(this->renderThreadCs);
        }

        this->renderQueue.push(std::move(frame));
    }

    this->renderThreadCv.notify();
}

HINSTANCE RenderWindow::GetHInstance() const {
    HMODULE hmod = nullptr;

    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, nullptr, &hmod);

    return hmod;
}

std::wstring RenderWindow::GetWndClassName() const {
    auto name = RenderWindow::wndClassName + std::to_wstring((size_t)this);
    return name;
}

void RenderWindow::InitWindow(const std::wstring &title) {
    WNDCLASSEX regClass;

    auto hInstance = this->GetHInstance();
    auto wndClassName = this->GetWndClassName();

    regClass.cbSize = sizeof WNDCLASSEX;
    regClass.style = CS_DBLCLKS;
    regClass.lpfnWndProc = RenderWindow::WndProc;
    regClass.cbClsExtra = 0;
    regClass.cbWndExtra = 0;
    regClass.hInstance = hInstance;
    regClass.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    regClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    regClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    regClass.lpszMenuName = NULL;
    regClass.lpszClassName = wndClassName.c_str();
    regClass.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&regClass)) {
        throw std::exception("Can't create RenderWindow. RegisterClassEx has failed.");
    }

    auto hwndTmp = CreateWindowEx(
        WS_EX_DLGMODALFRAME,
        wndClassName.c_str(),
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        nullptr, nullptr,
        hInstance,
        this);

    ShowWindow(hwndTmp, SW_SHOW);
}

void RenderWindow::MsgLoop() {
    MSG msg;
    BOOL ret;

    while ((ret = GetMessageW(&msg, nullptr, 0, 0)) != 0) {
        if (ret == -1) {
            // handle the error and possibly exit
            break;
        }
        else {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }
}

void RenderWindow::UninitWindow() {
    auto hInstance = this->GetHInstance();
    auto wndClassName = this->GetWndClassName();

    UnregisterClass(wndClassName.c_str(), hInstance);
}

void RenderWindow::RenderThreadMain() {
    std::unique_ptr<HwndOutput> output;
    std::shared_ptr<GraphicsRenderer> rendererLocal;

    while (true) {
        bool resize = false;
        bool render = false;
        bool resetOutput = false;
        DirectX::XMFLOAT2 renderSize(1.f, 1.f);
        RenderWindowFrame curFrame;
        bool trySetResources = true;

        {
            auto lk = thread::scoped_lock(this->renderThreadCs);

            while (this->renderThreadWork && !this->renderThreadResize && this->renderQueue.empty() && !this->rendererChanged) {
                this->renderThreadCv.wait(this->renderThreadCs);
            }

            if (!this->renderThreadWork) {
                break;
            }

            if (this->rendererChanged) {
                this->rendererChanged = false;
                output = nullptr;
                rendererLocal = this->renderer;

                if (rendererLocal) {
                    resetOutput = true;
                }
            }

            bool hasCurFrame = false;

            if (!this->renderQueue.empty()) {
                curFrame = std::move(this->renderQueue.front());
                hasCurFrame = true;
            }

            if (rendererLocal) {
                resize = this->renderThreadResize || resetOutput;
                render = hasCurFrame || resize;
                renderSize = this->renderSize;

                if (render && !hasCurFrame) {
                    curFrame = std::move(this->lastFrame);
                    this->renderQueue.push(RenderWindowFrame());
                }
            }

            this->renderThreadResize = false;
        }

        if (resetOutput) {
            HWND hwnd = nullptr;

            {
                auto lk = thread::scoped_lock(this->cs);
                hwnd = this->hwnd;
            }

            output = std::make_unique<HwndOutput>(rendererLocal->GetDxDevice(), hwnd);

            trySetResources = false;
            rendererLocal->SetResourceSlots(curFrame.GetSlots());
        }

        if (resize) {
            output->SetLogicalSize(renderSize);
            output->Resize();
        }

        if (render) {
            {
                auto dxLk = rendererLocal->GetDxDevice()->LockCtxScoped();
                output->BeginRender();

                if (trySetResources && !curFrame.GetSlots().Empty()) {
                    rendererLocal->SetResourceSlotsNoCtxLock(curFrame.GetSlots());
                }

                rendererLocal->RenderNoCtxLock(curFrame.cmdList);
                output->EndRender();
            }

            {
                auto lk = thread::scoped_lock(this->renderThreadCs);

                assert(!this->renderQueue.empty());
                this->renderQueue.pop();

                this->lastFrame.cmdList = std::move(curFrame.cmdList);

                if (!this->lastFrame.GetSlots().Empty()) {
                    this->lastFrame.SetSlots(this->lastFrame.DetachSlots());
                }
            }

            this->renderDoneCv.notify_all();
        }
    }
}

LRESULT RenderWindow::ThisWndProc(UINT msg, WPARAM wparam, LPARAM lparam, bool &handled) {
    LRESULT res = 0;
    handled = true;

    switch (msg) {
    case WM_CREATE: {
        this->renderThread = std::thread([=]() {
            this->RenderThreadMain();
        });
        break;
    }
    case WM_SIZE: {
        auto width = LOWORD(lparam);
        auto height = HIWORD(lparam);

        {
            auto lk = thread::scoped_lock(this->renderThreadCs);
            this->renderThreadResize = true;
            this->renderSize.x = (float)width;
            this->renderSize.y = (float)height;
        }

        this->renderThreadCv.notify();

        break;
    }
    default:
        handled = false;
        break;
    }

    return res;
}

LRESULT CALLBACK RenderWindow::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
    LRESULT res = 0;
    bool handled = false;
    auto _this = (RenderWindow*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

    if (_this) {
        res = _this->ThisWndProc(msg, wparam, lparam, handled);
    }

    switch (msg) {
    case WM_CREATE: {
        auto createStruct = (CREATESTRUCT*)lparam;
        auto _this = (RenderWindow*)createStruct->lpCreateParams;

        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)_this);

        {
            auto lk = thread::scoped_lock(_this->cs);
            _this->hwnd = hwnd;
        }

        _this->ThisWndProc(msg, wparam, lparam, handled);

        break;
    }
    case WM_CLOSE: {
        _this->ThisWndProc(msg, wparam, lparam, handled);
        DestroyWindow(hwnd);
        break;
    }
    case WM_DESTROY: {
        _this->ThisWndProc(msg, wparam, lparam, handled);
        PostQuitMessage(0);
        break;
    }
    default:
        if (!handled) {
            res = DefWindowProc(hwnd, msg, wparam, lparam);
        }
        break;
    }

    return res;
}