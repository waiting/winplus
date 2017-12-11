//////////////////////////////////////////////////////////////////////////
// purpose: Shell���
// author:  WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_SHELL_HPP__)
#define __WINPLUS_SHELL_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

// ���(Shell)��� --------------------------------------------------------

/* ��һ��·�����ItemIDList,·���������
   �����߸�����SHGetMalloc(),IMalloc::Free()�ͷŷ��ص�ItemIDList */
LPITEMIDLIST ItemIdListFromPath( String const & filePath );
/* ��һ��·�����ItemIDList,���������
   �����߸�����SHGetMalloc(),IMalloc::Free()�ͷŷ��ص�ItemIDList */
LPITEMIDLIST ItemIdListFromPathSimple( String const & filePath );
/* ��һ��·�����ItemIDList,�ɶ���
   �����߸�����SHGetMalloc(),IMalloc::Free()�ͷŷ��ص�ItemIDList */
HRESULT ItemIdListFromPathCustom( LPWSTR path, WIN32_FIND_DATAW * pfd, LPITEMIDLIST * ppidl );

} // namespace winplus

#endif // !defined(__WINPLUS_SHELL_HPP__)
