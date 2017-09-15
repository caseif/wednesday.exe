#define _SCL_SECURE_NO_WARNINGS

#pragma comment(lib, "MSIMG32.lib")
#pragma comment(lib, "Winmm.lib")

#include "wednesday.h"
#include "resources.h"
#include "resource_snd.h"

#include <algorithm>
#include <map>
#include <string>

#include <time.h>
#include <windows.h>

const wchar_t kClassName[] = L"Wednesday";

const double kUnitTime = 60000 / 152.0;

const double kStep = 5.5 / 72.0;

const COLORREF kRed = RGB(255, 0, 0);
const COLORREF kBlue = RGB(0, 0, 255);

const double kXOff[] = { 0, 1 * kStep, 2 * kStep, 3 * kStep, 4 * kStep, 5 * kStep,
                         6 * kStep, 7 * kStep, 8 * kStep, 9 * kStep, 10 * kStep,
                         10 * kStep, 9 * kStep, 8 * kStep, 7 * kStep, 6 * kStep,
                         5 * kStep, 4 * kStep, 3 * kStep, 2 * kStep, 1 * kStep, 0,
                         0, 0, 3 * kStep, 3 * kStep, 6 * kStep, 6 * kStep,
                         8 * kStep, 8 * kStep, 10 * kStep,
                         -1, -1, -1, -1, -1, -1, -1, -1, -1 };

const double kYOff[] = { 0, 1 * kStep, 2 * kStep, 3 * kStep, 4 * kStep, 5 * kStep,
                         6 * kStep, 7 * kStep, 8 * kStep, 9 * kStep, 10 * kStep,
                         0, 1 * kStep, 2 * kStep, 3 * kStep, 4 * kStep, 5 * kStep,
                         6 * kStep, 7 * kStep, 8 * kStep, 9 * kStep, 10 * kStep,
                         2 * kStep, 6 * kStep, 4 * kStep, 8 * kStep, 2 * kStep, 6 * kStep,
                         4 * kStep, 8 * kStep, 5 * kStep,
                         -1, -1, -1, -1, -1, -1, -1, -1, -1 };

const double timings[] = { 2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5,
                           2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5,
                           2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2.5,
                           2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2.5 };

const double kSizeX[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1.4, 1.4, 2.0, 2.0, 2.8, 2.8, 3.9, 3.9 };

const double kSizeY[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1.4, 1.4, 2.0, 2.0, 2.8, 2.8, 3.9 };

const COLORREF colors[] = { kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue,
							kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed,
							kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue,
							kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed };

const int kBaseSizeX = (int) (GetSystemMetrics(SM_CXSCREEN) / 6.0);
const int kBaseSizeY = (int) (GetSystemMetrics(SM_CXSCREEN) / 8.0);

const int kFrameCount = 11;
const int kFrameInterval = 33;

static HBITMAP bitmaps[kFrameCount];

std::map<HWND, int> lastFrameIndex;
std::map<HWND, long> lastSwitch;
std::map<HWND, COLORREF> hwndColors;

static HINSTANCE globalHInst;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, int nShowCmd) {
    globalHInst = hInst;

    unsigned char* bitmapBytes[] = { __0_bmp, __1_bmp, __2_bmp, __3_bmp, __4_bmp, __5_bmp, __6_bmp, __7_bmp, __8_bmp, __9_bmp, __10_bmp };
    std::transform(bitmapBytes, bitmapBytes + kFrameCount, bitmaps, LoadBitmapFromBytes);

    // Register the window class.
    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = kClassName;

    RegisterClass(&wc);

    PlayResource(hInst, MUSIC);

    int resX = GetSystemMetrics(SM_CXSCREEN);
    int resY = GetSystemMetrics(SM_CYSCREEN);

    const int totalWindows = sizeof(timings) / sizeof(double);
    HWND windows[totalWindows] = {};

    long lastWindow = 0;

	long time = GetTickCount();
	while (GetTickCount() - time < (3.5 * kUnitTime)) {
		Sleep(50);
	}

    int nextWindow = 0;
    while (true) {
        if (nextWindow == 0 || GetTickCount() - lastWindow >= timings[nextWindow - 1] * kUnitTime) {
            if (nextWindow >= totalWindows) {
                break;
            }

            int width = (int) (kSizeX[nextWindow] * kBaseSizeX);
            int height = (int) (kSizeY[nextWindow] * kBaseSizeY);

            int x;
            int y;
            if (kXOff[nextWindow] == -1) {
                x = resX / 2 - width / 2;
            } else {
                x = (int) (kXOff[nextWindow] * resX);
            }
            if (kYOff[nextWindow] == -1) {
                y = resY / 2 - height / 2;
            } else {
                y = (int) (kYOff[nextWindow] * resY);
            }
            windows[nextWindow] = MakeFrog(hInst, kClassName, x, y, width, height, colors[nextWindow], nShowCmd);
            lastWindow = GetTickCount();
            nextWindow++;
        }

        for (int i = 0; i < totalWindows; i++) {
            HWND hwnd = windows[i];
            if (hwnd != NULL) {
                if (lastFrameIndex[hwnd] < kFrameCount - 1 && GetTickCount() - lastSwitch[hwnd] >= kFrameInterval) {
                    AdvanceFrame(hwnd);
                }
            }
        }
    }

    return 0;
}

