#include "winux.hpp"
#include "winplus.hpp"
using namespace winplus;
using namespace std;

#include <atlimage.h>

#include "framework.h"
#include "GuiTest.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE g_hInst; // 当前实例
String g_strMainWndTitle; // 标题栏文本
String g_strMainWndClass; // 主窗口类名

HWND g_hMainWnd;
HWND g_hMainControlsWnd;
MemImage g_imgMainWnd;
MemDC g_memdc;
BLENDFUNCTION g_blend;

// 此代码模块中包含的函数的前向声明:
BOOL AppRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK MainControlsWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
INT_PTR CALLBACK AboutDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam );

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
    g_strMainWndTitle = LoadStringEx( hInstance, IDS_APP_TITLE );
    g_strMainWndClass = LoadStringEx( hInstance, IDC_GUITEST );

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

BOOL AppRegisterClass( HINSTANCE hInstance )
{
    ATOM atom;
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = MainWndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon( hInstance, MAKEINTRESOURCE(IDI_GUITEST) );
    wcex.hCursor        = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground  = (HBRUSH)( COLOR_BTNFACE + 1 );
    wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_GUITEST);
    wcex.lpszClassName  = g_strMainWndClass.c_str();
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // Register first Class.
    atom = RegisterClassEx(&wcex);
    if ( !atom ) return FALSE;

    String strMainControlsWndClass = g_strMainWndClass + "_Controls";

    wcex.lpfnWndProc = MainControlsWndProc;
    wcex.lpszMenuName = NULL;
    wcex.hbrBackground  = NULL;
    //wcex.hbrBackground  = (HBRUSH)( COLOR_WINDOW + 1 );
    wcex.lpszClassName = strMainControlsWndClass.c_str();

    // Register second Class.
    atom = RegisterClassEx(&wcex);
    if ( !atom ) return FALSE;
    return TRUE;
}

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
    g_hInst = hInstance; // 将实例句柄存储在全局变量中

    if ( !AppRegisterClass(hInstance) ) return FALSE;

    g_hMainWnd = CreateWindowEx(
        WS_EX_LAYERED,
        g_strMainWndClass.c_str(),
        g_strMainWndTitle.c_str(),
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );
    if ( !g_hMainWnd ) return FALSE;

    //SetLayeredWindowAttributes( g_hMainWnd, 0, 255, LWA_ALPHA );
    ShowWindow( g_hMainWnd, nCmdShow );
    UpdateWindow(g_hMainWnd);


    return TRUE;
}

