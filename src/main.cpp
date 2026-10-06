#include <Windows.h>
#include <cstdint>
#include "lin.hpp"

constexpr int WIDTH = 1920;
constexpr int HEIGHT = 1080;

// GDI resources
HDC memoryDC = nullptr;
HBITMAP bitmap = nullptr;
HGDIOBJ oldBitmap = nullptr;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND windowsCreate(HINSTANCE hInstance){
    
    
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    RECT rect = { 0, 0, WIDTH, HEIGHT };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Software Renderer", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
                               nullptr, nullptr, hInstance, nullptr);

    if (!hwnd){
        NULL;
    }
        return hwnd;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, int nCmdShow) {

    HWND hwnd = windowsCreate(hInstance);
    
    if (!hwnd) {
        return 1;
    }

    // Initialize memory device context
    memoryDC = CreateCompatibleDC(nullptr);

    if (!memoryDC)
        return 1;

    // Configure DIB
    BITMAPINFO bmi = {};

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = WIDTH;
    bmi.bmiHeader.biHeight = -HEIGHT; // Top-down bitmap
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    // Create framebuffer
    void* bitmapMemory = nullptr;

    bitmap = CreateDIBSection(memoryDC, &bmi, DIB_RGB_COLORS, &bitmapMemory, nullptr, 0);

    if (!bitmap || !bitmapMemory) {
        DeleteDC(memoryDC);
        return 1;
    }

    oldBitmap = SelectObject(memoryDC, bitmap);

    //Paintint proccess    

    lin::init(WIDTH, HEIGHT, static_cast<uint32_t*>(bitmapMemory));
    lin::clear(0x0000FF00);
    lin::drawCircle(70, 70, 60, 0x00000000);
    lin::drawLine(150, 150, 700, 150, 0x00000000);
    lin::drawRect(300, 300, 500, 500, 0x00000000);
    lin::drawTriangle(800, 200, 200, 0, 100, 700, 0x00000000);

    ShowWindow(hwnd, nCmdShow);

    // Message loop
    MSG msg = {};

    while (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Cleanup
    SelectObject(memoryDC, oldBitmap);
    DeleteObject(bitmap);
    DeleteDC(memoryDC);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC targetDC = BeginPaint(hwnd, &ps);

            RECT clientRect;
            GetClientRect(hwnd, &clientRect);

            // Display framebuffer
            StretchBlt(targetDC, 0, 0, clientRect.right, clientRect.bottom, memoryDC, 0, 0, WIDTH, HEIGHT, SRCCOPY);

            EndPaint(hwnd, &ps);
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
