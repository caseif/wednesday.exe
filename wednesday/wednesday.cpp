#define _SCL_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "MSIMG32.lib")
#pragma comment(lib, "Winmm.lib")

#include "wednesday.h"
#include "resources.h"

#include <algorithm>
#include <map>
#include <string>

#include <time.h>
#include <windows.h>
#include <CommCtrl.h>

const wchar_t kClassName[] = L"Wednesday";
const wchar_t kClassNameEnd[] = L"WSoD\0";

const double kUnitTime = 60000 / 145.0;

const COLORREF kRed = RGB(255, 0, 0);
const COLORREF kBlue = RGB(0, 0, 255);

const int xRes = GetSystemMetrics(SM_CXMAXIMIZED);
const int yRes = GetSystemMetrics(SM_CYMAXIMIZED);
const double xSizePart = 0.28;
const double ySizePart = 0.34;

const double borderXSize = 0.1;
const double borderYSize = 0.04;

const double kXStep = (0.5 - borderXSize - xSizePart / 2) / 5;
const double kYStep = (0.5 - borderYSize - ySizePart / 2) / 5;

const double kXOff[] = { -5 * kXStep, -4 * kXStep, -3 * kXStep, -2 * kXStep, -1 * kXStep,
                         0 * kXStep, 1 * kXStep, 2 * kXStep, 3 * kXStep, 4 * kXStep, 5 * kXStep,
						 5 * kXStep, 4 * kXStep, 3 * kXStep, 2 * kXStep, 1 * kXStep, 0 * kXStep,
                         -1 * kXStep, -2 * kXStep, -3 * kXStep, -4 * kXStep, -5 * kXStep,
						 -6.8 * kXStep, -5.3 * kXStep, -4.2 * kXStep, -3.2 * kXStep, -1 * kXStep, 1.7 * kXStep,
                         3 * kXStep, 6.3 * kXStep, 6.3 * kXStep,
                         -1, -1, -1, -1, -1, -1, -1, -1, -1 };

const double kYOff[] = { -5 * kYStep, -4 * kYStep, -3 * kYStep, -2 * kYStep, -1 * kYStep,
                         0 * kYStep, 1 * kYStep, 2 * kYStep, 3 * kYStep, 4 * kYStep, 5 * kYStep,
                         -5 * kYStep, -4 * kYStep, -3 * kYStep, -2 * kYStep, -1 * kYStep,
                         0 * kYStep, 1 * kYStep, 2 * kYStep, 3 * kYStep, 4 * kYStep, 5 * kYStep,
                         5 * kYStep, -5.1 * kYStep, 4 * kYStep, -3.8 * kYStep, 3 * kYStep, -1.3 * kYStep,
                         3.8 * kYStep, -5.1 * kYStep, 5.1 * kYStep,
                         -1, -1, -1, -1, -1, -1, -1, -1, -1 };

const double timings[] = { 2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5,
                           2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 1.5,
                           2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2.5,
                           2.0, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 2.5 };

const double kSizeX[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1.4, 1.4, 2.0, 2.0, 2.8, 2.8, 1. / xSizePart, 1. / xSizePart };

const double kSizeY[] = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1, 1, 1, 1, 1, 1, 1,
                          1, 1, 1.4, 1.4, 2.0, 2.0, 2.8, 2.8, 1. / ySizePart };

const COLORREF colors[] = { kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue,
							kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed,
							kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue, kBlue,
							kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed, kRed };

const int kBaseSizeX = (int) (xRes * xSizePart);
const int kBaseSizeY = (int) (yRes * ySizePart);



const int kFrameCount = 11;
const int kFrameInterval = 33;

static HBITMAP bitmaps[kFrameCount];
static HBITMAP hbmpWSoD;

std::map<HWND, int> lastFrameIndex;
std::map<HWND, long> lastSwitch;
std::map<HWND, COLORREF> hwndColors;

