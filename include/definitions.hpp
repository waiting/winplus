//////////////////////////////////////////////////////////////////////////
// purpose: WinPlus����ض���
// author:  WT
//////////////////////////////////////////////////////////////////////////

#if !defined(__WINPLUS_DEFINITIONS_HPP__)
#define __WINPLUS_DEFINITIONS_HPP__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning( disable : 4786 )
#pragma warning( disable : 4663 )
#pragma warning( disable : 4239 )
#pragma warning( disable : 4018 )
#pragma warning( disable : 4290 )

#ifdef __GNUC__
#define WINVER 0x0501
#define _WIN32_IE 0x0501
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

// ��������DLL����DLL�ļ����Ժ�
// ������DLLʱ(����DLL����ʹ��DLL), WINPLUS_DLL_EXPORTS�����ھ����ǵ���(����)���ǵ���(ʹ��).
// WINPLUS_API���ھ�������Լ��, ��dllʱΪstdcall, ��dllʱΪĬ��.
// ����DLLʱ, ��Ϊ�հ�, ���Լ���.
#ifndef	EXIST_WINPLUS_DLL

	#define	WINPLUS_DLL
	#define WINPLUS_API

#else

	#ifdef	WINPLUS_DLL_EXPORTS
		#define	WINPLUS_DLL	__declspec(dllexport)
	#else
		#define	WINPLUS_DLL	__declspec(dllimport)
	#endif

	#define WINPLUS_API __stdcall

#endif

#define WINPLUS_DECL(ret) WINPLUS_DLL ret WINPLUS_API
#define WINPLUS_IMPL(ret) ret WINPLUS_API

// win32 platform --------------------------------------------------------
#include <windows.h>
#include <comdef.h> // com wrapper
#include <shlobj.h> // shell ���
#include <gdiplus.h> // gdi+
//#include <dsclient.h> // DsClient
// cpp stl ---------------------------------------------------------------
#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <exception>
// c standard functions --------------------------------------------------
#include <math.h>
#include <io.h>
// custom macro definitions ----------------------------------------------
namespace winplus
{

#ifdef NOMINMAX

	#ifndef max
		#define max(a,b)            (((a) > (b)) ? (a) : (b))
	#endif

