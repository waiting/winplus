//////////////////////////////////////////////////////////////////////////
// purpose: ini�ļ���д
// author:  WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_INI_HPP__)
#define __WINPLUS_INI_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

/* Ini ��д֧���� */
class Ini
{
private:
	String _strIniFile;
public:

	class Section
	{
		Ini * _i;
		LPCTSTR _sectionName;
	public:

		Section( Ini * i, LPCTSTR sectionName ) : _i(i), _sectionName(sectionName) {}
		String get_name() const { return _sectionName; }

		Section & set( String const & key, String const & value );
		Section & set( String const & key, int value );
		Section & set( String const & key, uint value );
		Section & set( String const & key, double value );
		Section & set( String const & key, LPCVOID value, uint size );

		String get( String const & key, String const & defVal ) const;
		int get( String const & key, int defVal ) const;
		uint get( String const & key, uint defVal ) const;
		double get( String const & key, double defVal ) const;
		bool get( String const & key, LPVOID data, uint size ) const;

		Ini & ends() { return *this->_i; }

		//Section operator()( LPCTSTR sectionName )
		//{
		//	return Section( this->_i, sectionName );
		//}
	};

	Section operator()( LPCTSTR sectionName )
	{
		return Section( this, sectionName );
	}

public:
	// ���캯��, bInWindowsDir��ʾ��szIniFile��ָ��·��ʱ, �Ƿ�Ը�����windowsĿ¼��
	// �����Զ���szIniFile��ͷ����".\\"�ַ���, ��ʾ���ڵ�ǰĿ¼��
	Ini( String const & szIniFile, BOOL bInWindowsDir = FALSE );

	BOOL setValue( LPCTSTR szSection, LPCTSTR szKeyName, LPCTSTR szValue );
	BOOL setValue( LPCTSTR szSection, LPCTSTR szKeyName, INT nValue, BOOL bSigned = TRUE );
	BOOL setValue( LPCTSTR szSection, LPCTSTR szKeyName, DOUBLE dblValue );
	BOOL setData( LPCTSTR szSection, LPCTSTR szKeyName, LPCVOID lpDataBuf, UINT uSizeData );

	INT getValue( LPCTSTR szSection, LPCTSTR szKeyName, INT nDefault );
	String getValue( LPCTSTR szSection, LPCTSTR szKeyName, LPCTSTR szDefault );
	DOUBLE getValue( LPCTSTR szSection, LPCTSTR szKeyName, DOUBLE dblDefault );
	BOOL getData( LPCTSTR szSection, LPCTSTR szKeyName, LPVOID lpDataBuf, UINT uSizeData );

	String const & getIniFile() const { return _strIniFile; }
};

} // namespace winplus

#endif // !defined(__WINPLUS_INI_HPP__)
