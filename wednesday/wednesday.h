#pragma once

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND MakeFrog(HINSTANCE hInst, LPCTSTR className, int x, int y, int w, int h, int nCmdShow);

inline long TimeMillis();

void AdvanceFrame(HWND hwnd);
