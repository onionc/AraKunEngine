#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <new>


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef struct StateInfo{
    char a;
}StateInfo;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    HWND hWnd;

    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "WindowClass1";

    RegisterClassEx(&wc);

    StateInfo *pState = new (std::nothrow) StateInfo;
    hWnd = CreateWindowEx(0, "WindowClass1", "Hello Engine",
        WS_OVERLAPPEDWINDOW,
        300, 300, 500, 400,
        NULL, NULL, hInstance, pState);

    ShowWindow(hWnd, nCmdShow);

    MSG msg;

    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    switch(message){
        case WM_PAINT:
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rec = {20,20,60,60};
            HBRUSH brush = (HBRUSH) GetStockObject(BLACK_BRUSH);

            FillRect(hdc, &rec, brush);
            EndPaint(hWnd, &ps);
            break;
        case WM_KEYDOWN:
            switch(wParam){
                case VK_LEFT:
                    MessageBox(NULL, "test", "zz", MB_OK);
                    SendMessage (hWnd, WM_PAINT, 32, 0) ;
                    break;
            }

            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}