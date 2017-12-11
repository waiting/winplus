//////////////////////////////////////////////////////////////////////////
// purpose:  ��Դ���
// author:   WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_RESOURCE_HPP__)
#define __WINPLUS_RESOURCE_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

class Resource
{
public:
	Resource( HMODULE hModule, UINT nResourceId, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
	{
		new(this) Resource( hModule, MAKEINTRESOURCE(nResourceId), lpType, wLanguage );
	}
	Resource( UINT nResourceId, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
	{
		new(this) Resource( GetModuleHandle(NULL), nResourceId, lpType, wLanguage );
	}
	Resource( LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
	{
		new(this) Resource( GetModuleHandle(NULL), lpszName, lpType, wLanguage );
	}
	Resource( HMODULE hModule, LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) );
	Resource();
	~Resource();

	bool load( HMODULE hModule, LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) );
	bool load( LPCTSTR lpszName, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
	{
		return load( GetModuleHandle(NULL), lpszName, lpType, wLanguage );
	}
	bool load( HMODULE hModule, UINT nResourceId, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
	{
		return load( hModule, MAKEINTRESOURCE(nResourceId), lpType, wLanguage );
	}
	bool load( UINT nResourceId, LPCTSTR lpType, WORD wLanguage = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL ) )
	{
		return load( GetModuleHandle(NULL), nResourceId, lpType, wLanguage );
	}
	void free();
	// ������Ŀ�껺����,����Ŀ�껺��������Դ�Ĵ�С��
	// 0:ǡ����ͬ,����:Ŀ�껺������С������Դ,����:Ŀ�껺������СС����Դ
	int copyTo( LPVOID lpDestination, DWORD dwDestinationSize ) const;

	LPCVOID getData() const { return _lpData; }
	DWORD getSize() const { return _dwSize; }

	operator bool() const { return _lpData != NULL; }
private:
	HRSRC _hRSRC;
	HGLOBAL _hResMemBlock;
	HMODULE _hModule;
	DWORD _dwSize;
	LPCVOID _lpData;
	Resource( Resource const & );
	Resource & operator = ( Resource const & );
};

} // namespace winplus

#endif // !defined(__WINPLUS_RESOURCE_HPP__)
