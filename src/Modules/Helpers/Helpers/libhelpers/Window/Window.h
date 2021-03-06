#pragma once
#include "..\config.h"
#if HAVE_WINRT != 1
#include "WindowBaseData.h"

#include <DirectXMath.h>

struct ProcessMsgResult {
    LRESULT lresult;
    bool handled;

    ProcessMsgResult();
    ProcessMsgResult(bool handled);
};

class Window {
public:
    Window(WindowBaseData &baseData);
    virtual ~Window();

    DirectX::XMUINT2 GetSize() const;

    void ProcessMessages();
    virtual ProcessMsgResult ProcessMsg(uint32_t msg, WPARAM wparam, LPARAM lparam) = 0;

protected:
    HWND GetHwnd() const;
    const std::wstring &GetWndClassName() const;
    void PostShutdown();

private:
    WindowBaseData &baseData;
};
#endif // !HAVE_WINRT
