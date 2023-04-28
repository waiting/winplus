#include "winux.hpp"
#include "winplus.hpp"
using namespace winplus;
using namespace std;

#include <atlimage.h>

#include "framework.h"
#include <windowsx.h>
#include "GuiTest.h"
#include "resource.h"

#define MAX_LOADSTRING 100

HINSTANCE g_hInst; // 当前实例
String g_strMainWndTitle; // 标题栏文本
String g_strMainWndClass; // 主窗口类名

HWND g_hwndMain; // 主窗口
HWND g_hwndControls; // 控件窗口
MemImage g_imgMainWnd;
MemDC g_memdc;

// 主窗口客户区位置偏移
POINT g_ptMainWndClientOffset;
HRGN g_hControlsWndRgn;

// 此代码模块中包含的函数的前向声明:
BOOL AppRegisterClass( HINSTANCE hInstance );
BOOL InitInstance( HINSTANCE hInstance, int nCmdShow );
LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK ControlsWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
LRESULT CALLBACK MyStaticWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
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
    wcex.hbrBackground  = (HBRUSH)( COLOR_3DFACE + 1 );
    wcex.lpszMenuName   = NULL;
    //wcex.lpszMenuName   = MAKEINTRESOURCE(IDC_GUITEST);
    wcex.lpszClassName  = g_strMainWndClass.c_str();
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    // Register first Class.
    atom = RegisterClassEx(&wcex);
    if ( !atom ) return FALSE;

    String strControlsWndClass = g_strMainWndClass + "_Controls";

    wcex.lpfnWndProc = ControlsWndProc;
    wcex.lpszMenuName = NULL;
    wcex.hbrBackground  = NULL;
    //wcex.hbrBackground  = (HBRUSH)( COLOR_WINDOW + 1 );
    wcex.lpszClassName = strControlsWndClass.c_str();

    // Register second Class.
    atom = RegisterClassEx(&wcex);
    if ( !atom ) return FALSE;

    String strStaticWndClass = "MY_STATIC";

    wcex.lpfnWndProc = MyStaticWndProc;
    wcex.lpszMenuName = NULL;
    wcex.hbrBackground  = NULL;
    //wcex.hbrBackground  = (HBRUSH)( COLOR_WINDOW + 1 );
    wcex.lpszClassName = strStaticWndClass.c_str();

    // Register second Class.
    atom = RegisterClassEx(&wcex);
    if ( !atom ) return FALSE;

    return TRUE;
}

