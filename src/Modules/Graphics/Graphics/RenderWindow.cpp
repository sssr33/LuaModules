#include "pch.h"
#include "RenderWindow.h"

#include <future>
#include <libhelpers/ScopedValue.h>

const std::wstring RenderWindow::wndClassName = L"Graphics.RenderWindow";

RenderWindow::RenderWindow(std::wstring title)
    : hwnd(nullptr)
    , renderer(std::make_shared<GraphicsRenderer>())
    , renderThreadWork(true), renderThreadResize(true)
    , renderThreadRenderFrame(true), rendererChanged(true)
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

        {
            auto lk = thread::scoped_lock(this->renderThreadCs);

            while (this->renderThreadWork && !this->renderThreadResize && !this->renderThreadRenderFrame && !this->rendererChanged) {
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

            if (rendererLocal) {
                resize = this->renderThreadResize || resetOutput;
                render = this->renderThreadRenderFrame;
                renderSize = this->renderSize;
            }

            this->renderThreadResize = false;
            this->renderThreadRenderFrame = false;
        }

        if (resetOutput) {
            HWND hwnd = nullptr;

            {
                auto lk = thread::scoped_lock(this->cs);
                hwnd = this->hwnd;
            }

            output = std::make_unique<HwndOutput>(rendererLocal->GetDxDevice(), hwnd);

            RenderResourceSlots resSlots;

            resSlots.colorBrush = 2;
            resSlots.rect = 2;

            rendererLocal->SetResourceSlots(resSlots);

            RenderCmdList renderCmd;

            {
                SetRectCmd cmd;

                cmd.rect.left = 10.f;
                cmd.rect.top = 10.f;
                cmd.rect.right = 100.f;
                cmd.rect.bottom = 100.f;
                cmd.rectId = 0;

                renderCmd.Add(cmd);

                cmd.rect.left = 100.f;
                cmd.rect.top = 100.f;
                cmd.rect.right = 200.f;
                cmd.rect.bottom = 200.f;
                cmd.rectId = 1;

                renderCmd.Add(cmd);
            }

            {
                SetBrushColorCmd colorBrushRes;

                colorBrushRes.color.r = 1.f;
                colorBrushRes.color.g = 0.f;
                colorBrushRes.color.b = 0.f;
                colorBrushRes.color.a = 1.f;
                colorBrushRes.brushId = 0;

                renderCmd.Add(colorBrushRes);

                colorBrushRes.color.r = 1.f;
                colorBrushRes.color.g = 0.5f;
                colorBrushRes.color.b = 0.f;
                colorBrushRes.color.a = 1.f;
                colorBrushRes.brushId = 1;

                renderCmd.Add(colorBrushRes);
            }

            rendererLocal->Render(renderCmd);
        }

        if (resize) {
            output->SetLogicalSize(renderSize);
            output->Resize();
        }

        if (render)
        {
            auto rectId1 = 0;
            auto rectId2 = 1;

            auto brushId1 = 0;
            auto brushId2 = 1;

            RenderCmdList renderCmd;
            {
                renderCmd.Clear();

                {
                    ClearScreenCmd cmd;

                    cmd.color = { 0.1f, 0.1f, 0.1f, 1.f };
                    renderCmd.Add(std::move(cmd));
                }

                for (int i = 0; i < 10; i++) {
                    {
                        SetRectCmd cmd;

                        int x1 = rand() % 1000;
                        int x2 = rand() % 1000;
                        int y1 = rand() % 1000;
                        int y2 = rand() % 1000;

                        cmd.rect.left = (float)(std::min)(x1, x2);
                        cmd.rect.top = (float)(std::min)(y1, y2);
                        cmd.rect.right = (float)(std::max)(x1, x2);
                        cmd.rect.bottom = (float)(std::max)(y1, y2);
                        cmd.rectId = rectId1;

                        renderCmd.Add(cmd);
                    }

                    {
                        SetBrushColorCmd cmd;

                        cmd.color.r = 1.f;
                        cmd.color.g = 0.f;
                        cmd.color.b = (float)i / 9.f;
                        cmd.color.a = 0.1f + 0.9f * ((float)i / 9.f);
                        cmd.brushId = brushId1;

                        renderCmd.Add(cmd);
                    }

                    {
                        RenderRectCmd cmd;

                        cmd.fill = true;
                        cmd.brushId = brushId1;
                        cmd.rectId = rectId1;
                        renderCmd.Add(cmd);
                    }
                }

                {
                    RenderRectCmd cmd;

                    cmd.fill = true;
                    cmd.brushId = brushId1;
                    cmd.rectId = rectId1;
                    renderCmd.Add(cmd);
                }

                {
                    RenderRectCmd cmd;

                    cmd.fill = true;
                    cmd.brushId = brushId2;
                    cmd.rectId = rectId2;
                    renderCmd.Add(cmd);
                }
            }

            {
                auto dxLk = rendererLocal->GetDxDevice()->LockCtxScoped();
                output->BeginRender();
                rendererLocal->RenderNoCtxLock(renderCmd);
                output->EndRender();
            }
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
            this->renderThreadRenderFrame = true;
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