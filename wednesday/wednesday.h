#pragma once

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WSoDProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND MakeFrog(HINSTANCE hInst, LPCTSTR className, int x, int y, int w, int h, COLORREF color, int nCmdShow);

HWND CreateFullscreenWindow(HWND hwnd, HINSTANCE g_hinst);

inline long TimeMillis();

void AdvanceFrame(HWND hwnd);

BOOL PlayResource(HINSTANCE hInst, INT res);

HBITMAP LoadBitmapFromBytes(unsigned char bytes[]);

HBITMAP LoadBitmapFromResource(HINSTANCE hInst, LPTSTR res);