BOOL InitInstance( HINSTANCE hInstance, int nCmdShow )
{
    g_hInst = hInstance; // 将实例句柄存储在全局变量中

    if ( !AppRegisterClass(hInstance) ) return FALSE;

    g_hwndMain = CreateWindowEx(
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
    if ( !g_hwndMain ) return FALSE;

    //SetLayeredWindowAttributes( g_hwndMain, 0, 255, LWA_ALPHA );
    ShowWindow( g_hwndMain, nCmdShow );
    UpdateWindow(g_hwndMain);

    return TRUE;
}

LRESULT CALLBACK MainWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    using namespace Gdiplus;

    switch ( message )
    {
    case WM_NCCALCSIZE:
        cout << "MainWnd WM_NCCALCSIZE " << wParam << endl;
        if ( wParam )
        {
            // 一定要处理wParam==TRUE时的情况，否则GetClientRect()无法正确获得客户区
            NCCALCSIZE_PARAMS & params = *(LPNCCALCSIZE_PARAMS)lParam;
            // rgrc[0] Dest, rgrc[1] Src, rgrc[2] Src-client
            cout << "first rect: " << params.rgrc[0] << endl;
            cout << "secnd rect: " << params.rgrc[1] << endl;
            cout << "third rect: " << params.rgrc[2] << endl;
 
            // rgrc[0] Dest-client, rgrc[1] Dest, rgrc[2] Src
            swap( params.rgrc[0], params.rgrc[1] );
            swap( params.rgrc[0], params.rgrc[2] );

            cout << "dest client:" << params.rgrc[0] << endl;
            return 0; //DefWindowProc( hWnd, message, wParam, lParam );
        }
        else
        {
            // 窗口第一次创建时，客户区大小别去管它，直接修改四边边距即可。
            RECT & rect = *(LPRECT)lParam;
            RECT rcOrigin = rect;
            cout << "new origin:" << rcOrigin << endl;

            rect.left = rcOrigin.left + 15;
            rect.top = rcOrigin.top + 27;
            rect.right = rcOrigin.right - 27;
            rect.bottom = rcOrigin.bottom - 12;

            cout << "new dest client:" << rect << endl;

            return 0; //DefWindowProc( hWnd, message, wParam, lParam );
        }
        break;
    case WM_NCHITTEST:
        {
            POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
            ScreenToClient( hWnd, &pt );
            //cout << "MainWnd WM_NCHITTEST " << pt.x << ", " << pt.y << "\n";
            RECT rcClient;
            GetClientRect(hWnd,&rcClient);
            //cout << "client: " << rcClient << endl;
            
            return HTCAPTION;
        }
        break;
    case WM_CREATE:
        cout << "MainWnd WM_CREATE" << ", " << wParam << ", " << lParam << "\n";
        {
            // 创建图片
            //g_imgMainWnd.create("cg_dialog1.png");
            g_imgMainWnd.create( CreateStreamFromResource(IDR_PNG1, "PNG") );
            //Picture_Load();

            if ( g_imgMainWnd ) cout << "image create ok:" << g_imgMainWnd.width() << ", " << g_imgMainWnd.height() << endl;
            g_memdc.create( NULL, g_imgMainWnd.width(), g_imgMainWnd.height() );

            {
                winplus::Graphics g( (HDC)g_memdc );
                g.SetSmoothingMode(SmoothingModeAntiAlias);
                //SolidBrush brush( Color( 128, 255,255,0 ) );
                //g.DrawBackground( RectF(0,0,g_memdc.width(), g_memdc.height()), &brush, 14 );
                Gdiplus::Status s = g.DrawImage( g_imgMainWnd, 0, 0, g_memdc.width(), g_memdc.height() );
                //g.FillRoundRectangle( &Gdiplus::SolidBrush(Gdiplus::Color(125,255,255,255)), Gdiplus::RectF(50,50,100,30), 8 );
            }

            BLENDFUNCTION blend;
            blend.BlendOp = AC_SRC_OVER;
            blend.SourceConstantAlpha = 255;
            blend.AlphaFormat = AC_SRC_ALPHA;

            POINT ptPos = { 0, 0 };
            SIZE sizeWnd = { g_memdc.width(), g_memdc.height() };

            //HDC hScreenDC = GetDC(hWnd);
            //UpdateLayeredWindow( hWnd, hScreenDC, NULL, &sizeWnd, g_memdc, &ptPos, 0, &blend, ULW_ALPHA );
            UpdateLayeredWindow( hWnd, NULL, NULL, &sizeWnd, g_memdc, &ptPos, 0, &blend, ULW_ALPHA );
            //ReleaseDC( hWnd, hScreenDC );

            // 创建控件窗口
            RECT rcMainWndClient, rcMainWndRect;
            GetClientRect( hWnd, &rcMainWndClient );
            ClientToScreen( hWnd, &rcMainWndClient );

            // 计算客户区相对于窗口的偏移量
            GetWindowRect( hWnd, &rcMainWndRect );
            g_ptMainWndClientOffset.x = rcMainWndClient.left - rcMainWndRect.left;
            g_ptMainWndClientOffset.y = rcMainWndClient.top - rcMainWndRect.top;

            String strMainControlsWndClass = g_strMainWndClass + "_Controls";
            g_hwndControls = CreateWindowEx(
                0,
                strMainControlsWndClass.c_str(),
                g_strMainWndTitle.c_str(),
                WS_POPUP,
                rcMainWndClient.left, rcMainWndClient.top, RectWidth(rcMainWndClient), RectHeight(rcMainWndClient),
                hWnd,
                nullptr,
                g_hInst,
                nullptr
            );
            ShowWindow( g_hwndControls, SW_SHOW );
            UpdateWindow(g_hwndControls);
        }
        break;
    case WM_MOVING:
        {
            RECT & rcMainWnd = *(LPRECT)lParam;

            SetWindowPos( g_hwndControls, nullptr, rcMainWnd.left + g_ptMainWndClientOffset.x, rcMainWnd.top + g_ptMainWndClientOffset.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE );

            //InvalidateRect(hWnd, NULL, FALSE);
        }
        break;
    case WM_COMMAND:
        cout << "MainWnd WM_COMMAND" << ", " << wParam << ", " << lParam << "\n";
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
        cout << "MainWnd WM_PAINT" << ", " << wParam << ", " << lParam << "\n";
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);

            // TODO: 在此处添加使用 hdc 的任何绘图代码...
            //g_memdc.alphaEntireTo( hdc, 0, 0, g_memdc.width(), g_memdc.height() );

            cout << "PAINTSTRUCT: " << ps.rcPaint << "\n";

            //Rectangle(hdc,0,0,100,30);
            //SetLayeredWindowAttributes(hWnd, 0, 255, LWA_ALPHA);


            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        cout << "MainWnd WM_DESTROY" << ", " << wParam << ", " << lParam << "\n";
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT CALLBACK ControlsWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    using namespace Gdiplus;

    switch ( message )
    {
    case WM_CREATE:
        cout << "ControlsWnd: WM_CREATE" << ", " << wParam << ", " << lParam << "\n";
        {
            RECT rcWnd;
            GetWindowRect( hWnd, &rcWnd );

            //HWND hwndStatic = CreateWindowEx( WS_EX_TRANSPARENT, "MY_STATIC", "Hello world", WS_CHILD | WS_VISIBLE, 0, 0, 100, 52, hWnd, (HMENU)-1, NULL, NULL );
            // 创建按钮
            HWND hBtnWnd = CreateWindowEx( 0, "BUTTON", "Start!", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 50, 30, 100, 30, hWnd, (HMENU)1, NULL, NULL );
            RECT rcBtn;
            GetWindowRect( hBtnWnd, &rcBtn );
            rcBtn.left = rcBtn.left - rcWnd.left;
            rcBtn.top = rcBtn.top - rcWnd.top;
            rcBtn.right = rcBtn.right - rcWnd.left;
            rcBtn.bottom = rcBtn.bottom - rcWnd.top;

            InflateRect( &rcBtn, -1, -1 );

            g_hControlsWndRgn = CreateRectRgn(0,0,0,0);

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
                    {
                        //while (1)
                        {
                            Registry reg( R"(HKCR\Directory\Background\shell\TidyFolderTime)", true );
                            auto v = reg.getValue("");
                            cout << Registry::ValueType(v) << ", " << Registry::Value(v);
                            cout << "\n";
                            v = reg.getValue("exv");
                            cout << Registry::ValueType(v) << ", " << Registry::Value(v);
                            cout << "\n";
                            v = reg.getValue("dv");
                            cout << Registry::ValueType(v) << ", " << Registry::Value(v);
                            cout << "\n";
                            v = reg.getValue("qv");
                            cout << Registry::ValueType(v) << ", " << Registry::Value(v);
                            cout << "\n";
                            v = reg.getValue("mul");
                            cout << Registry::ValueType(v) << ", " << Registry::Value(v);
                            cout << "\n";

                            //Registry::ForceDelete( R"(HKCR\Directory\Background\shell\TidyFolderTime\bb)" );
                            cout << boolalpha << reg.hasValue("") << endl;
                            cout << boolalpha << reg.hasValue("qv") << endl;

                            String name;
                            //Mixed v;
                            while ( reg.enumValues( &name, &v ) )
                            {
                                cout << name << " = " << Registry::Value(v) << endl;
                            }

                            Registry reg2(R"(HKCR\Directory\Background\shell)");
                            String key;
                            while ( reg2.enumKeys( &key ) )
                            {
                                cout << key << endl;
                                Registry reg3( reg2.key(), key );
                                while ( reg3.enumKeys(&key) )
                                {
                                    cout << "\t" << key << endl;
                                }
                            }
                        }

                    }
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
        cout << "ControlsWnd: WM_PAINT" << ", " << wParam << ", " << lParam << "\n";
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint( hWnd, &ps );
            /*SimpleHandle<HPEN> pen( CreatePen( PS_SOLID, 1, RGB(255,0,0) ), NULL, DeleteObject );
            GdiObjectSelector selPen( hdc, pen.get() );
            GdiObjectSelector selBrush( hdc, GetStockObject(NULL_BRUSH) );

            cout << "PAINTSTRUCT: " << ps.rcPaint.left << ", " << ps.rcPaint.top << ", " << ps.rcPaint.right << ", " << ps.rcPaint.bottom << "\n";
            RECT rcClient;
            GetClientRect( hWnd, &rcClient );
            MoveToEx( hdc, 0, 0, nullptr );
            LineTo(hdc, rcClient.right-1, rcClient.bottom-1);*/

            EndPaint( hWnd, &ps );
        }
        break;
    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}

LRESULT CALLBACK MyStaticWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    using namespace Gdiplus;

    switch ( message )
    {
    case WM_CREATE:
        cout << "MyStaticWnd: WM_CREATE" << ", " << wParam << ", " << lParam << "\n";
        {
        }
        break;
    case WM_PAINT:
        cout << "MyStaticWnd: WM_PAINT" << ", " << wParam << ", " << lParam << "\n";
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint( hWnd, &ps );

            //TextOut( hdc, 0, 0, "Hello world", 11 );
            winplus::Graphics g(hdc);
            Gdiplus::Font font(L"宋体", 36);
            SolidBrush brush( Color(125, 255,0,0) );
            StringFormat sf;
            //sf.SetAlignment(StringAlignmentCenter);
            g.DrawString( L"Hello world", 11, &font, PointF(10,10), &sf, &brush );

            EndPaint( hWnd, &ps );
        }
        break;
    default:
        return DefWindowProc( hWnd, message, wParam, lParam );
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK AboutDlgProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
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
