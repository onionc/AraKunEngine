#include <windows.h>
#include <tchar.h>
#include <string>
#include <iostream>
#include <fstream>


// log
std::ofstream wf;

// 创建时指定的窗口大小
#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 400
RECT clientRect; // 真实客户区大小

// 向量
typedef struct VecS{
    float x=0;
    float y=0;

    std::string toString(){
        char a[100];
        sprintf(a, "(%lf,%lf)", x, y);
        return std::string(a);
    }
}Vec;

// 矩形，通过左上角（x,y）点和宽高来表示
typedef struct RectXyS{
    long x;
    long y;
    long w;
    long h;

    RectXyS(RECT &r){
        x = r.left;
        y = r.top;
        w = r.right - r.left;
        h = r.bottom - r.top;
    }
}RectXy;


// 接球板
static const int width = 50, height = 10, start=150;
static RECT board = {start, 300, start+width, 300+height};
// 小球
static const int ballSize = 10;
static const int ballR = ballSize/2; // 半径
static RECT ball = {start+width/2-ballR, board.top - ballSize, ball.left+ballSize, board.top}; // 初始位置在球板的中间



// 重力
Vec g{0,-3};
// 弹力
Vec bounce;
// 摩擦力
Vec force;
// 系数
//float k=1/9.8+1e-3; // 重力作用下的最小x像素运动的系数


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void ballUpdate(Vec &pos);
void move();
bool hitDetection(RECT &rect1, RECT &rect2);

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
        300, 300, WINDOW_WIDTH, WINDOW_HEIGHT,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL) return 0;

    ShowWindow(hwnd, nCmdShow);

    // log
    wf.open("run.log", std::ios::out | std::ios::trunc );

    // 获取客户区大小
    GetClientRect(hwnd, &clientRect);
    wf<<"client size:"<<clientRect.right<<","<<clientRect.bottom;

    // 定时器
    SetTimer (hwnd, 1, 100, NULL) ;
    
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);

        DispatchMessage(&msg);
    }

    


    return msg.wParam;
}



// 移动球
void move(){
    RECT ballOld = ball;

    wf << "bounce="<<bounce.toString()<<std::endl;
    // 球移动（在重力和弹力的作用下）
    Vec t{g.x+bounce.x, g.y+bounce.y};

    ballUpdate(t);
    // 板和球 碰撞检测
    if(hitDetection(ball, board)){
        // 碰撞，首先撤回
        ball = ballOld; // todo: 应该贴近，直接撤回会有卡顿

        // 弹力为两个中心连线向量，方向从board到ball, 大小随机
        float x = (ball.right+ball.left)/2.0 - (board.right+board.left)/2.0;
        float y = (ball.bottom+ball.top)/2.0 - (board.bottom+board.top)/2.0;

        float k2 =0.1;
        
        wf << "x="<<x<<",y="<<y<<std::endl;
        bounce.x += x*k2;
        bounce.y += y*k2;
    }

    // 墙壁碰撞
    if(ball.left<1e-5 || ball.right>clientRect.right){
        bounce.x*=-1;
        ball = ballOld;
    }else if(ball.top<1e-5 || ball.bottom>clientRect.bottom){
        bounce.y*=-1;
        bounce.y+=g.y;
        ball = ballOld;
    }


    // 弹力衰减
    //if(bounce.x>0.3) bounce.x-=0.15;
    //if(bounce.x<-0.3) bounce.x+=0.15;
    //if(bounce.y>0.3) bounce.y-=0.15;
    //if(bounce.y<-0.3) bounce.y+=0.15;

}
// 碰撞检测
bool hitDetection(RECT &rect1, RECT &rect2){
    // 碰撞计算
    if(
        rect1.left < rect2.right &&
        rect1.right > rect2.left &&
        rect1.top < rect2.bottom &&
        rect1.bottom > rect2.top 
    ){
        
        return true;
    }
                
    return false;

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

    wf << "start:" << start.toString() << std::endl;
    wf << "+:" << pos.toString() << std::endl;
    wf << "ball:" << pos2.toString() << std::endl;
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