LRESULT CALLBACK MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    using namespace Gdiplus;

    switch ( message )
    {
    case WM_CREATE:
        cout << "WM_CREATE" << ", " << wParam << ", " << lParam << "\n";
        {
            // 创建按钮
            //CreateWindowEx( 0, "BUTTON", "Start!", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 50, 100, 30, hWnd, (HMENU)1, NULL, NULL );

            g_imgMainWnd.create("cg_dialog1.png");
            g_memdc.create( NULL, g_imgMainWnd.width(), g_imgMainWnd.height() );

            {
                Gdiplus::Graphics g( (HDC)g_memdc );
                g.SetSmoothingMode(SmoothingModeAntiAlias);
                Gdiplus::Status s = g.DrawImage( g_imgMainWnd, 0, 0, g_memdc.width(), g_memdc.height() );
                //FillRoundRectangle(g, Gdiplus::SolidBrush(Gdiplus::Color(255,255,255)), Gdiplus::RectF(50,50,100,30), 8 );
            }

            // BLENDFUNCTION
            g_blend.BlendOp = AC_SRC_OVER;
            g_blend.SourceConstantAlpha = 255;
            g_blend.AlphaFormat = AC_SRC_ALPHA;

            POINT ptPos = { 0, 0 };
            SIZE sizeWnd = { g_memdc.width(), g_memdc.height() };

            HDC hScreenDC = GetDC(hWnd);
            UpdateLayeredWindow( hWnd, hScreenDC, NULL, &sizeWnd, g_memdc, &ptPos, 0, &g_blend, ULW_ALPHA );
            ReleaseDC( hWnd, hScreenDC );

            // 创建控件窗口
            RECT rcMainWnd;
            GetClientRect( hWnd, &rcMainWnd );
            ClientToScreen(hWnd, (LPPOINT)&rcMainWnd);
            ClientToScreen(hWnd, (LPPOINT)&rcMainWnd + 1);

            String strMainControlsWndClass = g_strMainWndClass + "_Controls";
            g_hMainControlsWnd = CreateWindowEx(
                0,
                strMainControlsWndClass.c_str(),
                g_strMainWndTitle.c_str(),
                WS_POPUP,
                rcMainWnd.left, rcMainWnd.top, g_memdc.width(), g_memdc.height(),
                hWnd,
                nullptr,
                g_hInst,
                nullptr
            );
            ShowWindow( g_hMainControlsWnd, SW_SHOW );
            UpdateWindow(g_hMainControlsWnd);
        }
        break;
    case WM_MOVING:
        {
            RECT rcMainWnd;
            GetClientRect( hWnd, &rcMainWnd );
            LONG dx = rcMainWnd.left, dy = rcMainWnd.top;
            ClientToScreen(hWnd, (LPPOINT)&rcMainWnd);
            dx = rcMainWnd.left - dx, dy = rcMainWnd.top - dy;
            rcMainWnd.right += dx;
            rcMainWnd.bottom += dy;

            SetWindowPos( g_hMainControlsWnd, nullptr, rcMainWnd.left, rcMainWnd.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE );
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
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDOK:
                switch ( HIWORD(wParam) )
                {
                case BN_CLICKED:
                    MsgBox( Window_GetText((HWND)lParam), "MyButton", hWnd);
                    break;
                default:
                    break;
                }
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    /*case WM_LBUTTONUP:
        cout << message << ", " << wParam << ", " << lParam << "\n";
        {
            CLSID clsid;
            ImageEncoderFromExtName("png", &clsid);

            Bitmap gdipbmp1( g_memdc.getBitmap(), NULL );
            gdipbmp1.Save( L"gdip-gmemdc1.png", &clsid );


            BITMAP bm = { 0 };
            GetObject( g_memdc.getBitmap(), sizeof(BITMAP), &bm );

            BITMAPV5HEADER bmiHeader = { 0 };
            bmiHeader.bV5Size = sizeof(BITMAPV5HEADER);
            bmiHeader.bV5Width = bm.bmWidth;
            bmiHeader.bV5Height = -bm.bmHeight;
            bmiHeader.bV5Planes = bm.bmPlanes;
            bmiHeader.bV5BitCount = bm.bmBitsPixel;
            bmiHeader.bV5Compression = BI_RGB;
            bmiHeader.bV5SizeImage = 0;
            bmiHeader.bV5RedMask = 0x00FF0000;
            bmiHeader.bV5GreenMask = 0x0000FF00;
            bmiHeader.bV5BlueMask = 0x000000FF;
            bmiHeader.bV5AlphaMask = 0xFF000000;

            Buffer bitsData;
            bitsData.alloc( bm.bmWidthBytes * bm.bmHeight );
            int nLines = GetDIBits( g_memdc, g_memdc.getBitmap(), 0, bm.bmHeight, bitsData.get(), (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS );

            CLSID clsid1;
            ImageEncoderFromExtName( "bmp", &clsid1 );

            Bitmap gdipbmp2( (LPBITMAPINFO)&bmiHeader, bitsData.get() );
            //cout << "GetPixelFormat: " << (gdipbmp.GetPixelFormat()==PixelFormat32bppRGB) << endl;
            gdipbmp2.Save( L"gdip-gmemdc2.bmp", &clsid1 );

            Bitmap gdipbmp3( bm.bmWidth, bm.bmHeight, bm.bmWidthBytes, PixelFormat32bppARGB, bitsData.get<BYTE>() );
            gdipbmp3.Save( L"gdip-gmemdc3.png", &clsid );

            auto gdipbmp4 = g_memdc.obtainGdiplusBitmap();
            gdipbmp4->Save(L"gdip-gmemdc4.png", &clsid );


            InvalidateRect(hWnd, NULL, TRUE);
        }
        break;*/
    case WM_PAINT:
        cout << "WM_PAINT" << ", " << wParam << ", " << lParam << ", ";
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            g_memdc.alphaEntireTo( hdc, 0, 0, g_memdc.width(), g_memdc.height() );

            cout << "PAINTSTRUCT: " << ps.rcPaint.left << ", " << ps.rcPaint.top << ", " << ps.rcPaint.right << ", " << ps.rcPaint.bottom << "\n";

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

HRGN g_hControlsWndRgn;

LRESULT CALLBACK MainControlsWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    using namespace Gdiplus;

    switch ( message )
    {
    case WM_CREATE:
        cout << "ControlsWnd: WM_CREATE" << ", " << wParam << ", " << lParam << "\n";
        {
            // 创建按钮
            HWND hBtnWnd = CreateWindowEx( 0, "BUTTON", "Start!", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 50, 100, 30, hWnd, (HMENU)1, NULL, NULL );
            RECT rcWnd;
            GetWindowRect( hWnd, &rcWnd );
            RECT rcBtn;
            GetWindowRect( hBtnWnd, &rcBtn );

            rcBtn.left = rcBtn.left - rcWnd.left;
            rcBtn.top = rcBtn.top - rcWnd.top;
            rcBtn.right = rcBtn.right - rcWnd.left;
            rcBtn.bottom = rcBtn.bottom - rcWnd.top;

            g_hControlsWndRgn = CreateRectRgn(0,0,0,0);

            //HRGN hrgn = CreateRectRgn(0,0,0,0);
            //int regionType = GetWindowRgn(hBtnWnd, hrgn);
            //if (regionType != ERROR) 
            //{ 
            //}
            //DeleteObject(hrgn);
            //GetWindowClient();


            HRGN hRgn = CreateRectRgnIndirect(&rcBtn);
            CombineRgn( g_hControlsWndRgn, g_hControlsWndRgn, hRgn, RGN_OR );

            DeleteObject(hRgn);
            SetWindowRgn( hWnd, g_hControlsWndRgn, TRUE );

        }
        break;
    case WM_COMMAND:
        cout << "ControlsWnd: WM_COMMAND" << ", " << wParam << ", " << lParam << "\n";
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutDlgProc);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDOK:
                switch ( HIWORD(wParam) )
                {
                case BN_CLICKED:
                    WarnBox( Window_GetText( (HWND)lParam), "MyButton", g_hMainWnd );
                    break;
                default:
                    break;
                }
                break;
            default:
                return DefWindowProc( hWnd, message, wParam, lParam );
            }
        }
        break;
    case WM_PAINT:
        cout << "ControlsWnd: WM_PAINT" << ", " << wParam << ", " << lParam << ". ";
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint( hWnd, &ps );

            cout << "PAINTSTRUCT: " << ps.rcPaint.left << ", " << ps.rcPaint.top << ", " << ps.rcPaint.right << ", " << ps.rcPaint.bottom << "\n";

            EndPaint( hWnd, &ps );
        }
        break;
    case WM_DESTROY:
        cout << "ControlsWnd: WM_DESTROY" << ", " << wParam << ", " << lParam << "\n";
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
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