static HINSTANCE globalHInst;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR lpCmdLine, int nShowCmd) {
    //AllocConsole();
    //freopen("CONOUT$", "w", stdout);

    globalHInst = hInst;

    //unsigned char* bitmapBytes[] = { __0_bmp, __1_bmp, __2_bmp, __3_bmp, __4_bmp, __5_bmp, __6_bmp, __7_bmp, __8_bmp, __9_bmp, __10_bmp };
    //std::transform(bitmapBytes, bitmapBytes + kFrameCount, bitmaps, LoadBitmapFromBytes);

    hbmpWSoD = (HBITMAP)LoadBitmapFromResource(hInst, (LPTSTR)IDB_LAST_SCREEN);

    int frameResources[] = {IDB_ANIM_00, IDB_ANIM_01, IDB_ANIM_02, IDB_ANIM_03, IDB_ANIM_04, IDB_ANIM_05, IDB_ANIM_06,
        IDB_ANIM_07, IDB_ANIM_08, IDB_ANIM_09, IDB_ANIM_10};
    for (int i = 0; i < kFrameCount; i++) {
        bitmaps[i] = LoadBitmapFromResource(hInst, (LPTSTR) frameResources[i]);
    }

    WNDCLASS wc = {};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = kClassName;

    wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON));

    RegisterClass(&wc);

    PlayResource(hInst, MUSIC);

    const int totalWindows = sizeof(timings) / sizeof(double);
    HWND windows[totalWindows] = {};

    long lastWindow = 0;

	long time = GetTickCount();
	while (GetTickCount() - time < (3.2 * kUnitTime)) {
		Sleep(50);
	}
    printf("done sleeping at at %d\n", GetTickCount());

    int nextWindow = 0;
    unsigned int nextWindowTime = GetTickCount() - 1;
    while (true) {
        if (nextWindow == 0 || GetTickCount() > nextWindowTime) {
            if (nextWindow >= totalWindows) {
                break;
            }

            if (nextWindow <= totalWindows - 1) {
                nextWindowTime += timings[nextWindow] * kUnitTime;
            }

            int width = (int) (kSizeX[nextWindow] * kBaseSizeX);
            int height = (int) (kSizeY[nextWindow] * kBaseSizeY) + 40;

            int x;
            int y;
            if (kXOff[nextWindow] == -1) {
                x = xRes / 2 - width / 2;
            } else {
                x = (int) (xRes / 2 + kXOff[nextWindow] * xRes - width / 2);
            }
            if (kYOff[nextWindow] == -1) {
                y = yRes / 2 - height / 2;
            } else {
                y = (int) (yRes / 2 + kYOff[nextWindow] * yRes - height / 2);
            }
            if (nextWindow == totalWindows - 1) { // for fixing error on borders in fullscreen
                x = 0;
                y = 0;
                width = xRes;
                height = yRes;
            }
            windows[nextWindow] = MakeFrog(hInst, kClassName, x, y, width, height, colors[nextWindow], nShowCmd);
            lastWindow = GetTickCount();
            nextWindow++;
        }

        for (int i = 0; i < totalWindows; i++) {
            HWND hwnd = windows[i];

            PeekMessage(NULL, hwnd, 0, 0, PM_NOREMOVE);

            if (hwnd != NULL) {
                if (lastFrameIndex[hwnd] < kFrameCount - 1 && GetTickCount() - lastSwitch[hwnd] >= kFrameInterval) {
                    AdvanceFrame(hwnd);
                }
            }
        }
    }

    for (int i = 0; i < totalWindows; i++)
        DestroyWindow(windows[i]);

    WNDCLASS wc_end = {};
    wc_end.lpfnWndProc = WSoDProc;
    wc_end.hInstance = hInst;
    wc_end.lpszClassName = kClassNameEnd;
    RegisterClass(&wc_end);

    HWND hwnd = CreateWindowEx(NULL, kClassNameEnd, L"My Dudes", WS_OVERLAPPEDWINDOW,
        0, 0, 0, 0,
        NULL, NULL, hInst, NULL);
    HWND hwnd_full = CreateFullscreenWindow(hwnd, hInst);
    ShowWindow(hwnd_full, nShowCmd);
    UpdateWindow(hwnd_full);
    Sleep(5000);

    return 0;
}

