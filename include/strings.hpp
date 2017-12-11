//////////////////////////////////////////////////////////////////////////
// purpose: �ִ����
// author:  WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_STRINGS_HPP__)
#define __WINPLUS_STRINGS_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace winplus
{

/* ����һ�������ַ������ж��ٸ�ʵ�ʵ��ַ�(by local CodePage),����mbstowcs */
UINT LocalCharsCount( LocalString const & local );
/* ����һ��unicode�ַ������ٳ���(by local CodePage),����wcstombs */
UINT UnicodeMinLength( UnicodeString const & unicode );
/* Unicodeת��������Ansi */
LocalString UnicodeToLocal( UnicodeString const & unicode );
/* ����ANSIת��Unicode */
UnicodeString LocalToUnicode( LocalString const & local );

/* �����ַ�����Unicode,Local�ַ����໥ת�� */
String LocalToString( LocalString const & local );
String UnicodeToString( UnicodeString const & unicode );
LocalString StringToLocal( String const & str );
UnicodeString StringToUnicode( String const & str );

/* UTF8����ݴ�֮���ת�� */
String Utf8ToString( AnsiString const & utf8 );
AnsiString StringToUtf8( String const & str );

/* �ָ��ַ��� */
INT StrSplit( String const & str, String const & delim, StringArray * arr );
INT StrSplit( AnsiString const & str, AnsiString const & delim, AnsiStringArray * arr );
INT StrSplit( UnicodeString const & str, UnicodeString const & delim, UnicodeStringArray * arr );

StringArray StrSplit( String const & str, String const & delim );
/* ʹ�ö��ŷָ��ַ��� */
inline StringArray StrSplitComma( String const & str )
{
	return StrSplit( str, TEXT(",") );
}
/* ʹ�÷ֺŷָ��ַ��� */
inline StringArray StrSplitSemicolon( String const & str )
{
	return StrSplit( str, TEXT(";") );
}
String StrJoin( String const & delim, StringArray const & arr );
String StrInsert( String const & str, INT start, INT end, String const & insert );
String FormatExV( INT cch, LPCTSTR fmt, va_list args );
String FormatEx( INT cch, LPCTSTR fmt, ... );
String Format( LPCTSTR fmt, ... );
// ȥ���������˵Ŀհ��ַ�
AnsiString _StrTrim( AnsiString const & str );
UnicodeString _StrTrim( UnicodeString const & str );
String _StrTrim( String const & str );

/* �����ַ�����Դ */
UnicodeString LoadStringExW( HMODULE module, UINT id );
String LoadStringEx( HMODULE module, UINT id );
/* ����GetModuleHandle(NULL)ָ��ģ�� */
String LoadString( UINT id );

/* ��б�ܲ��� */
AnsiString AddSlashes( AnsiString const & str, AnsiString const & charlist );
AnsiString StripSlashes( AnsiString const & str, AnsiString const & charlist );

/* KMP����ƥ���滻 */
class MultiMatch
{
public:
	typedef std::map<String, String> StringMap;
	typedef std::vector<String> StringArray;
	#define KmpCalcNext _Templ_KmpCalcNext<short>
	typedef std::vector<short> KmpNextValueArray;

	struct MatchState
	{
		int j; //��һ�δ��Ӵ���ʼ��λ��
		int markpos;// ��ǣ���ʾ���е����λ���ˣ��ô����λ�ÿ�ʼ
	};

	typedef std::vector<MatchState> MatchStates;

	struct MatchResult
	{
		int pos;
		int item;   // ƥ���������
	};

	typedef String (* ReplaceFuncType)( MultiMatch const * matchObj, int item, void * extra );

	MultiMatch( String matches[], uint m, String replaces[], uint n );
	MultiMatch( String matches[], uint count, ReplaceFuncType replaceFunc, void * extra = NULL );
	MultiMatch( StringArray const & matches, StringArray const & replaces );
	MultiMatch( StringArray const & matches, ReplaceFuncType replaceFunc, void * extra = NULL );

	MultiMatch();
	void init( StringArray const & matches, StringArray const & replaces );
	void init( StringArray const & matches, ReplaceFuncType replaceFunc, void * extra = NULL );

	/** ���һ��ƥ���滻��,����Ҫƥ������� */
	int addMatchReplacePair( String const & match, String const & replace );
	/** ���Ҫƥ���� */
	int addMatch( String const & match );
	/** �����µ��滻�ص����������ؾɵ��滻�ص����� */
	ReplaceFuncType setReplaceFunc( ReplaceFuncType newReplaceFunc );

	/** ��������һ��ƥ�� str:�ַ���, offset:ƫ��,��ʾ���ĸ�λ�ÿ�ʼ��
	 * ���ص�λ�ð���offsetֵ */
	MatchResult search( String const & str, int offset = 0 ) const;
	/* ̰��ģʽ */
	//MatchResult greedSearch( String const & str, int offset = 0 ) const;
	/** �滻 */
	String replace( String const & str) const;

	String getMatchItem( int item ) const;
	String getReplaceItem( int item ) const;

private:
	std::vector<KmpNextValueArray> _nextVals;
	StringArray _matchItems;
	StringArray _replaceItems;
	ReplaceFuncType _replaceFunc;
	void * _extra;
};

///////////////////////////////////////////////////////////////////////////
/* ���based-zero�ַ�����������װ�� */
class SZInput
{
	union
	{
		char * _psz;
		wchar_t * _pwsz;
	};
	enum { szCharInput, szWCharInput } _type;
	size_t _count;
public:
	explicit SZInput( char * psz, size_t count ) : _psz(psz), _type(szCharInput), _count(count) { }
	explicit SZInput( wchar_t * pwsz, size_t count ) : _pwsz(pwsz), _type(szWCharInput), _count(count) { }

	SZInput & operator = ( char const * pstr )
	{
		switch ( _type )
		{
		case szCharInput:
			{
				size_t uTextLen = strlen(pstr);
				if ( _count - 1 < uTextLen )
				{
					memcpy( _psz, pstr, sizeof(char) * ( _count - 1 ) );
					_psz[_count - 1] = 0;
				}
				else
				{
					memcpy( _psz, pstr, sizeof(char) * uTextLen );
					_psz[uTextLen] = 0;
				}
			}
			break;
		case szWCharInput:
			{
				UnicodeString str = LocalToUnicode(pstr);
				size_t uTextLen = str.length();
				if ( _count - 1 < uTextLen )
				{
					memcpy( _pwsz, str.c_str(), sizeof(wchar_t) * ( _count - 1 ) );
					_pwsz[_count - 1] = 0;
				}
				else
				{
					memcpy( _pwsz, str.c_str(), sizeof(wchar_t) * uTextLen );
					_pwsz[uTextLen] = 0;
				}
			}
			break;
		}
		return *this;
	}

	SZInput & operator = ( wchar_t const * pwstr )
	{
		switch ( _type )
		{
		case szCharInput:
			{
				LocalString str = UnicodeToLocal(pwstr);
				size_t uTextLen = str.length();
				if ( _count - 1 < uTextLen )
				{
					memcpy( _psz, str.c_str(), sizeof(char) * ( _count - 1 ) );
					_psz[_count - 1] = 0;
				}
				else
				{
					memcpy( _psz, str.c_str(), sizeof(char) * uTextLen );
					_psz[uTextLen] = 0;
				}
			}
			break;
		case szWCharInput:
			{
				size_t uTextLen = wcslen(pwstr);
				if ( _count - 1 < uTextLen )
				{
					memcpy( _pwsz, pwstr, sizeof(wchar_t) * ( _count - 1 ) );
					_pwsz[_count - 1] = 0;
				}
				else
				{
					memcpy( _pwsz, pwstr, sizeof(wchar_t) * uTextLen );
					_pwsz[uTextLen] = 0;
				}
			}
			break;
		}
		return *this;
	}

};

#if _MSC_VER >= 1600
/* VC2010����֧��ģ��ȡ�����С */
template < typename _CHAR, DWORD _N >
SZInput SZ( _CHAR (&sz)[_N] )
{
	return SZInput( sz, _N );
}

#else
/* ����ʹ�ú궨�� */
#define SZ(sz) SZInput( sz, sizeof(sz) / sizeof(sz[0]) )

#endif
/* ������������ǻ�����ָ�룬��ʹ��SZP�� */
#define SZP SZInput

} // namespace winplus

#endif // !defined(__WINPLUS_STRINGS_HPP__)
