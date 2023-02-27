#include <windows.h>
#include <tchar.h>


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// 接球板
static const int width = 50, height = 10, start=0;
static RECT board = {start, 300, start+width, 300+height};
// 小球
static const int ballSize = 10;
static RECT ball = {start+width/2-ballSize/2, board.top - ballSize, ball.left+ballSize, board.top};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    const TCHAR CLASS_NAME[] = _T("WindowClass1"); 
    
    WNDCLASSEX wc={}; // {} 用默认值初始化结构体

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassEx(&wc);

    HWND hwnd = CreateWindowEx(0, CLASS_NAME, _T("ping"),
        WS_OVERLAPPEDWINDOW,
        300, 300, 500, 400,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);


    SetTimer (hwnd, 1, 1000, NULL) ;
    
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }

    


    return msg.wParam;
}


// 重力
float g = 9.8;
// 弹力
float f = 0;
// 弹力方向
float fpoint = 90;

// 系数
float k=0.1;

// 移动球
void move(){
    if(fpoint>1e-4 && fpoint-1e-4<90){
        // 0~90

    }


    // 碰撞检测

    // 弹力衰减
    if(f>5){
        f-=5;
    }
    if(fpoint>2){
        fpoint-=2;
    }

}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){

    switch(message){
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HBRUSH brush = (HBRUSH) GetStockObject(BLACK_BRUSH);
            FillRect(hdc, &board, brush);

            // 画圆
            SelectObject(hdc, brush);
            Ellipse(hdc, ball.left, ball.top, ball.right, ball.bottom);
            DeleteObject(brush);

            EndPaint(hWnd, &ps);
        }
            break;
        case WM_KEYDOWN:
            switch(wParam){
                case VK_LEFT:
                    if(board.left>=10){
                        board.left-=10;
                        ball.left-=10;
                        board.right = board.left+width;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    
                    break;
                case VK_RIGHT:
                    if(board.right<(500-10)){
                        board.right+=10;
                        board.left = board.right-width;
                        InvalidateRect(hWnd, NULL, TRUE);
                    }
                    
                    break;
                
            }

            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            break;
        case WM_TIMER:
            ball.right++;
            InvalidateRect(hWnd, NULL, FALSE);
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}