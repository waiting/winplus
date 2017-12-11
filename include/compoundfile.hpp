//////////////////////////////////////////////////////////////////////////
// purpose: 复合文件相关
// author:  WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_COMPOUNDFILE_HPP__)
#define __WINPLUS_COMPOUNDFILE_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

// IStream 相关 -----------------------------------------------------------

/* 从磁盘载入文件到内存中,并得到IStream接口指针 */
IStreamPtr CreateStreamExistingFile( String const & fileName );
/* 从指定缓冲区新建一个IStream */
IStreamPtr CreateStreamFromBuffer( LPCVOID buffer, DWORD size );

/* 从资源创建一个IStream */
IStreamPtr CreateStreamFromResourceEx( HMODULE module, UINT resourceId, LPCTSTR type );
IStreamPtr CreateStreamFromResource( UINT resourceId, LPCTSTR type );

} // namespace winplus

#endif // !defined(__WINPLUS_COMPOUNDFILE_HPP__)
