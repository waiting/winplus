//////////////////////////////////////////////////////////////////////////
// Purpose:   
// Desc:      WinPlus sample 6
// Author:    WT
// Date:      2013-09-26
//////////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;

#include <windows.h>
#include <CommCtrl.h>

//#pragma comment ( lib, "comctl32" )

#include <winplus.hpp>
using namespace winplus;

#include "resource.h"

WINPLUS_FUNC_IMPL(HWND) GetConsoleHwnd( String * pConsoleTitle = NULL )
{
    static String consoleTitle;
    if ( !pConsoleTitle )
    {
        pConsoleTitle = &consoleTitle;
    }

    int const MY_BUFSIZE = 1024; // Buffer size for console window titles.

    pConsoleTitle->resize(MY_BUFSIZE);
    GetConsoleTitle( &(*pConsoleTitle)[0], pConsoleTitle->size() );

    HWND hwndFound = NULL;         // This is what is returned to the caller.

    // Format a "unique" NewWindowTitle.
    String newWindowTitle = Format( "%d/%d", GetTickCount(), GetCurrentProcessId() );

    // Change current window title.
    SetConsoleTitle( newWindowTitle.c_str() );

    // Ensure window title has been updated.
    int tryCount = 40;
    do 
    {
        Sleep(1);
        // Look for NewWindowTitle.
        hwndFound = FindWindow( NULL, newWindowTitle.c_str() );
    } while ( !hwndFound &&  --tryCount );

    // Restore original window title.
    SetConsoleTitle( pConsoleTitle->c_str() );

    return hwndFound;
}

WNDPROC __wndProcDefault;
LRESULT CALLBACK ConsoleWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch ( uMsg )
    {
    case WM_RBUTTONDOWN:
        MsgBox("popup menu");
        //return 0;
        break;
    }
    return CallWindowProc( __wndProcDefault, hwnd, uMsg, wParam, lParam );
}

void TaskBox( HWND hwndConsole, String consoleTitle )
{
/*
    int nButtonPressed = 0;
    TaskDialog(
        hwndConsole,
        NULL,
        LocalToUnicode(consoleTitle).c_str(),
        L"内标题",
        LocalToUnicode(consoleTitle).c_str(),
        TDCBF_OK_BUTTON | TDCBF_CANCEL_BUTTON|0x3f,
        TD_WARNING_ICON, 
        &nButtonPressed
        );

    if ( nButtonPressed == IDOK )
    {
        //cin >> consoleTitle;
    }*/
}

String ErrStr( DWORD dwErr )
{
    char sz[100] = "";
    FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, dwErr, 0, sz, 100, NULL );
    return sz;
}

void Subclassing( HWND hwnd )
{
    __wndProcDefault = (WNDPROC)GetWindowLong( hwnd, GWL_WNDPROC );
    auto r = SetWindowLong( hwnd, GWL_WNDPROC, (LONG)&ConsoleWndProc );
    if ( !r )
    {
        auto err = GetLastError();
        cout << "子类化控制台窗口错误:"<< ErrStr(err) ;//<< endl;
    }
}
/*

class Notification
{
private:
    NOTIFYICONDATA _nid;
    bool _isAdd;
protected:
    void _construct()
    {
        _nid.cbSize = sizeof(_nid);
        _nid.uFlags = 0;
        _isAdd = false;
    }

public:
    Notification()
    {
        _construct();
    }
    ~Notification()
    {
        _isAdd && this->del();
    }
    bool add( HWND hwnd, UINT notificationId, Icon const & ico, String const & strTip )
    {
        _nid.hWnd = hwnd;
        _nid.uID = notificationId;

        _nid.uFlags = 0;
        this->setIconInfo( ico, strTip );

        if ( !Shell_NotifyIcon( NIM_ADD, &_nid ) )
            return false;

        _isAdd = true;
        _nid.uVersion = NOTIFYICON_VERSION_4;
        return Shell_NotifyIcon(NIM_SETVERSION, &_nid) != FALSE;
    }

    bool modify()
    {
        return Shell_NotifyIcon( NIM_MODIFY, &_nid ) != FALSE;
    }

    void setMessage( UINT msg )
    {
        _nid.uFlags |= NIF_MESSAGE;
        _nid.uCallbackMessage = msg;
    }

    void setBalloonInfo( String const & infoTitle, String const & infoText, DWORD infoFlags = NIIF_USER | NIIF_RESPECT_QUIET_TIME )
    {
        _nid.uFlags |= NIF_INFO;

        _nid.dwInfoFlags = infoFlags;
        SZ(_nid.szInfoTitle) = infoTitle.c_str();
        SZ(_nid.szInfo) = infoText.c_str();
    }

    void setIconInfo( Icon const & ico, String const & strTip )
    {
        _nid.uFlags |= NIF_ICON | NIF_TIP | NIF_SHOWTIP;
        _nid.hIcon = ico;
        SZ(_nid.szTip) = strTip.c_str();
    }

    bool del()
    {
        bool r = Shell_NotifyIcon( NIM_DELETE, &_nid ) != FALSE;
        _isAdd = false;
        return r;
    }
};
*/

