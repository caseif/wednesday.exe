#pragma once

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void MakeFrog(HINSTANCE hInst, LPCTSTR className, int x, int y, int nCmdShow);
