//////////////////////////////////////////////////////////////////////////
// purpose: 系统环境相关
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
// 一些常量
//////////////////////////////////////////////////

/* 目录分割符 */
extern WINPLUS_DLL String const dirSep;// = TEXT("\\");
extern WINPLUS_DLL String const dirSepUnix;// = TEXT("/");
extern WINPLUS_DLL String const dirSepMac;// = TEXT("/");

/* windows行分割符 */
extern WINPLUS_DLL String const lineSep;// = TEXT("\r\n");
/* unix行分割符 */
extern WINPLUS_DLL String const lineSepUnix;// = TEXT("\n");
/* mac行分割符 */
extern WINPLUS_DLL String const lineSepMac;// = TEXT("\r");

/** \brief 判断一个字符串是否可展开，即是否包含%varname% */
WINPLUS_FUNC_DECL(bool) IsExpandString( String const & str );
/** \brief 展开字符串中存在的'%变量%' */
WINPLUS_FUNC_DECL(String) ExpandVars( String const & str );

/** \brief 通过HWND获得程序路径和进程ID以及线程ID */
WINPLUS_FUNC_DECL(String) GetAppPathByHwnd( HWND hwnd, DWORD * pProcessId = NULL, DWORD * pThreadId = NULL );

/** \brief 通过进程ID获得主窗口HWND */
WINPLUS_FUNC_DECL(HWND) GetMainWindowByProcessId( DWORD dwProcessId );

/** \brief 通过线程ID获得主窗口HWND */
WINPLUS_FUNC_DECL(HWND) GetMainWindowByThreadId( DWORD dwThreadId );

/** \brief 返回模块路径(末尾不含目录分隔符)，输出模块文件名 */
WINPLUS_FUNC_DECL(String) ModulePath( HMODULE mod = NULL, String * fileName = NULL );

/** \brief 获得程序的命令行参数 */
WINPLUS_FUNC_DECL(UINT) CommandArgumentArray( StringArray * argArr );
/** \brief 构造一个const char*[]，以vector<const char*>返回
 *
 *  不要用返回的vector的元素个数作为命令行参数个数，因为它始终多一个nullptr元素
 *  \param argArr 由CommandArgumentArray()取得的命令行参数数组，并且请保持其有效，如果其被摧毁，那么本函数返回的参数列表也就无效了 */
WINPLUS_FUNC_DECL(std::vector<String::value_type const *>) CommandArgs( StringArray * argArr );
/** \brief 使能关机 */
WINPLUS_FUNC_DECL(bool) ShutdownPrivilege( bool enable );

/** \brief Windows错误号转成错误字符串 */
WINPLUS_FUNC_DECL(String) GetErrorStr( DWORD err );

/** \brief 获得描述NT版本的字符串 */
WINPLUS_FUNC_DECL(String) GetNtVersion( void );
/** \brief 获得描述系统版本的字符串 */
WINPLUS_FUNC_DECL(String) GetOsVersion( void );
/** \brief 获得自身模块版本号 */
WINPLUS_FUNC_DECL(String) GetSelfModuleVersion( void );
/** \brief 获得模块版本号 */
WINPLUS_FUNC_DECL(String) GetModuleVersion( String const & moduleFile );


//////////////////////////////////////////////////////////////////////////

/** \brief 共享内存(基于FileMapping)
 *
 *  共享内存属于Windows内核对象，故可以跨进程访问。常用于进程间通讯 */
template < typename _Ty > class SharedMemory
{
public:
    SharedMemory() : _data(NULL), _hShared(NULL) {}
    SharedMemory( String const & name, size_t size ) : _data(NULL), _hShared(NULL)
    {
        this->create( name, size );
    }
    SharedMemory( String const & name ) : _data(NULL), _hShared(NULL)
    {
        this->create(name);
    }

    ~SharedMemory()
    {
        this->free();
    }

    bool create( String const & name, size_t size )
    {
        this->free();
        union
        {
            struct
            {
                DWORD dwLow;
                DWORD dwHigh;
            };
            size_t size;
        } v;
        ZeroMemory( &v, sizeof(v) );
        _size = v.size = size;

        _hShared = CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, v.dwHigh, v.dwLow, name.c_str() );
        return _hShared != NULL;
    }

    bool create( String const & name )
    {
        return this->create( name, sizeof(_Ty) );
    }

    bool open( String const & name, size_t size = (size_t)-1 )
    {
        this->free();
        _size = size == (size_t)-1 ? sizeof(_Ty) : size;
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
            _size = 0;
        }
    }

    _Ty * lock()
    {
        this->unlock();
        return _data = reinterpret_cast<_Ty *>( MapViewOfFile( _hShared, FILE_MAP_ALL_ACCESS, 0, 0, 0 ) );
    }

    _Ty * operator -> ()
    {
        return _data ? _data : this->lock();
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
    size_t _size;
    HANDLE _hShared;

    SharedMemory( SharedMemory const & );
    SharedMemory & operator = ( SharedMemory const & );
};


} // namespace winplus

#endif // !defined(__WINPLUS_SYS_HPP__)
