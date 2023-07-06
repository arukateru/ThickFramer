// ==WindhawkMod==
// @id              thickframer
// @name            thickframer
// @description     The best mod ever that does great things
// @version         0.1
// @author          Alcatel
// @github          https://github.com/arukateru
// @include         *
// @exclude         notepad++.exe
// ==/WindhawkMod==

#include <Windows.h>

HHOOK g_hHook = NULL;

BOOL HasCaptionButtons(HWND hWnd)
{
    LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);

    return ((style & WS_CAPTION) == WS_CAPTION) && !(style & (WS_THICKFRAME));
}

LRESULT CALLBACK CbtHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HCBT_CREATEWND)
    {
        HWND hWnd = (HWND)wParam;
        DWORD dwProcessId;
        GetWindowThreadProcessId(hWnd, &dwProcessId);

        if (dwProcessId == GetCurrentProcessId() && HasCaptionButtons(hWnd))
        {

            LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
            style |= WS_THICKFRAME;
            SetWindowLongPtr(hWnd, GWL_STYLE, style);

            SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
        }
    }

    return CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        g_hHook = SetWindowsHookEx(WH_CBT, CbtHookProc, hinstDLL, 0);
    }
    else if (fdwReason == DLL_PROCESS_DETACH)
    {
        if (g_hHook != NULL)
        {
            UnhookWindowsHookEx(g_hHook);
            g_hHook = NULL;
        }
    }

    return TRUE;
}
