#include "winux.hpp"
#include "winplus.hpp"
using namespace winplus;
using namespace std;

#include <atlimage.h>

#include "framework.h"
#include "GuiTest.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
TCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM MyRegisterClass( HINSTANCE );
BOOL InitInstance( HINSTANCE, int );
LRESULT CALLBACK MainWndProc( HWND, UINT, WPARAM, LPARAM );
INT_PTR CALLBACK AboutDlgProc( HWND, UINT, WPARAM, LPARAM );

int APIENTRY _tWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR lpCmdLine,
    int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    GdiplusInit gdipInit;
    gdipInit.canShutdown(false);
    Win32GUI_ShowConsole();

    // 初始化全局字符串
    LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
    LoadString( hInstance, IDC_GUITEST, szWindowClass, MAX_LOADSTRING );
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if ( !InitInstance( hInstance, nCmdShow ) )
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE(IDC_GUITEST) );

    MSG msg;

    // 主消息循环:
    while ( GetMessage( &msg, nullptr, 0, 0 ) )
    {
        if ( !TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass( HINSTANCE hInstance )
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = MainWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUITEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_GUITEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowEx(
       WS_EX_LAYERED,
       szWindowClass,
       szTitle,
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
       nullptr,
       nullptr,
       hInstance,
       nullptr
   );

   if ( !hWnd ) return FALSE;

   SetLayeredWindowAttributes( hWnd, 0, 255, LWA_ALPHA );
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

MemImage g_img;
MemDC g_memdc;
HWND g_hButtonWnd;
BLENDFUNCTION g_blend;

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    using namespace Gdiplus;

    switch ( message )
    {
    case WM_CREATE:
        cout << "WM_CREATE" << ", " << wParam << ", " << lParam << "\n";
        {
            // 创建按钮
            g_hButtonWnd = CreateWindowEx( 0, "BUTTON", "OK!!", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 50, 100, 30, hWnd, (HMENU)1, NULL, NULL );
            UpdateWindow(g_hButtonWnd);

            g_img.create("cg_dialog1.png");
            g_memdc.create( NULL, g_img.width(), g_img.height() );

            {
                Gdiplus::Graphics g( (HDC)g_memdc );
                g.SetSmoothingMode(SmoothingModeAntiAlias);

                Gdiplus::Status s = g.DrawImage( g_img, 0, 0, g_memdc.width(), g_memdc.height() );

                FillRoundRectangle(g, Gdiplus::SolidBrush(Gdiplus::Color(255,255,255)), Gdiplus::RectF(50,50,100,30), 8 );
            }

            // BLENDFUNCTION
            g_blend.BlendOp = AC_SRC_OVER;
            g_blend.SourceConstantAlpha = 255;
            g_blend.AlphaFormat = AC_SRC_ALPHA;

            POINT ptPos = { 0, 0 };
            SIZE sizeWnd = { g_memdc.width(), g_memdc.height() };

            HDC hScreenDC = GetDC(hWnd);
            //UpdateLayeredWindow(hWnd, hScreenDC, NULL, &sizeWnd, g_memdc, &ptPos, 0, &g_blend, ULW_ALPHA );
            ReleaseDC( hWnd, hScreenDC );

        }
        break;
    case WM_COMMAND:
        cout << "WM_COMMAND" << ", " << wParam << ", " << lParam << "\n";
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_LBUTTONUP:
        cout << message << ", " << wParam << ", " << lParam << "\n";
        {
            HDC hBtnDC = GetDC(g_hButtonWnd);
            //BOOL b=BitBlt( g_memdc, 0, 0, 100, 30, hBtnDC, 0, 0, SRCCOPY );
            //g_memdc.copyFrom(hBtnDC,0,0,100,30,10,10);
            SIZE bmSize = GetHdcBitmapSize(hBtnDC);
            SIZE wndSize = GetHdcWindowSize(hBtnDC);

            cout
                << "BMP(" << bmSize.cx << "," << bmSize.cy << ")" << endl
                << "HWND(" << wndSize.cx << "," << wndSize.cy << ")" << endl
            ;

            g_memdc.copyFrom( hBtnDC, 0, 0, wndSize.cx, wndSize.cy, 0, 0 );
            ReleaseDC(g_hButtonWnd, hBtnDC);

            Bitmap gdipBmp{ g_memdc.getMemBitmap(), NULL };

            //Bitmap_SaveFile(g_memdc.getMemBitmap(),"gmemdc.bmp");

            CLSID clsid;
            ImageEncoderFromExtName("png", &clsid);
            gdipBmp.Save( L"gdip-gmemdc.png", &clsid );

            Bitmap_SaveFile( g_img.ObtainHBITMAP(), "g_img.bmp");

            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;
    case WM_PAINT:
        cout << "WM_PAINT" << ", " << wParam << ", " << lParam << "\n";
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            //g_memdc.transparentEntireTo( hdc, 0, 0, g_memdc.width(), g_memdc.height() );

            AlphaBlend( hdc, 0, 0, g_memdc.width(), g_memdc.height(), g_memdc, 0, 0, g_memdc.width(), g_memdc.height(), g_blend );

            //g.DrawImage()


            cout << "wm_paint: " << ps.rcPaint.left << ", "
                << ps.rcPaint.top << ", "
                << ps.rcPaint.right << ", "
                << ps.rcPaint.bottom << "\n"
                ;

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        cout << "WM_DESTROY" << ", " << wParam << ", " << lParam << "\n";
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK AboutDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
