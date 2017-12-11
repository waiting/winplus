#include <windows.h>
#include "../include/definitions.hpp"
#include "../include/winctrl.hpp"
#include "../include/shell.hpp"
#include "../include/strings.hpp"
#include "../include/system.hpp"
#include "../include/compoundfile.hpp"
#include <commctrl.h>

#pragma comment( lib, "comctl32.lib" )

namespace winplus
{

UINT_PTR WindowTimer::_idAutoIncrement = 0;
WindowTimer::WindowTimer( void ) : _window(NULL), _id(0)
{
}

WindowTimer::~WindowTimer( void )
{
	destroy();
}

void WindowTimer::create( HWND window, UINT elapse, UINT_PTR id )
{
	destroy();
	_window = window;
	if ( _window )
	{
		if ( id == -1 )
		{
			_id = ++_idAutoIncrement;
		}
		else
		{
			_id = id;
			if ( _idAutoIncrement < id )
				_idAutoIncrement = id;
		}
		SetTimer( _window, _id, elapse, NULL );
	}
}

void WindowTimer::destroy( void )
{
	if ( _window )
	{
		KillTimer( _window, _id );
		_window = NULL;
	}
}

// ������� ---------------------------------------------------------------
int MsgBox( String const & msg, String const & title )
{
	return MessageBox( GetForegroundWindow(), msg.c_str(), title.c_str(), MB_ICONINFORMATION );
}

int ErrBox( String const & msg, String const & title )
{
	return MessageBox( GetForegroundWindow(), msg.c_str(), title.c_str(), MB_ICONERROR );
}

int WarnBox( String const & msg, String const & title )
{
	return MessageBox( GetForegroundWindow(), msg.c_str(), title.c_str(), MB_ICONEXCLAMATION );
}

RECT Window_GetClient( HWND window )
{
	RECT rc;
	GetClientRect( window, &rc );
	return rc;
}

RECT Window_GetRect( HWND window )
{
	RECT rc;
	GetWindowRect( window, &rc );
	HWND parent;
	parent = GetParent(window);
	if ( IsWindow(parent) ) // �жϸ������Ƿ���ڣ�����Ļ���껻Ϊ��Ը���������
	{
		ScreenToClient( parent, (LPPOINT)&rc );
		ScreenToClient( parent, (LPPOINT)&rc + 1 );
	}
	return rc;
}

void Window_SetRect( HWND window, LPCRECT rect, bool is_redraw /*= true */ )
{
	SetWindowPos( window, NULL, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SWP_NOZORDER | ( is_redraw ? 0 : SWP_NOREDRAW ) );
}

void Window_Inflate( HWND window, INT dx, INT dy )
{
	RECT rc = Window_GetRect(window);
	InflateRect( &rc, dx, dy );
	Window_SetRect( window, &rc );
}

void Window_Center( HWND window, HWND relative_wnd, bool is_in_relative /*= false*/, bool is_redraw /*= true */ )
{
	INT cx, cy, offsetLeft, offsetTop;
	if ( relative_wnd && IsWindow(relative_wnd) )
	{
		if ( is_in_relative )
		{
			RECT rcWnd;
			GetClientRect( relative_wnd, &rcWnd );
			cx = rcWnd.right - rcWnd.left;
			cy = rcWnd.bottom - rcWnd.top;
			offsetLeft = 0;//rcWnd.left;
			offsetTop = 0;//rcWnd.top;
		}
		else
		{
			RECT rcWnd;
			GetWindowRect( relative_wnd, &rcWnd );
			cx = rcWnd.right - rcWnd.left;
			cy = rcWnd.bottom - rcWnd.top;
			offsetLeft = rcWnd.left;
			offsetTop = rcWnd.top;
		}
	}
	else // use screen coord
	{
		cx = GetSystemMetrics(SM_CXSCREEN);
		cy = GetSystemMetrics(SM_CYSCREEN);
		offsetLeft = 0;
		offsetTop = 0;
	}
	RECT rc;
	GetWindowRect( window, &rc );
	LONG nWidth = rc.right - rc.left;
	LONG nHeight = rc.bottom - rc.top;
	INT left, top;
	left = offsetLeft + ( cx - nWidth ) / 2;
	top = offsetTop + ( cy - nHeight ) / 2;
	SetWindowPos( window, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | ( is_redraw ? 0 : SWP_NOREDRAW ) );
}

String Window_GetText( HWND window )
{
	String res;
	INT len = GetWindowTextLength(window);
	res.resize(len);
	GetWindowText( window, &res[0], len + 1 );
	return res.c_str();
}

void Window_SetText( HWND window, String const & text )
{
	SetWindowText( window, text.c_str() );
}

bool Window_IsShow( HWND window )
{
	return ( GetWindowLong( window, GWL_STYLE ) & WS_VISIBLE ) != 0;
}

void Window_Show( HWND window, bool show )
{
	LONG style = GetWindowLong( window, GWL_STYLE );
	if ( show )
	{
		//SetWindowLong( window, GWL_STYLE, style | WS_VISIBLE );
		ShowWindow( window, SW_NORMAL );
	}
	else
	{
		//SetWindowLong( window, GWL_STYLE, style & ~WS_VISIBLE );
		ShowWindow( window, SW_HIDE );
	}
}


// ListCtrl - Report����---------------------------------------------------
INT Report_AddStrings( HWND report, StringArray const & cols )
{
	INT itemsCount = ListView_GetItemCount(report);
	INT realColsCount = Header_GetItemCount( ListView_GetHeader(report) );
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = itemsCount;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)NULL;
	item.state = 0;
	item.stateMask = 0;
	item.iImage = 0;
	item.lParam = 0;

