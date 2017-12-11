//////////////////////////////////////////////////////////////////////////
// purpose:  窗口控件相关
// author:   WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_WINCTRL_HPP__)
#define __WINPLUS_WINCTRL_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{
// UI相关 ----------------------------------------------------------------

/* 窗口定时器 */
class WindowTimer
{
public:
	WindowTimer( void );
	void create( HWND window, UINT elapse, UINT_PTR id = -1 );
	void destroy( void );
	UINT getId( void ) const { return _id; }
	~WindowTimer( void );
private:
	HWND _window;
	UINT_PTR _id;
	static UINT_PTR _idAutoIncrement;

	WindowTimer( WindowTimer const & );
	WindowTimer & operator = ( WindowTimer const & );
};

// 窗口相关 --------------------------------------------------------------

/* 弹出一个消息框 */
int MsgBox( String const & msg, String const & title = TEXT("WinPlus") );
/* 弹出一个错误框 */
int ErrBox( String const & msg, String const & title = TEXT("WinPlus Error") );
/* 弹出一个警告框 */
int WarnBox( String const & msg, String const & title = TEXT("WinPlus Warning") );

/* 获取客户区矩形 */
RECT Window_GetClient( HWND window );
/* 获取窗口矩形,相对于父窗口 */
RECT Window_GetRect( HWND window );
/* 设置窗口矩形,相对于父窗口 */
void Window_SetRect( HWND window, LPCRECT rect, bool is_redraw = true );
/* 膨胀窗口 */
void Window_Inflate( HWND window, INT dx, INT dy );
/* 以相对窗口为准居中窗口 */
void Window_Center( HWND window, HWND relative_wnd, bool is_in_relative = false, bool is_redraw = true );
/* 窗口文本 */
String Window_GetText( HWND window );
void Window_SetText( HWND window, String const & text );
/* 获得窗口是否显示 */
bool Window_IsShow( HWND window );
/* 让窗口可见 */
void Window_Show( HWND window, bool show = true );

// ListCtrl - Report操作 -------------------------------------------------
INT Report_AddStrings( HWND report, StringArray const & cols );
INT Report_InsertStrings( HWND report, INT item_index, StringArray const & cols );
void Report_SetStrings( HWND report, INT item_index, StringArray const & cols );
INT Report_GetStrings( HWND report, INT item_index, StringArray * cols );

// ImageList control相关 -------------------------------------------------
#ifdef _GDIPLUS_H
/* 添加一张含Alpha通道的图片进ImageList via Gdiplus::Bitmap,ImageList必须创建为ILC_COLOR32 */
int ImageList_AddAlphaImage( HIMAGELIST hImageList, Gdiplus::Bitmap const & image );
int ImageList_AddAlphaImage( HIMAGELIST hImageList, UINT nId, LPCTSTR lpType );
int ImageList_AddAlphaImage( HIMAGELIST hImageList, String const & imagePath );
#endif
/* 添加一张含Alpha通道的32位DIB进ImageList via LoadImage(),ImageList必须创建为ILC_COLOR32 */
int ImageList_Add32bpp( HIMAGELIST hImageList, UINT nId, UINT uType );
int ImageList_Add32bpp( HIMAGELIST hImageList, String const & imagePath, UINT uType );

// HotkeyCtrl相关 --------------------------------------------------------

/* HOTKEYF标识转为MOD标识
   便于从Hotkey控件输入的按键信息用RegisterHotkey()注册 */
UINT HOTKEYF_to_MOD( UINT hkf );

/* MOD标识转为HOTKEYF标识
   与HOTKEYF_to_MOD()相反 */
UINT MOD_to_HOTKEYF( UINT mod );


// 通用对话框相关 --------------------------------------------------------

/* 文件夹选择对话框
   基于SHBrowseForFolder() */
class FolderDialog
{
private:
	FolderDialog( FolderDialog const & );
	FolderDialog & operator = ( FolderDialog const & );

	BROWSEINFO _bi;
	String _strPath;
	String _strDisplay;
	LPCTSTR _lpszWindowTitle;
	LPCTSTR _lpszSelectionDir;
	// 回调函数
	static int CALLBACK BrowseCallback( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData );
public:
	FolderDialog( HWND hWndOwner, LPCTSTR lpszWindowTitle = NULL, LPCTSTR lpszPromptText = NULL, BOOL bIncludedFiles = FALSE );
	// 成功获取路径返回TRUE,否则返回FALSE
	BOOL doModal( LPCTSTR lpszRootDir = NULL, LPCTSTR lpszSelectionDir = NULL );	// 显示对话框
	String getDisplayName() const; // 选择的文件夹显示名(显示名并不一定就是文件夹名:例如"我的文档" and "My Documents")
	String getPath() const; // 选择的路径
};

/* 文件选择对话框 */
class FileDialog
{
private:
	FileDialog( FileDialog const & );
	FileDialog & operator = ( FileDialog const & );

	String _buffer;
	OPENFILENAME _ofn;
	BOOL _isOpen;
public:
	FileDialog( HWND hwndOwner, BOOL bOpenDialog = TRUE, LPCTSTR lpszWindowTitle = NULL, LPCTSTR lpszDefExt = NULL, BOOL bMultiSelected = FALSE, DWORD dwAdditionalFlags = 0, DWORD dwRemoveFlags = 0 );
	BOOL doModal( LPCTSTR lpszInitialDir = NULL, LPCTSTR lpFilter = NULL );
	String getFilePath() const; // 选择的文件路径
	String getDirPath() const; // 获取目录路径
	int getFilePaths( StringArray * pArr ) const; // 当多选时通过此获得选择的文件

};

} // namespace winplus

#endif // !defined(__WINPLUS_WINCTRL_HPP__)