HWND MakeFrog(HINSTANCE hInst, LPCTSTR className, int x, int y, int w, int h, COLORREF color, int nCmdShow) {
    HWND hwnd = CreateWindowEx(NULL, kClassName, L"My Dudes", WS_OVERLAPPEDWINDOW,
        x, y, w, h,
        NULL, NULL, hInst, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, std::to_wstring(GetLastError()).c_str(), L"Failed to Launch", MB_ICONERROR);
        return NULL;
    }

	hwndColors[hwnd] = color;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return hwnd;
}

void AdvanceFrame(HWND hwnd) {
    lastFrameIndex[hwnd] = lastFrameIndex[hwnd] + 1;
    lastSwitch[hwnd] = GetTickCount();
    InvalidateRect(hwnd, NULL, true);
    UpdateWindow(hwnd);
}

BOOL PlayResource(HINSTANCE hInst, INT res) {
    BOOL bRtn;
    LPVOID lpRes;
    HANDLE hRes;
    HRSRC hResInfo;

    // Find the wave resource.
    hResInfo = FindResource(hInst, MAKEINTRESOURCE(res), L"WAVE");

    if (hResInfo == NULL)
        return FALSE;

    // Load the wave resource. 
    hRes = LoadResource(hInst, hResInfo);

    if (hRes == NULL)
        return FALSE;

    // Lock the wave resource and play it. 
    lpRes = LockResource(hRes);

    if (lpRes != NULL) {
        bRtn = sndPlaySound((LPTSTR) lpRes, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
    } else
        bRtn = 0;

    return bRtn;
}

HBITMAP LoadBitmapFromBytes(unsigned char bytes[]) {
    BITMAPFILEHEADER* bmfh;
    bmfh = (BITMAPFILEHEADER*) bytes;

    BITMAPINFOHEADER* bmih;
    bmih = (BITMAPINFOHEADER*) (bytes + sizeof(BITMAPFILEHEADER));
    BITMAPINFO* bmi;
    bmi = (BITMAPINFO*) bmih;

    void* bits;
    bits = (void*) (bytes + bmfh->bfOffBits);

    HDC hdc = ::GetDC(NULL);

    HBITMAP hbmp = CreateDIBitmap(hdc, bmih, CBM_INIT, bits, bmi, DIB_RGB_COLORS);

    ::ReleaseDC(NULL, hdc);

    return hbmp;
}

HBITMAP LoadBitmapFromResource(LPTSTR resource) {
    return NULL;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
    {
        lastFrameIndex[hwnd] = 0;
        lastSwitch[hwnd] = GetTickCount();

        break;
    }
    case WM_DESTROY:
    {
        PostQuitMessage(0);

        break;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT     ps;
        HDC             hdc;
        BITMAP          bitmap;
        HDC             hdcMem;
        HGDIOBJ         oldBitmap;

		HBRUSH brush = CreateSolidBrush(hwndColors[hwnd]);
		SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG) brush);

        hdc = BeginPaint(hwnd, &ps);

        HBITMAP hbmp = bitmaps[lastFrameIndex[hwnd]];

        hdcMem = CreateCompatibleDC(hdc);
        oldBitmap = SelectObject(hdcMem, hbmp);

        GetObject(hbmp, sizeof(bitmap), &bitmap);
        RECT rect;
        GetWindowRect(hwnd, &rect);
        AlphaBlend(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top - 38, hdcMem, 0, 0,
			bitmap.bmWidth, bitmap.bmHeight, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);

        EndPaint(hwnd, &ps);

        break;
    }
	case WM_ERASEBKGND:
	{
	}
    default:
    {
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    }

    return 0;
}

