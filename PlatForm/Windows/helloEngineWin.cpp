#include <windows.h>
#include <new>

#define UNICODE TRUE

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

typedef struct StateInfo{
    char a;
}StateInfo;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    const wchar_t CLASS_NAME[] = L"WindowClass1"; 
    
    WNDCLASSEX wc={}; // {} 用默认值初始化结构体

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = "WindowClass1";

    RegisterClassEx(&wc);

    StateInfo *pState = new (std::nothrow) StateInfo;
    HWND hwnd = CreateWindowEx(WS_EX_TRANSPARENT, CLASS_NAME, L"Hello Engine",
        WS_OVERLAPPEDWINDOW,
        300, 300, 500, 400,
        NULL, NULL, hInstance, pState);

    if(hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    MSG msg;

    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }

    return msg.wParam;
}

StateInfo* GetAppState(HWND hwnd){
    LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    return (StateInfo*)ptr;
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
    // 获取对话框对象
    StateInfo *pState;
    if(message == WM_CREATE){
        CREATESTRUCT *pCreate = (CREATESTRUCT*)lParam;
        pState = (StateInfo*)(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pState);
    }else{
        pState = GetAppState(hWnd);
    }

    switch(message){
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rec = {20,20,60,60};
            HBRUSH brush = (HBRUSH) GetStockObject(BLACK_BRUSH);

            FillRect(hdc, &rec, brush);
            EndPaint(hWnd, &ps);
        }
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