HWND CreateFullscreenWindow(HWND hwnd, HINSTANCE g_hinst) {
    HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFO mi = { sizeof(mi) };
    if (!GetMonitorInfo(hmon, &mi)) return NULL;
    return CreateWindow(
        kClassNameEnd,
        NULL,
        WS_POPUP | WS_VISIBLE,
        mi.rcMonitor.left,
        mi.rcMonitor.top,
        mi.rcMonitor.right - mi.rcMonitor.left,
        mi.rcMonitor.bottom - mi.rcMonitor.top,
        hwnd, NULL, g_hinst, 0);
}

HWND MakeFrog(HINSTANCE hInst, LPCTSTR className, int x, int y, int w, int h, COLORREF color, int nCmdShow) {
    HMENU hMenubar = CreateMenu();
    HMENU hFile = CreateMenu();
    HMENU hEdit = CreateMenu();
    HMENU hView = CreateMenu();
    HMENU hHelp = CreateMenu();
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hFile, L"File");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hEdit, L"Edit");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hEdit, L"View");
    AppendMenu(hMenubar, MF_POPUP, (UINT_PTR)hHelp, L"Help");

    HWND hwnd = CreateWindowEx(NULL, kClassName, L"My Dudes", WS_OVERLAPPEDWINDOW,
        x, y, w, h,
        NULL, hMenubar, hInst, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, std::to_wstring(GetLastError()).c_str(), L"Failed to Launch", MB_ICONERROR);
        return NULL;
    }

    HWND hStatus = CreateWindowEx(0, STATUSCLASSNAME, NULL,
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0,
        hwnd, NULL, GetModuleHandle(NULL), NULL);
    int statwidths[] = { 100, -1 };
    
    SendMessage(hStatus, SB_SETPARTS, sizeof(statwidths) / sizeof(int), (LPARAM)statwidths);
    SendMessage(hStatus, SB_SETTEXT, 0, (LPARAM)L"It is wednesday");
    
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
        // so the idea here is that sometimes it takes a bit for the sound to actually start playing.
        // by "warming" the sound prior to playing it all the way through, this delay can be effectively eliminated.
        // the "warmed" sound is halted after 50 ms, before the user can notice it's playing.
        printf("warm sound at %d\n", GetTickCount());
        bRtn = sndPlaySound((LPTSTR) lpRes, SND_MEMORY | SND_ASYNC | SND_NODEFAULT);
        Sleep(50);
        sndPlaySound(NULL, SND_SYNC);
        printf("play sound at %d\n", GetTickCount());
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

HBITMAP LoadBitmapFromResource(HINSTANCE hInst, LPTSTR res) {
    return LoadBitmap(hInst, MAKEINTRESOURCE(res));
}

LRESULT CALLBACK WSoDProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            lastFrameIndex[hwnd] = 0;
            lastSwitch[hwnd] = GetTickCount();
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            break;
        }
        case WM_PAINT: {
            PAINTSTRUCT     ps;
            HDC             hdc;
            BITMAP          bitmap;
            HDC             hdcMem;
            HGDIOBJ         oldBitmap;

            HBRUSH brush = CreateSolidBrush(hwndColors[hwnd]);
            SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG)brush);

            hdc = BeginPaint(hwnd, &ps);

            HBITMAP hbmp = hbmpWSoD;

            hdcMem = CreateCompatibleDC(hdc);
            oldBitmap = SelectObject(hdcMem, hbmp);

            GetObject(hbmp, sizeof(bitmap), &bitmap);
            RECT rect;
            GetWindowRect(hwnd, &rect);
            AlphaBlend(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, hdcMem, 0, 0,
                bitmap.bmWidth, bitmap.bmHeight, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
            SelectObject(hdcMem, oldBitmap);
            DeleteDC(hdcMem);

            EndPaint(hwnd, &ps);

            break;
        }
        case WM_ERASEBKGND: {
        }
        default: {
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return 0;
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