BOOL AddNotificationIcon( HWND hwnd, UINT id )
{
    NOTIFYICONDATA nid = { sizeof(nid) };
    nid.hWnd = hwnd;
    nid.uID = id;
    // add the icon, setting the icon, tooltip, and callback message.
    // the icon will be identified with the GUID
    nid.uFlags = NIF_ICON | NIF_TIP | NIF_SHOWTIP ;
    //nid.guidItem = __uuidof(PrinterIcon);
    //nid.uCallbackMessage = WMAPP_NOTIFYCALLBACK;
    LoadIconMetric( GetModuleHandle(NULL), MAKEINTRESOURCEW(IDI_ICON1), LIM_SMALL, &nid.hIcon );
    //nid.hIcon = LoadIcon( GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1) );
    //LoadString(g_hInst, IDS_TOOLTIP, nid.szTip, ARRAYSIZE(nid.szTip));
    SZ(nid.szTip) = "这是个工具提示串\n试试换行";

    Shell_NotifyIcon( NIM_ADD, &nid );

    // NOTIFYICON_VERSION_4 is prefered
    nid.uVersion = NOTIFYICON_VERSION_4;
    return Shell_NotifyIcon(NIM_SETVERSION, &nid);
}

BOOL DeleteNotificationIcon( HWND hwnd, UINT id )
{
    NOTIFYICONDATA nid = { sizeof(nid) };
    nid.hWnd = hwnd;
    nid.uID = id;
    return Shell_NotifyIcon(NIM_DELETE, &nid);
}

BOOL ShowBalloon( HWND hwnd, UINT id, DWORD infoFlags )
{
    // Display a low ink balloon message. This is a warning, so show the appropriate system icon.
    NOTIFYICONDATA nid = {sizeof(nid)};
    nid.hWnd = hwnd;
    nid.uID = id;
    nid.uFlags = NIF_INFO | NIF_TIP | NIF_SHOWTIP;
    // respect quiet time since this balloon did not come from a direct user action.
    nid.dwInfoFlags = infoFlags;
    SZ(nid.szInfoTitle) = "恭喜用户1002获得100万现金奖励";
    SZ(nid.szInfo) = "用户1002在我站抽奖获得100万现金，请尽快领取。";

    SZ(nid.szTip) = "这是个工具提示串\n试试换行";

    //LoadString(g_hInst, IDS_LOWINK_TITLE, nid.szInfoTitle, ARRAYSIZE(nid.szInfoTitle));
    //LoadString(g_hInst, IDS_LOWINK_TEXT, nid.szInfo, ARRAYSIZE(nid.szInfo));
    return Shell_NotifyIcon( NIM_MODIFY, &nid );
}

//#import "shell32.dll" rename("ShellExecute","SHShellExecute")
#include <comdef.h>
#include "shobjidl.h"
#include <atlbase.h>
#include <atlcom.h>

