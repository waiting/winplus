//////////////////////////////////////////////////////////////////////////
// purpose:  ���ڿؼ����
// author:   WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_WINCTRL_HPP__)
#define __WINPLUS_WINCTRL_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{
// UI��� ----------------------------------------------------------------

/* ���ڶ�ʱ�� */
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

// ������� --------------------------------------------------------------

/* ����һ����Ϣ�� */
int MsgBox( String const & msg, String const & title = TEXT("WinPlus") );
/* ����һ������� */
int ErrBox( String const & msg, String const & title = TEXT("WinPlus Error") );
/* ����һ������� */
int WarnBox( String const & msg, String const & title = TEXT("WinPlus Warning") );

/* ��ȡ�ͻ������� */
RECT Window_GetClient( HWND window );
/* ��ȡ���ھ���,����ڸ����� */
RECT Window_GetRect( HWND window );
/* ���ô��ھ���,����ڸ����� */
void Window_SetRect( HWND window, LPCRECT rect, bool is_redraw = true );
/* ���ʹ��� */
void Window_Inflate( HWND window, INT dx, INT dy );
/* ����Դ���Ϊ׼���д��� */
void Window_Center( HWND window, HWND relative_wnd, bool is_in_relative = false, bool is_redraw = true );
/* �����ı� */
String Window_GetText( HWND window );
void Window_SetText( HWND window, String const & text );
/* ��ô����Ƿ���ʾ */
bool Window_IsShow( HWND window );
/* �ô��ڿɼ� */
void Window_Show( HWND window, bool show = true );

// ListCtrl - Report���� -------------------------------------------------
INT Report_AddStrings( HWND report, StringArray const & cols );
INT Report_InsertStrings( HWND report, INT item_index, StringArray const & cols );
void Report_SetStrings( HWND report, INT item_index, StringArray const & cols );
INT Report_GetStrings( HWND report, INT item_index, StringArray * cols );

// ImageList control��� -------------------------------------------------
#ifdef _GDIPLUS_H
/* ���һ�ź�Alphaͨ����ͼƬ��ImageList via Gdiplus::Bitmap,ImageList���봴��ΪILC_COLOR32 */
int ImageList_AddAlphaImage( HIMAGELIST hImageList, Gdiplus::Bitmap const & image );
int ImageList_AddAlphaImage( HIMAGELIST hImageList, UINT nId, LPCTSTR lpType );
int ImageList_AddAlphaImage( HIMAGELIST hImageList, String const & imagePath );
#endif
/* ���һ�ź�Alphaͨ����32λDIB��ImageList via LoadImage(),ImageList���봴��ΪILC_COLOR32 */
int ImageList_Add32bpp( HIMAGELIST hImageList, UINT nId, UINT uType );
int ImageList_Add32bpp( HIMAGELIST hImageList, String const & imagePath, UINT uType );

// HotkeyCtrl��� --------------------------------------------------------

/* HOTKEYF��ʶתΪMOD��ʶ
   ���ڴ�Hotkey�ؼ�����İ�����Ϣ��RegisterHotkey()ע�� */
UINT HOTKEYF_to_MOD( UINT hkf );

/* MOD��ʶתΪHOTKEYF��ʶ
   ��HOTKEYF_to_MOD()�෴ */
UINT MOD_to_HOTKEYF( UINT mod );


// ͨ�öԻ������ --------------------------------------------------------

/* �ļ���ѡ��Ի���
   ����SHBrowseForFolder() */
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
	// �ص�����
	static int CALLBACK BrowseCallback( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData );
public:
	FolderDialog( HWND hWndOwner, LPCTSTR lpszWindowTitle = NULL, LPCTSTR lpszPromptText = NULL, BOOL bIncludedFiles = FALSE );
	// �ɹ���ȡ·������TRUE,���򷵻�FALSE
	BOOL doModal( LPCTSTR lpszRootDir = NULL, LPCTSTR lpszSelectionDir = NULL );	// ��ʾ�Ի���
	String getDisplayName() const; // ѡ����ļ�����ʾ��(��ʾ������һ�������ļ�����:����"�ҵ��ĵ�" and "My Documents")
	String getPath() const; // ѡ���·��
};

/* �ļ�ѡ��Ի��� */
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
	String getFilePath() const; // ѡ����ļ�·��
	String getDirPath() const; // ��ȡĿ¼·��
	int getFilePaths( StringArray * pArr ) const; // ����ѡʱͨ���˻��ѡ����ļ�

};

} // namespace winplus

#endif // !defined(__WINPLUS_WINCTRL_HPP__)