	INT index = ListView_InsertItem( report, &item );
	INT i, colsCount = cols.size();

	for ( i = 0; i < colsCount && i < realColsCount; i++ )
	{
		ListView_SetItemText( report, itemsCount, i, (LPTSTR)cols[i].c_str() );
	}

	return index;
}

INT Report_InsertStrings( HWND report, INT item_index, StringArray const & cols )
{
	INT realColsCount = Header_GetItemCount( ListView_GetHeader(report) );
	LVITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = item_index;
	item.iSubItem = 0;
	item.pszText = (LPTSTR)NULL;
	item.state = 0;
	item.stateMask = 0;
	item.iImage = 0;
	item.lParam = 0;

	INT index = ListView_InsertItem( report, &item );
	INT i, colsCount = cols.size();
	
	for ( i = 0; i < colsCount && i < realColsCount; i++ )
	{
		ListView_SetItemText( report, item_index, i, (LPTSTR)cols[i].c_str() );
	}
	
	return index;
}

void Report_SetStrings( HWND report, INT item_index, StringArray const & cols )
{
	INT realColsCount = Header_GetItemCount( ListView_GetHeader(report) );
	INT i, colsCount = cols.size();

	for ( i = 0; i < colsCount && i < realColsCount; i++ )
	{
		ListView_SetItemText( report, item_index, i, (LPTSTR)cols[i].c_str() );
	}
}

INT Report_GetStrings( HWND report, INT item_index, StringArray * cols )
{
	INT real_cols_count = Header_GetItemCount( ListView_GetHeader(report) );
	INT i;
	for ( i = 0; i < real_cols_count; ++i )
	{
		LVITEM item;
		item.iItem = item_index;
		item.iSubItem = i;

		String text;
		INT len = 511;
		text.resize(len);
		item.cchTextMax = len + 1;
		item.pszText = &text[0];

		len = SendMessage( report, LVM_GETITEMTEXT, item_index, (LPARAM)&item );

		cols->push_back(item.pszText);
	}
	return i;
}

// ImageList control��� -------------------------------------------------
#ifdef _GDIPLUS_H
int ImageList_AddAlphaImage( HIMAGELIST hImageList, Gdiplus::Bitmap const & image )
{
	using namespace Gdiplus;
	Bitmap & bm = const_cast<Bitmap &>(image);
	// ת��32λbpp
	HBITMAP hBitmap;
	void * pvBits;
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biWidth = bm.GetWidth();
	bmi.bmiHeader.biHeight = bm.GetHeight();
	bm.RotateFlip( RotateNoneFlipY );
	hBitmap = CreateDIBSection( NULL, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0 );
	BitmapData bitmapData;
	bm.LockBits( &Rect( 0, 0, bm.GetWidth(), bm.GetHeight() ), ImageLockModeRead, PixelFormat32bppARGB, &bitmapData );
	RtlMoveMemory( pvBits, bitmapData.Scan0, bm.GetHeight() * bitmapData.Stride );
	bm.UnlockBits(&bitmapData);
	int i;
	i = ImageList_Add( hImageList, hBitmap, NULL );
	DeleteObject(hBitmap);
	return i;
}