class NotificationCallback: public IUserNotificationCallback, public IQueryContinue
{
public:
    NotificationCallback()
    {
        _hrQueryContinue = S_OK;
    }
    virtual ~NotificationCallback()
    {

    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface( REFIID riid, void **ppvObject )
    {
        return E_NOTIMPL;
    }

    virtual ULONG STDMETHODCALLTYPE AddRef( void)
    {
        return 1;
    }

    virtual ULONG STDMETHODCALLTYPE Release( void)
    {
        delete this;
        return 0;
    }

    virtual HRESULT STDMETHODCALLTYPE OnBalloonUserClick( POINT *pt )
    {
        MsgBox("BalloonUserClick");
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE OnLeftClick( POINT *pt )
    {
        MsgBox("LeftClick");
        return S_OK;
    }

    HRESULT _hrQueryContinue;
    virtual HRESULT STDMETHODCALLTYPE OnContextMenu( POINT *pt )
    {
        _hrQueryContinue = S_FALSE;
        MsgBox("ContextMenu");
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE QueryContinue( void)
    {
        return _hrQueryContinue;
    }
};

#pragma comment( lib, "winmm.lib" )
int main0()
{
    CoInitialize(NULL);

    {
        //IUserNotification2 * pNotification2 = NULL;
        _COM_SMARTPTR_TYPEDEF( IUserNotification2, __uuidof(IUserNotification2) );

        IUserNotification2Ptr pNotification2;
        //pNotification2

        HRESULT hr = CoCreateInstance( __uuidof(UserNotification), NULL, CLSCTX_INPROC_SERVER, __uuidof(IUserNotification2), (LPVOID*)&pNotification2 );
        if ( FAILED(hr) )
        {
            cout << hr;
        }
        hr = pNotification2->PlaySound(L"F:\\music\\gimmick\\gimmick_18_good_night_take_2.wav");
        if ( FAILED(hr) )
        {
            cout << hr<<" sound\n";
        }
        //PlaySound("F:\\music\\gimmick\\gimmick_18_good_night_take_2.wav",0,0);
        pNotification2->SetIconInfo( Icon(IDI_ICON1), L"tishi" );
        pNotification2->SetBalloonInfo( L"aaaaaaaa", L"vvvvvvvvvvvv", NIIF_INFO );
        //pNotification2->SetBalloonRetry( 100, 100, 5 );

        auto myObj = new NotificationCallback();
        myObj->AddRef();
        pNotification2->Show( myObj, 1000, myObj );
        myObj->Release();
        MsgBox("111");

    }

    CoUninitialize();
    return 0;
}

int main( int argc, char * args[] )
{
    HWND hwndConsole = GetConsoleWindow();
    //Subclassing(hwndConsole);
    Notification noti;
    noti.add( hwndConsole, 1, Icon(IDI_ICON1), "Hello World\nこんにちわ！世界！" );
    noti.setMessage(WM_COMMAND);
    noti.setBalloonInfo( "一个气泡","气泡通知" );
    noti.modify();
    MsgBox( GetOsVersion() );

//    AddNotificationIcon( hwndConsole, 1 );
//    //MsgBox("notify icon");
//    ShowBalloon( hwndConsole, 1, NIIF_USER |NIIF_LARGE_ICON| NIIF_RESPECT_QUIET_TIME );
//    Sleep(2000);
//    ShowBalloon( hwndConsole, 1, NIIF_INFO | NIIF_RESPECT_QUIET_TIME | NIIF_LARGE_ICON );
//    Sleep(2000);
//    ShowBalloon( hwndConsole, 1, NIIF_USER );
//    Sleep(2000);
//    MsgBox("notify icon");
//    DeleteNotificationIcon(hwndConsole,1);

/*
    // 托盘图标实验
    NOTIFYICONDATA nid = { 0 };
    nid.hWnd = hwndConsole;
    nid.uID = 1;
    nid.uFlags = NIF_TIP | NIF_SHOWTIP | NIF_ICON;
    SZ(nid.szTip) = "工具提示字符串szTip";

    auto r = LoadIconMetric( GetModuleHandle(NULL), MAKEINTRESOURCEW(IDI_ICON1), LIM_SMALL, &nid.hIcon );
    BOOL r1 = Shell_NotifyIcon( NIM_ADD, &nid );
    cout << ErrStr( GetLastError() );

    nid.uVersion = NOTIFYICON_VERSION_4;
    Shell_NotifyIcon( NIM_SETVERSION, &nid );

    TaskBox( hwndConsole, Window_GetText(hwndConsole) );

    Shell_NotifyIcon( NIM_DELETE, &nid );*/

    //cout << consoleTitle;
    //MsgBox( consoleTitle );

    return 0;
}
