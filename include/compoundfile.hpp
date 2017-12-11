//////////////////////////////////////////////////////////////////////////
// purpose: �����ļ����
// author:  WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_COMPOUNDFILE_HPP__)
#define __WINPLUS_COMPOUNDFILE_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

// IStream ��� -----------------------------------------------------------

/* �Ӵ��������ļ����ڴ���,���õ�IStream�ӿ�ָ�� */
IStreamPtr CreateStreamExistingFile( String const & fileName );
/* ��ָ���������½�һ��IStream */
IStreamPtr CreateStreamFromBuffer( LPCVOID buffer, DWORD size );

/* ����Դ����һ��IStream */
IStreamPtr CreateStreamFromResourceEx( HMODULE module, UINT resourceId, LPCTSTR type );
IStreamPtr CreateStreamFromResource( UINT resourceId, LPCTSTR type );

} // namespace winplus

#endif // !defined(__WINPLUS_COMPOUNDFILE_HPP__)
