//////////////////////////////////////////////////////////////////////////
// purpose:  资源相关
// author:   WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_RESOURCE_HPP__)
#define __WINPLUS_RESOURCE_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{
/** \brief 加载模块资源 */
class WINPLUS_DLL Resource
{
public:
    Resource()
    {
        _construct();
    }
    Resource( HMODULE hModule, UINT nResourceId, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
    {
        _construct( hModule, MAKEINTRESOURCE(nResourceId), lpType, wLanguage );
    }
    Resource( HMODULE hModule, LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
    {
        _construct( hModule, lpszName, lpType, wLanguage );
    }
    Resource( UINT nResourceId, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
    {
        _construct( GetModuleHandle(NULL), MAKEINTRESOURCE(nResourceId), lpType, wLanguage );
    }
    Resource( LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
    {
        _construct( GetModuleHandle(NULL), lpszName, lpType, wLanguage );
    }
    ~Resource();

    bool load( HMODULE hModule, LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) );
    bool load( HMODULE hModule, UINT nResourceId, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
    {
        return this->load( hModule, MAKEINTRESOURCE(nResourceId), lpType, wLanguage );
    }
    bool load( LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
    {
        return this->load( GetModuleHandle(NULL), lpszName, lpType, wLanguage );
    }
    bool load( UINT nResourceId, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
    {
        return this->load( GetModuleHandle(NULL), nResourceId, lpType, wLanguage );
    }
    void free();

    /** \brief 拷贝到目标缓冲区，返回目标缓冲区与资源的大小差
     *
     *  \return 0:恰好相同，正数:目标缓冲区大小大于资源，负数:目标缓冲区大小小于资源 */
    ssize_t copyTo( LPVOID lpDestination, size_t uDestinationSize ) const;

    template < typename _Ty >
    std::remove_const_t<_Ty> const * getData() const { return (std::remove_const_t<_Ty> const *)_lpData; }
    LPCVOID getData() const { return _lpData; }
    size_t getSize() const { return _uSize; }

    operator bool() const { return _lpData != NULL; }
protected:
    void _construct();
    void _construct( HMODULE hModule, LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) );
private:
    //HRSRC _hRSRC;
    HGLOBAL _hResMemBlock;
    //HMODULE _hModule;
    size_t _uSize;
    LPCVOID _lpData;

    DISABLE_OBJECT_COPY(Resource)
};

} // namespace winplus

#endif // !defined(__WINPLUS_RESOURCE_HPP__)
