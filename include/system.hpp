//////////////////////////////////////////////////////////////////////////
// purpose: ϵͳ�������
// author:  WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_SYS_HPP__)
#define __WINPLUS_SYS_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

//////////////////////////////////////////////////
// һЩ����
//////////////////////////////////////////////////

/* Ŀ¼�ָ�� */
extern String const dirSep;// = TEXT("\\");
extern String const dirSepUnix;// = TEXT("/");
extern String const dirSepMac;// = TEXT("/");

/* windows�зָ�� */
extern String const lineSep;// = TEXT("\r\n");
/* unix�зָ�� */
extern String const lineSepUnix;// = TEXT("\n");
/* mac�зָ�� */
extern String const lineSepMac;// = TEXT("\r");

/* �ж�һ���ַ����Ƿ��չ�������Ƿ����%varname% */
bool IsExpandString( String const & str );
/* չ���ַ����д��ڵ�'%����%' */
String ExpandVars( String const & str );

/* ͨ��HWND��ó���·���ͽ���ID */
String GetAppPathFromHWND( HWND hWnd, DWORD * processId = NULL );
/* ���ص�ǰ����Ŀ¼(ĩβ����Ŀ¼�ָ���) */
String GetCurrentDir( void );
/* ����ģ��·��(ĩβ����Ŀ¼�ָ���)�����ģ���ļ��� */
String ModulePath( HMODULE module = NULL, String * fileName = NULL );
/* ��ȡ·����(ĩβ����Ŀ¼�ָ���) */
String FilePath( String const & fullPath, String * fileName = NULL );
/* ��ȡһ���ļ����ı������չ�� */
String FileTitle( String const & fileName, String * extName = NULL );
/* �ж��Ƿ���һ��Ŀ¼ */
bool IsDir( String const & path );
/* ��ȡ�ļ���С */
uint64 FileSize( String const & fullpath );

/* ·���ָ������� */
String PathWithSep( String const & path );
String PathNoSep( String const & path );

/* ��ó���������в��� */
INT CommandArguments( StringArray * arr );
/* ʹ�ܹػ� */
bool ShutdownPrivilege( bool enable );

/* �������NT�汾���ַ��� */
String GetNtVersion( void );
/* �������ϵͳ�汾���ַ��� */
String GetOsVersion( void );
/* �������ģ��汾�� */
String GetSelfModuleVersion( void );
/* ���ģ��汾�� */
String GetModuleVersion( String const & moduleFile );


// �ļ���������

////////////////////////////////////////////////
// ��ȡ�ļ����е��ļ������ļ���
// sort_type:
// 0 - ������, 1 - ����, 2 - ����
////////////////////////////////////////////////
void FolderData( String const & path, StringArray * files, StringArray * sub_folders, int sort_type = 0 );
////////////////////////////////////////////////
// ��ָ��·����ö��ָ����չ�����ļ�
// files: ���յ��ļ�������·������
// is_recursion: �Ƿ�ݹ�����
////////////////////////////////////////////////
int EnumFiles( String const & path, StringArray const & extnames, StringArray * files, bool is_recursion = true );

//////////////////////////////////////////////////////////////////////////

/* �����ڴ�(����FileMapping)
   �����ڴ�����Windows�ں˶���,�ʿ��Կ���̷���
   �����ڽ��̼�ͨѶ */
template < typename _Ty > class SharedMemory
{
public:
    SharedMemory() : _data(NULL), _hShared(NULL) {}
    SharedMemory( String const & name, int size )
    : _data(NULL), _hShared(NULL)
    {
        this->create( name, size );
    }
    ~SharedMemory()
    {
        this->free();
    }


    bool create( String const & name, int size )
    {
        this->free();
        _hShared = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name.c_str() );
        return _hShared != NULL;
    }
    bool open( String const & name )
    {
        this->free();
        _hShared = OpenFileMapping( FILE_MAP_ALL_ACCESS, FALSE, name.c_str() );
        return _hShared != NULL;
    }
    void free()
    {
        this->unlock();
        if ( _hShared != NULL )
        {
            CloseHandle(_hShared);
            _hShared = NULL;
        }
    }

    _Ty * lock()
    {
        this->unlock();
        return _data = reinterpret_cast<_Ty *>( MapViewOfFile( _hShared, FILE_MAP_ALL_ACCESS, 0, 0, 0 ) );
    }

    _Ty * operator -> ()
    {
        return _data ? _data : lock();
    }

    void unlock()
    {
        if ( _data != NULL )
        {
            UnmapViewOfFile(_data);
            _data = NULL;
        }
    }

private:
    _Ty * _data;
    HANDLE _hShared;

    SharedMemory( SharedMemory const & );
    SharedMemory & operator = ( SharedMemory const & );
};

/* ��������
   ���ڴ�����������ȡָ������ */
class CommandLine
{
    StringArray _args;
    String _paramPrefix;
    // �������Ƚ�
    bool compareEqv( String const & name, String const & arg ) const;
    // �Ƿ��ǲ���
    bool isParamName( String const & arg ) const;
public:
    CommandLine( String const & paramPrefix = TEXT("-/") );
    String getParam( String const & name, String const & defValue = TEXT("") ) const;
    int getParamInt( String const & name, int defValue = 0 ) const;
    bool include( String const & name, int * valueIndex = NULL ) const;
    bool includeValue( String const & value ) const;
};

/* DLL��̬������ */
class DllLoader
{
public:
    DllLoader( LPCTSTR dllName ) : _hDll(NULL)
    {
        _hDll = LoadLibrary(dllName);
    }
    ~DllLoader()
    {
        if ( _hDll )
        {
            FreeLibrary(_hDll);
            _hDll = NULL;
        }
    }
    operator bool() const { return _hDll != NULL; }
    operator HMODULE() const { return _hDll; }

private:
    HMODULE _hDll;

    DllLoader( DllLoader const & );
    DllLoader & operator = ( DllLoader const & );
};

/* Dll������̬���� */
template < typename _PfnType >
class DllFunction
{
    _PfnType _pfn;
public:
    DllFunction( HMODULE module, LPCSTR procName )
    {
        _pfn = (_PfnType)GetProcAddress( module, procName );
    }

    operator bool() const { return _pfn != NULL; }
    operator _PfnType() const { return _pfn; }
};

} // namespace winplus

#endif // !defined(__WINPLUS_SYS_HPP__)
