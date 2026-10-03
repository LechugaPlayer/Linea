#include <Windows.h>
#include <minwindef.h>
#include <windef.h>
#include <winuser.h>

// Register the window class.
namespace lin{
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  
  int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow){
    
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";
    
    WNDCLASS wc = { };
    
    wc.lpfnWndProc   = DefWindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Sample Window Class",
                                 WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                                 NULL, NULL, hInstance, NULL);
    
    if (hwnd == NULL) {
      return 0;
    }

  ShowWindow(hwnd, iCmdShow);

  return 1;  
  }
  
}

int main(){
  
  return 0;  
}
  
