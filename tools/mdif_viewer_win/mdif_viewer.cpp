/* 
 * Copyright 2024 Nathanne Isip
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "mdif.h"

#define WINDOW_CLASS    "MDIFWindowClass"
#define WINDOW_CLASSL   L"MDIFWindowClass"

mdif_t image;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch(msg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            BITMAPINFO bmi = {0};

            bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth       = image.width;
            bmi.bmiHeader.biHeight      = -image.height;
            bmi.bmiHeader.biPlanes      = 1;
            bmi.bmiHeader.biBitCount    = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            int imageSize = image.width * image.height;
            char *imageData = (char*) malloc(imageSize * 4);

            for(int i = 0; i < imageSize; i++) {
                imageData[i * 4]        = image.blue[i];
                imageData[i * 4 + 1]    = image.green[i];
                imageData[i * 4 + 2]    = image.red[i];
                imageData[i * 4 + 3]    = image.alpha[i];
            }

            StretchDIBits(
                hdc, 0, 0,
                image.width, image.height,
                0, 0,
                image.width, image.height,
                imageData, &bmi,
                DIB_RGB_COLORS, SRCCOPY
            );
            EndPaint(hwnd, &ps);

            free(imageData);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

const char* GetMDIFFilename(int argc, LPWSTR* argv) {
    if(argc < 2) {
        MessageBoxW(
            NULL,
            L"Usage: mdif_viewer <filename.mdif>",
            L"Error",
            MB_OK | MB_ICONERROR
        );
        return NULL;
    }

    int bufferSize = WideCharToMultiByte(
        CP_UTF8, 0,
        argv[1], -1,
        NULL, 0,
        NULL, NULL
    );

    if(bufferSize == 0) {
        MessageBoxW(
            NULL,
            L"Failed to convert filename to UTF-8",
            L"Error",
            MB_OK | MB_ICONERROR
        );

        return NULL;
    }

    char *filename = (char*) malloc(bufferSize);
    WideCharToMultiByte(
        CP_UTF8, 0,
        argv[1], -1,
        filename, bufferSize,
        NULL, NULL
    );

    return filename;
}

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd
) {
    int nArgs = 0;
    LPWSTR *szArglist = CommandLineToArgvW(
        GetCommandLineW(),
        &nArgs
    );
    const char *mdifFilename = GetMDIFFilename(nArgs, szArglist);

    if(mdifFilename == NULL) {
        LocalFree(szArglist);
        return -1;
    }

    mdif_error_t result = mdif_read(mdifFilename, &image);
    if(result != MDIF_ERROR_NONE) {
        MessageBox(
            NULL,
            "Failed to read MDIF file",
            mdif_error_message(result),
            MB_OK | MB_ICONERROR
        );
        return -1;
    }

    RECT rect = {0, 0, image.width - 24, image.height - 24};
    WNDCLASS wc = { };

    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = WINDOW_CLASS;

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    RegisterClass(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        WINDOW_CLASSL,
        L"MDIF Viewer",
        WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
        (GetSystemMetrics(SM_CXSCREEN) - (rect.right - rect.left)) / 2,
        (GetSystemMetrics(SM_CYSCREEN) - (rect.bottom - rect.top)) / 2,
        rect.right - rect.left, rect.bottom - rect.top,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if(hwnd == NULL)
        return 0;
    ShowWindow(hwnd, nShowCmd);

    MSG msg = {};
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    mdif_free(&image);
    return 0;
}