	#ifndef min
		#define min(a,b)            (((a) < (b)) ? (a) : (b))
	#endif

#endif

#ifndef countof
#define countof(arr) ( sizeof(arr) / sizeof(arr[0]) )
#endif

// ������ת��Ϊansi_string�����ƴ�
#define BufferToAnsiString( buf, size ) AnsiString( (char const *)(buf), (size_t)(size) )

// if pointer
#define IfPTR(ptr) if( (ptr) != NULL ) (ptr)
#define AssignPTR(ptr) if( (ptr) != NULL ) *(ptr)

// ����֧�� --------------------------------------------------------------

typedef std::basic_string<CHAR> AnsiString;
typedef AnsiString LocalString;
typedef std::basic_string<WCHAR> UnicodeString;
typedef std::basic_string<TCHAR> String;

typedef std::vector<AnsiString> AnsiStringArray;
typedef std::vector<LocalString> LocalStringArray;
typedef std::vector<UnicodeString> UnicodeStringArray;
typedef std::vector<String> StringArray;

typedef std::list<AnsiString> AnsiStringList;
typedef std::list<LocalString> LocalStringList;
typedef std::list<UnicodeString> UnicodeStringList;
typedef std::list<String> StringList;

typedef std::map< AnsiString, AnsiString > AnsiStringMap;
typedef std::map<String, String> StringMap;
// basic types -----------------------------------------------------------
typedef wchar_t wchar;

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;

#ifdef __GNUC__
typedef unsigned long long uint64;
typedef unsigned long long ulonglong;
typedef long long int64;
typedef long long longlong;
#else
typedef unsigned __int64 uint64;
typedef unsigned __int64 ulonglong;
typedef __int64 int64;
typedef __int64 longlong;
#endif

#ifndef byte
typedef unsigned char byte;
#endif

// ����壬�ܱ�ʾ�������͵�ֵ��MT_ARRAY��MTBINARY�ݲ�֧�� ----------------
enum MixedType
{
	MT_NULL,
	MT_BOOLEAN,
	MT_BYTE,
	MT_SHORT, MT_USHORT,
	MT_INT, MT_UINT,
	MT_LONG, MT_ULONG,
	MT_FLOAT,
	MT_INT64, MT_UINT64,
	MT_DOUBLE,
	MT_STRING, MT_UNICODE,
	MT_ARRAY,
	MT_BINARY,
};

/* ���ֵ�ṹ�� */
struct MIXED
{
	MixedType type;
	uint buf_size;  // ����ָ��ʱ�������ʾ����������
	union
	{
		bool boolVal;
		byte btVal;
		short shVal;
		ushort ushVal;
		int iVal;
		uint uiVal;
		long lVal;
		ulong ulVal;
		float fltVal;
		int64 i64Val;
		uint64 ui64Val;
		double dblVal;
		struct // �ַ���
		{
			int str_length; // �ַ������ȣ��ַ���
			union
			{
				PSTR pStr;
				PWSTR pWStr;
			};
		};
		struct // ����
		{
			int elements_count;
			struct MIXED * pArray;
		};
		byte * pBinary; // ����������
	};
};

/* ������� */
class Mixed : public MIXED
{
public:
	Mixed( void );
	Mixed( AnsiString const & str ); // ���ֽ��ַ���
	Mixed( UnicodeString const & str ); // Unicode�ַ���
	Mixed( PCSTR str ); // ���ֽ��ַ���
	Mixed( PCWSTR str ); // Unicode�ַ���
	Mixed( bool boolVal );
	Mixed( byte btVal );
	Mixed( short shVal );
	Mixed( ushort ushVal );
	Mixed( int iVal );
	Mixed( uint uiVal );
	Mixed( long lVal );
	Mixed( ulong ulVal );
	Mixed( float fltVal );
	Mixed( int64 i64Val );
	Mixed( uint64 ui64Val );
	Mixed( double dblVal );
	Mixed( Mixed const & other );
	~Mixed( void );
	Mixed & operator = ( Mixed const & other );
	// ����ת��
	operator AnsiString( void ) const;
	operator UnicodeString( void ) const;
	operator bool( void ) const;
	operator byte( void ) const;
	operator short( void ) const;
	operator ushort( void ) const;
	operator int( void ) const;
	operator uint( void ) const;
	operator long( void ) const;
	operator ulong( void ) const;
	operator float( void ) const;
	operator int64( void ) const;
	operator uint64( void ) const;
	operator double( void ) const;
protected:
	void free( void );
	void assign( PCSTR str );
	void assign( PCWSTR str );
	void assign( bool boolVal );
	void assign( byte btVal );
	void assign( short shVal );
	void assign( ushort ushVal );
	void assign( int iVal );
	void assign( uint uiVal );
	void assign( long lVal );
	void assign( ulong ulVal );
	void assign( float fltVal );
	void assign( int64 i64Val );
	void assign( uint64 ui64Val );
	void assign( double dblVal );
};

// map ֧�� --------------------------------------------------------------
/* ���map���Ƿ��иü���ֵ */
template < typename _MAP, typename _KEY >
inline bool isset( _MAP const & m, _KEY const & k )
{
	return m.find(k) != m.end();
}

// �ַ���ת����64λ���� --------------------------------------------------

int64 StrToInt64( AnsiString const & num_str, int ibase );
uint64 StrToUint64( AnsiString const & num_str, int ibase );

/* �����,�������n1~n2������. ����n1,n2���� */
int Random( int n1, int n2 );
// ģ��Ԫ���֧�� --------------------------------------------------------
/* ����һ������void�ĺ�����������,����һ������
   ͨ����Ϊ���ڳ�ʼ������з�����÷���void�ĺ��� */
template < typename FN >
int VoidReturnInt( FN fn )
{
	fn();
	return 1;
}
template < typename FN, typename ARG1 >
int VoidReturnInt( FN fn, ARG1 a1 )
{
	fn(a1);
	return 1;
}
template < typename FN, typename ARG1, typename ARG2 >
int VoidReturnInt( FN fn, ARG1 a1, ARG2 a2 )
{
	fn( a1, a2 );
	return 1;
}
template < typename FN, typename ARG1, typename ARG2, typename ARG3 >
int VoidReturnInt( FN fn, ARG1 a1, ARG2 a2, ARG3 a3 )
{
	fn( a1, a2, a3 );
	return 1;
}


/* ��������,����ʱ����, 1��ͷ(����10����) */
template < uint64 n > struct Bin1
{
	enum { val = ( Bin1< n / 10 >::val  << 1 ) + n % 10 };
};

template <> struct Bin1<0>
{
	enum { val = 0 };
};

/* ��������,����ʱ����, 0��ͷ(����8����) */
template < uint64 n > struct Bin0
{
	enum { val = ( Bin0< n / 8 >::val  << 1 ) + n % 8 };
};

template <> struct Bin0<0>
{
	enum { val = 0 };
};

} // namespace winplus

#endif // !defined(__WINPLUS_DEFINITIONS_HPP__)
