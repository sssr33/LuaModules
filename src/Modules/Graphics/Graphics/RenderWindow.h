#pragma once
#include "GraphicsRenderer.h"
#include "RenderWindowFrame.h"

#include <memory>
#include <atomic>
#include <thread>
#include <string>
#include <queue>
#include <Windows.h>
#include <libhelpers/Thread/critical_section.h>
#include <libhelpers/Thread/condition_variable.h>
#include <libhelpers/Macros.h>
#include <libhelpers/Dx/Renderer/HwndOutput.h>

class RenderWindow {
    static const std::wstring wndClassName;
public:
    NO_COPY_MOVE(RenderWindow);

    RenderWindow(std::wstring title);
    ~RenderWindow();

    std::shared_ptr<GraphicsRenderer> GetRenderer() const;
    void SetRenderer(std::shared_ptr<GraphicsRenderer> v);

    void Render(RenderWindowFrame frame);

private:
    std::thread th;

    thread::critical_section cs;
    HWND hwnd;

    mutable thread::critical_section renderThreadCs;
    thread::condition_variable renderThreadCv;
    thread::condition_variable renderDoneCv;
    std::shared_ptr<GraphicsRenderer> renderer;
    bool renderThreadWork;
    bool renderThreadResize;
    bool rendererChanged;
    std::queue<RenderWindowFrame> renderQueue;
    RenderWindowFrame lastFrame;
    DirectX::XMFLOAT2 renderSize;
    std::thread renderThread;

    HINSTANCE GetHInstance() const;
    std::wstring GetWndClassName() const;

    void InitWindow(const std::wstring &title);
    void MsgLoop();
    void UninitWindow();

    void RenderThreadMain();

    LRESULT ThisWndProc(UINT msg, WPARAM wparam, LPARAM lparam, bool &handled);
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};