int ImageList_AddAlphaImage( HIMAGELIST hImageList, UINT nId, LPCTSTR lpType )
{
	IStreamPtr pImageStream = CreateStreamFromResource( nId, lpType );
	return ImageList_AddAlphaImage( hImageList, Gdiplus::Bitmap(pImageStream) );
}

int ImageList_AddAlphaImage( HIMAGELIST hImageList, String const & imagePath )
{
	return ImageList_AddAlphaImage( hImageList, Gdiplus::Bitmap( StringToUnicode(imagePath).c_str() ) );
}
#endif

int ImageList_Add32bpp( HIMAGELIST hImageList, UINT nId, UINT uType )
{
	HBITMAP hBitmap = (HBITMAP)LoadImage( GetModuleHandle(NULL), MAKEINTRESOURCE(nId), uType, 0, 0, LR_CREATEDIBSECTION );
	int i = ImageList_Add( hImageList, hBitmap, NULL );
	DeleteObject(hBitmap);
	return i;
}

int ImageList_Add32bpp( HIMAGELIST hImageList, String const & imagePath, UINT uType )
{
	HBITMAP hBitmap = (HBITMAP)LoadImage( GetModuleHandle(NULL), imagePath.c_str(), uType, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION );
	int i = ImageList_Add( hImageList, hBitmap, NULL );
	DeleteObject(hBitmap);
	return i;
}

// HotkeyCtrl��� --------------------------------------------------------

UINT HOTKEYF_to_MOD( UINT hkf )
{
	UINT mod = 0;
	UINT hkfs[3] = {HOTKEYF_SHIFT, HOTKEYF_CONTROL, HOTKEYF_ALT};
	UINT mods[3] = {MOD_SHIFT, MOD_CONTROL, MOD_ALT};
	int i;
	for (i = 0; i < 3; i++)
		(hkf & hkfs[i]) && (mod |= mods[i]);
	return mod;
}

UINT MOD_to_HOTKEYF( UINT mod )
{
	UINT hkf = 0;
	UINT hkfs[3] = {HOTKEYF_SHIFT, HOTKEYF_CONTROL, HOTKEYF_ALT};
	UINT mods[3] = {MOD_SHIFT, MOD_CONTROL, MOD_ALT};
	int i;
	for (i = 0; i < 3; i++)
		(mod & mods[i]) && (hkf |= hkfs[i]);
	return hkf;
}

// ͨ�öԻ������ --------------------------------------------------------

FolderDialog::FolderDialog( HWND hWndOwner, LPCTSTR lpszWindowTitle, LPCTSTR lpszPromptText, BOOL bIncludedFiles )
{
	ZeroMemory( &_bi, sizeof(_bi) );
	_strPath.resize(MAX_PATH);
	_strDisplay.resize(MAX_PATH);
	
	_bi.pszDisplayName = &_strDisplay[0];
	_bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI | BIF_STATUSTEXT | BIF_UAHINT | BIF_NEWDIALOGSTYLE;
	if ( bIncludedFiles )
	{
		_bi.ulFlags |= BIF_BROWSEINCLUDEFILES;
	}

	_bi.hwndOwner = hWndOwner;
	if ( lpszPromptText != NULL )
		_bi.lpszTitle = lpszPromptText;
	else
		_bi.lpszTitle = TEXT("ѡ���ļ���");

	_lpszSelectionDir = NULL;
	_lpszWindowTitle = lpszWindowTitle;

	_bi.lpfn = BrowseCallback;
	_bi.lParam = (LPARAM)this;
}

BOOL FolderDialog::doModal( LPCTSTR lpszRootDir, LPCTSTR lpszSelectionDir )
{
	LPMALLOC pMalloc;
	LPITEMIDLIST pItemIDList;
	HRESULT hr;
	BOOL bRet = FALSE;

	if ( lpszRootDir != NULL && *lpszRootDir )
		_bi.pidlRoot = ItemIdListFromPath(lpszRootDir);
	else
		_bi.pidlRoot = NULL;
	
	_lpszSelectionDir = lpszSelectionDir;

	pItemIDList = SHBrowseForFolder(&_bi);

	hr = SHGetMalloc(&pMalloc);
	
	if ( pItemIDList != NULL )
	{
		SHGetPathFromIDList( pItemIDList, &_strPath[0] );

		if ( pMalloc != NULL )
			pMalloc->Free(pItemIDList);

		bRet = TRUE;
	}
	
	if ( pMalloc != NULL && _bi.pidlRoot != NULL )
	{
		pMalloc->Free( (void *)_bi.pidlRoot );
		_bi.pidlRoot = NULL;
	}
	
	if ( pMalloc != NULL )
		pMalloc->Release();
	
	return bRet;
}

