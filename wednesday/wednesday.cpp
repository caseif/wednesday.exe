#include "wednesday.h"

#include <string>
#include <windows.h>

const wchar_t kClassName[] = L"Wednesday";

const double kUnitTime = 60000 / 145.0;

const double kStep = 5.5 / 72.0;

const double kXOff[] = { 0, 1 * kStep, 2 * kStep, 3 * kStep, 4 * kStep, 5 * kStep,
                         6 * kStep, 7 * kStep, 8 * kStep, 9 * kStep, 10 * kStep, 11 * kStep,
                         11 * kStep, 10 * kStep, 9 * kStep, 8 * kStep, 7 * kStep, 6 * kStep,
                         5 * kStep, 4 * kStep, 3 * kStep, 2 * kStep, 1 * kStep, 0,
                         0, 0, 2 * kStep, 2 * kStep, 4 * kStep, 4 * kStep,
                         6 * kStep, 6 * kStep, 8 * kStep, 8 * kStep, 10 * kStep, 10 * kStep };

const double kYOff[] = { 0, 1 * kStep, 2 * kStep, 3 * kStep, 4 * kStep, 5 * kStep,
                         6 * kStep, 7 * kStep, 8 * kStep, 9 * kStep, 10 * kStep, 11 * kStep,
                         0, 1 * kStep, 2 * kStep, 3 * kStep, 4 * kStep, 5 * kStep,
                         6 * kStep, 7 * kStep, 8 * kStep, 9 * kStep, 10 * kStep, 11 * kStep,
                         2 * kStep, 6 * kStep, 4 * kStep, 8 * kStep, 2 * kStep, 6 * kStep,
                         4 * kStep, 8 * kStep, 2 * kStep, 6 * kStep, 4 * kStep, 8 * kStep };

const double timings[] = { 2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0,
                           2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0,
                           2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.0 };

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, int nShowCmd) {
    // Register the window class.
    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = kClassName;

    RegisterClass(&wc);

    int resX = GetSystemMetrics(SM_CXSCREEN);
    int resY = GetSystemMetrics(SM_CYSCREEN);

    for (int i = 0; i < sizeof(timings) / sizeof(double); i++) {
        MakeFrog(hInst, kClassName, kXOff[i] * resX, kYOff[i] * resY, nShowCmd);
        Sleep(timings[i] * kUnitTime);
    }

    return 0;
}

inline LPCWSTR IntToStr(int i) {
    return std::to_wstring(i).c_str();
}

void MakeFrog(HINSTANCE hInst, LPCTSTR className, int x, int y, int nCmdShow) {
    int resX = GetSystemMetrics(SM_CXSCREEN);
    int resY = GetSystemMetrics(SM_CYSCREEN);

    int size = resX / 6.0;

    HWND hwnd = CreateWindowEx(NULL, kClassName, L"wednesday", WS_OVERLAPPEDWINDOW,
        x, y, size, size,
        NULL, NULL, hInst, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, std::to_wstring(GetLastError()).c_str(), L"Failed to Launch", MB_ICONERROR);
        return;
    }

    ShowWindow(hwnd, nCmdShow);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

