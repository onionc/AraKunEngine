#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>

// 向量
typedef struct Vec{
    float x=0;
    float y=0;

    std::string toString(){
        char a[100];
        sprintf(a, "(%lf,%lf)", x, y);
        return std::string(a);
    }
}Vec;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ballUpdate(Vec &pos);
void move();

// 接球板
static const int width = 50, height = 10, start=0;
static RECT board = {start, 300, start+width, 300+height};
// 小球
static const int ballSize = 10;
static const int ballR = ballSize/2; // 半径
static RECT ball = {start+width/2-ballR, board.top - ballSize, ball.left+ballSize, board.top}; // 初始位置在球板的中间



// 重力
Vec g{0,-9.8};
// 弹力
Vec bounce;
// 摩擦力
Vec force;
// 系数
float k=1;

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


    SetTimer (hwnd, 1, 200, NULL) ;
    
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }

    


    return msg.wParam;
}



// 移动球
void move(){
    
    // 球移动（在重力和弹力的作用下）
    Vec t{g.x+bounce.x, g.y+bounce.y};
    t.x *= k;
    t.y *= k;
    ballUpdate(t);
    // 碰撞检测

    // 弹力衰减

}

// 更新球的位置
void ballUpdate(Vec &pos){
    // 初始位置
    
    Vec start{ball.right-ballR, ball.bottom-ballR};
    Vec pos2{start.x+pos.x, start.y-pos.y};
    ball.left =  pos2.x-ballR;
    ball.right = pos2.x+ballR;
    ball.top = pos2.y-ballR;
    ball.bottom = pos2.y+ballR;

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
            //SelectObject(hdc, brush);
            Ellipse(hdc, ball.left, ball.top, ball.right, ball.bottom);
            //DeleteObject(brush);

            EndPaint(hWnd, &ps);
        }
            break;
        case WM_KEYDOWN:
            switch(wParam){
                case VK_LEFT:
                    if(board.left>=10){
                        board.left-=10;
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
            move();
            InvalidateRect(hWnd, NULL, TRUE);
            break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}