String FolderDialog::getPath() const
{
	return _strPath.c_str();
}

String FolderDialog::getDisplayName() const
{
	return _strDisplay.c_str();
}

int CALLBACK FolderDialog::BrowseCallback( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
	switch ( uMsg )
	{
	case BFFM_INITIALIZED:
		{
			//SendMessage( hwnd, BFFM_SETOKTEXT, 0, (LPARAM)L"Helloworld" );
			//SendMessage( hwnd, BFFM_SETSTATUSTEXT, 0, (LPARAM)L"Helloworld" );
			FolderDialog * pDlg = reinterpret_cast<FolderDialog *>(lpData);
			// ���öԻ������
			if ( pDlg->_lpszWindowTitle )
			{
				SetWindowText( hwnd, pDlg->_lpszWindowTitle );
			}
			// ���ñ�ѡ��Ŀ¼
			if ( pDlg->_lpszSelectionDir )
			{
				SendMessage( hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pDlg->_lpszSelectionDir );
			}
			SendMessage( hwnd, WM_SETICON, ICON_BIG, SendMessage( pDlg->_bi.hwndOwner, WM_GETICON, ICON_BIG, 0 ) );
		}
		break;
	case BFFM_SELCHANGED:
		break;
	case BFFM_VALIDATEFAILED:
		break;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

FileDialog::FileDialog( HWND hwndOwner, BOOL bOpenDialog, LPCTSTR lpszWindowTitle, LPCTSTR lpszDefExt, BOOL bMultiSelected, DWORD dwAdditionalFlags, DWORD dwRemoveFlags )
{
	ZeroMemory( &_ofn, sizeof(_ofn) );
	_ofn.lStructSize = sizeof(_ofn);
	_ofn.hwndOwner = hwndOwner;
	_ofn.lpstrTitle = lpszWindowTitle;
	_ofn.lpstrDefExt = lpszDefExt;
	_isOpen = bOpenDialog;
	_ofn.Flags = OFN_HIDEREADONLY | OFN_EXPLORER | OFN_PATHMUSTEXIST | ( bOpenDialog ? OFN_FILEMUSTEXIST : OFN_OVERWRITEPROMPT );
	if ( bOpenDialog && bMultiSelected )
	{
		_ofn.Flags |= OFN_ALLOWMULTISELECT;
	}
	_ofn.Flags |= dwAdditionalFlags;
	_ofn.Flags &= ~dwRemoveFlags;

}

BOOL FileDialog::doModal( LPCTSTR lpszInitialDir, LPCTSTR lpFilter )
{
	_ofn.lpstrInitialDir = lpszInitialDir;
	_ofn.lpstrFilter = lpFilter;
	if ( _ofn.Flags & OFN_ALLOWMULTISELECT )
	{
		// �ɶ�ѡ�������,��Ҫ����Ŀռ����洢·��
		_buffer.resize(0x20000);
	}
	else
	{
		_buffer.resize(MAX_PATH);
	}

	_ofn.lpstrFile = &_buffer[0];
	_ofn.nMaxFile = _buffer.size();

	return _isOpen ? GetOpenFileName(&_ofn) : GetSaveFileName(&_ofn);
}

String FileDialog::getDirPath() const
{
	return _buffer.substr( 0, _ofn.nFileOffset - 1 );
}

String FileDialog::getFilePath() const
{
	String dirpath = getDirPath();
	int offset = _ofn.nFileOffset;
	if ( offset < _buffer.size() && _ofn.lpstrFile[offset] )
	{
		String fileName = _ofn.lpstrFile + offset;
		offset += fileName.length();
		offset += 1;
		return dirpath + dirSep + fileName;
	}
	return _buffer.c_str();
}

int FileDialog::getFilePaths( StringArray * pArr ) const
{
	String dirpath = getDirPath();
	int offset = _ofn.nFileOffset;
	while ( offset < _buffer.size() && _ofn.lpstrFile[offset] )
	{
		String fileName = _ofn.lpstrFile + offset;
		offset += fileName.length();
		offset += 1;
		pArr->push_back( dirpath + dirSep + fileName );
	}
	return pArr->size();
}


} // namespace winplus