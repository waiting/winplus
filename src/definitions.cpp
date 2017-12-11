#include <windows.h>
#include "../include/definitions.hpp"
#include "../include/strings.hpp"
#include <stdlib.h>
#include <wchar.h>
#include <new>
#include <sstream>
#include <time.h>
#pragma warning ( disable:4996 )

namespace winplus
{

/* flag values */
#define FL_UNSIGNED   1       /* strtouq called */
#define FL_NEG        2       /* negative sign found */
#define FL_OVERFLOW   4       /* overflow occured */
#define FL_READDIGIT  8       /* we've read at least one correct digit */

static uint64 str_to_xq( char const * nptr, char const * * endptr, int ibase, int flags )
{
    char const * p;
    char c;
    uint64 number;
    uint digval;
    uint64 maxval;

    p = nptr;            /* p is our scanning pointer */
    number = 0;            /* start with zero */

    c = *p++;            /* read char */

    while ( isspace((int)(unsigned char)c) )
        c = *p++;        /* skip whitespace */

    if (c == '-') {
        flags |= FL_NEG;    /* remember minus sign */
        c = *p++;
    }
    else if (c == '+')
        c = *p++;        /* skip sign */

    if (ibase < 0 || ibase == 1 || ibase > 36) {
        /* bad base! */
        if (endptr)
            /* store beginning of string in endptr */
            *endptr = nptr;
        return 0L;        /* return 0 */
    }
    else if (ibase == 0) {
        /* determine base free-lance, based on first two chars of
           string */
        if (c != '0')
            ibase = 10;
        else if (*p == 'x' || *p == 'X')
            ibase = 16;
        else
            ibase = 8;
    }

    if (ibase == 16) {
        /* we might have 0x in front of number; remove if there */
        if (c == '0' && (*p == 'x' || *p == 'X')) {
            ++p;
            c = *p++;    /* advance past prefix */
        }
    }

    /* if our number exceeds this, we will overflow on multiply */
    maxval = _UI64_MAX / ibase;


    for (;;) {    /* exit in middle of loop */
        /* convert c to value */
        if ( isdigit((int)(unsigned char)c) )
            digval = c - '0';
        else if ( isalpha((int)(unsigned char)c) )
            digval = toupper(c) - 'A' + 10;
        else
            break;
        if (digval >= (unsigned)ibase)
            break;        /* exit loop if bad digit found */

        /* record the fact we have read one digit */
        flags |= FL_READDIGIT;

        /* we now need to compute number = number * base + digval,
           but we need to know if overflow occured.  This requires
           a tricky pre-check. */

        if (number < maxval || (number == maxval &&
        (uint64)digval <= _UI64_MAX % ibase)) {
            /* we won't overflow, go ahead and multiply */
            number = number * ibase + digval;
        }
        else {
            /* we would have overflowed -- set the overflow flag */
            flags |= FL_OVERFLOW;
        }

        c = *p++;        /* read next digit */
    }

    --p;                /* point to place that stopped scan */

    if (!(flags & FL_READDIGIT)) {
        /* no number there; return 0 and point to beginning of
           string */
        if (endptr)
            /* store beginning of string in endptr later on */
            p = nptr;
        number = 0L;        /* return 0 */
    }
    else if ( (flags & FL_OVERFLOW) ||
              ( !(flags & FL_UNSIGNED) &&
                ( ( (flags & FL_NEG) && (number > -_I64_MIN) ) ||
                  ( !(flags & FL_NEG) && (number > _I64_MAX) ) ) ) )
    {
        /* overflow or signed overflow occurred */
        errno = ERANGE;
        if ( flags & FL_UNSIGNED )
            number = _UI64_MAX;
        else if ( flags & FL_NEG )
            number = _I64_MIN;
        else
            number = _I64_MAX;
    }
    if (endptr != NULL)
        /* store pointer to char that stopped the scan */
        *endptr = p;

    if (flags & FL_NEG)
        /* negate result if there was a neg sign */
        number = (uint64)(-(int64)number);

    return number;            /* done. */
}

int64 StrToInt64( AnsiString const & num_str, int ibase )
{
    return (int64)str_to_xq( num_str.c_str(), NULL, ibase, 0 );
}
uint64 StrToUint64( AnsiString const & num_str, int ibase )
{
    return str_to_xq( num_str.c_str(), NULL, ibase, FL_UNSIGNED );
}

int Random( int n1, int n2 )
{
	static int seedInit = VoidReturnInt( srand, (unsigned int)time(NULL) );
	return ( rand() * rand() ) % ( abs( n2 - n1 ) + 1 ) + ( n1 < n2 ? 1 : -1 ) * n1;
}
////////////////////////////////////////////////////////////////////////
// 混合体实现
// class Mixed's members --------------------------------------
Mixed::Mixed( void )
{
	memset( this, 0, sizeof( MIXED ) );
	this->type = MT_NULL;
}
Mixed::Mixed( AnsiString const & str )
{
	new ( this ) Mixed(); // call the default constructor
	this->assign( str.c_str() );
}
Mixed::Mixed( UnicodeString const & str )
{
	new ( this ) Mixed(); // call the default constructor
	this->assign( str.c_str() );
}
Mixed::Mixed( PCSTR str ) // 多字节字符串
{
	new ( this ) Mixed(); // call the default constructor
	this->assign( str );
}
Mixed::Mixed( PCWSTR str ) // Unicode字符串
{
	new ( this ) Mixed(); // call the default constructor
	this->assign( str );
}
Mixed::Mixed( bool boolVal )
{
	new ( this ) Mixed();
	this->assign( boolVal );
}
Mixed::Mixed( byte btVal )
{
	new ( this ) Mixed();
	this->assign( btVal );
}
Mixed::Mixed( short shVal )
{
	new ( this ) Mixed();
	this->assign( shVal );
}
Mixed::Mixed( ushort ushVal )
{
	new ( this ) Mixed();
	this->assign( ushVal );
}
Mixed::Mixed( int iVal )
{
	new ( this ) Mixed();
	this->assign( iVal );
}
Mixed::Mixed( uint uiVal )
{
	new ( this ) Mixed();
	this->assign( uiVal );
}
Mixed::Mixed( long lVal )
{
	new ( this ) Mixed();
	this->assign( lVal );
}
Mixed::Mixed( ulong ulVal )
{
	new ( this ) Mixed();
	this->assign( ulVal );
}
Mixed::Mixed( float fltVal )
{
	new ( this ) Mixed();
	this->assign( fltVal );
}
Mixed::Mixed( int64 i64Val )
{
	new ( this ) Mixed();
	this->assign( i64Val );
}
Mixed::Mixed( uint64 ui64Val )
{
	new ( this ) Mixed();
	this->assign( ui64Val );
}
Mixed::Mixed( double dblVal )
{
	new ( this ) Mixed();
	this->assign( dblVal );
}
Mixed::Mixed( Mixed const & other )
{
	new ( this ) Mixed();
	this->operator = ( other );
}
Mixed::~Mixed( void )
{
	this->free();
}
Mixed & Mixed::operator = ( Mixed const & other )
{
	if ( this == &other ) goto RETURN;
	this->free(); // 先清空自己
	switch ( other.type )
	{
	case MT_STRING:
		this->type = other.type;
		this->buf_size = other.buf_size;
		this->str_length = other.str_length;
		this->pStr = (PSTR)malloc( this->buf_size );
		memset( this->pStr, 0, this->buf_size );
		memcpy( this->pStr, other.pStr, this->buf_size );
		break;
	case MT_UNICODE:
		this->type = other.type;
		this->buf_size = other.buf_size;
		this->str_length = other.str_length;
		this->pWStr = (PWSTR)malloc( this->buf_size );
		memset( this->pWStr, 0, this->buf_size );
		memcpy( this->pWStr, other.pWStr, this->buf_size );
		break;
	case MT_ARRAY:
		break;
	case MT_BINARY:
		break;
	default: // 是其他的话，直接copy就好
		memcpy( this, &other, sizeof( other ) );
		break;
	}
RETURN:
	return *this;
}
Mixed::operator AnsiString( void ) const
{
	// other to string
	AnsiString s;
	switch ( this->type )
	{
	case MT_BOOLEAN:
		s = this->boolVal ? "true" : "false";
		break;
	case MT_BYTE:
		{
			char buf[4] = { 0 };
			sprintf( buf, "0x%02X", this->btVal );
			s = buf;
		}
		break;
	case MT_SHORT:
		{
			std::ostringstream sout;
			sout << this->shVal;
			s = sout.str();
		}
		break;
	case MT_USHORT:
		{
			std::ostringstream sout;
			sout << this->ushVal;
			s = sout.str();
		}
		break;
	case MT_INT:
		{
			std::ostringstream sout;
			sout << this->iVal;
			s = sout.str();
		}
		break;
	case MT_UINT:
		{
			std::ostringstream sout;
			sout << this->uiVal;
			s = sout.str();
		}
		break;
	case MT_LONG:
		{
			std::ostringstream sout;
			sout << this->lVal;
			s = sout.str();
		}
		break;
	case MT_ULONG:
		{
			std::ostringstream sout;
			sout << this->ulVal;
			s = sout.str();
		}
		break;
	case MT_FLOAT:
		{
			std::ostringstream sout;
			sout << this->fltVal;
			s = sout.str();
		}
		break;
	case MT_INT64:
		{
			/*std::ostringstream sout;
			sout << this->i64Val;
			s = sout.str();*/
			char buf[30] = { 0 };
			sprintf( buf, "%I64d", this->i64Val );
			s = buf;
		}
		break;
	case MT_UINT64:
		{
			/*std::ostringstream sout;
			sout << this->ui64Val;
			s = sout.str();*/
			char buf[30] = { 0 };
			sprintf( buf, "%I64u", this->ui64Val );
			s = buf;
		}
		break;
	case MT_DOUBLE:
		{
			std::ostringstream sout;
			sout << this->dblVal;
			s = sout.str();
		}
		break;
	case MT_STRING:
		{
			s = this->pStr;
		}
		break;
	case MT_UNICODE:
		{
			// 获取unicode串最少需要多长存储
			int length = UnicodeMinLength( this->pWStr );
			int size = ( length + 1 ) * sizeof( char );
			PSTR _pStr = (PSTR)malloc( size );
			memset( _pStr, 0, size );
			wcstombs( _pStr, this->pWStr, size );
			s = _pStr;
			::free( _pStr );
		}
		break;
	case MT_ARRAY:
		break;
	case MT_BINARY:
		break;
	default:
		s = "Null";
		break;
	}
	return s;
}
Mixed::operator UnicodeString( void ) const
{
	// other to unicode
	UnicodeString s;
	switch ( this->type )
	{
	case MT_BOOLEAN:
		s = this->boolVal ? L"true" : L"false";
		break;
	case MT_BYTE:
		{
			wchar buf[4] = { 0 };
			swprintf( buf, L"0x%02X", this->btVal );
			s = buf;
		}
		break;
	case MT_SHORT:
		{
			std::wostringstream sout;
			sout << this->shVal;
			s = sout.str();
		}
		break;
	case MT_USHORT:
		{
			std::wostringstream sout;
			sout << this->ushVal;
			s = sout.str();
		}
		break;
	case MT_INT:
		{
			std::wostringstream sout;
			sout << this->iVal;
			s = sout.str();
		}
		break;
	case MT_UINT:
		{
			std::wostringstream sout;
			sout << this->uiVal;
			s = sout.str();
		}
		break;
	case MT_LONG:
		{
			std::wostringstream sout;
			sout << this->lVal;
			s = sout.str();
		}
		break;
	case MT_ULONG:
		{
			std::wostringstream sout;
			sout << this->ulVal;
			s = sout.str();
		}
		break;
	case MT_FLOAT:
		{
			std::wostringstream sout;
			sout << this->fltVal;
			s = sout.str();
		}
		break;
	case MT_INT64:
		{
			/*std::wostringstream sout;
			sout << this->i64Val;
			s = sout.str();*/
			wchar buf[30] = { 0 };
			swprintf( buf, L"%I64d", this->i64Val );
			s = buf;

		}
		break;
	case MT_UINT64:
		{
			/*std::wostringstream sout;
			sout << this->ui64Val;
			s = sout.str();*/
			wchar buf[30] = { 0 };
			swprintf( buf, L"%I64u", this->ui64Val );
			s = buf;
		}
		break;
	case MT_DOUBLE:
		{
			std::wostringstream sout;
			sout << this->dblVal;
			s = sout.str();
		}
		break;
	case MT_STRING:
		{
			int length = LocalCharsCount(this->pStr);
			int size = ( length + 1 ) * sizeof( wchar );
			PWSTR _pWStr = (PWSTR)malloc( size );
			memset( _pWStr, 0, size );
			mbstowcs( _pWStr, this->pStr, size );
			s = _pWStr;
			::free( _pWStr );
		}
		break;
	case MT_UNICODE:
		{
			s = this->pWStr;
		}
		break;
	case MT_ARRAY:
		break;
	case MT_BINARY:
		break;
	default:
		s = L"Null";
		break;
	}
	return s;
}
Mixed::operator bool( void ) const
{
	bool b = false;
	switch ( this->type )
	{
	case MT_BOOLEAN:
		b = this->boolVal;
		break;
	case MT_BYTE:
		b = this->btVal != 0;
		break;
	case MT_SHORT:
		b = this->shVal != 0;
		break;
	case MT_USHORT:
		b = this->ushVal != 0;
		break;
	case MT_INT:
		b = this->iVal != 0;
		break;
	case MT_UINT:
		b = this->uiVal != 0;
		break;
	case MT_LONG:
		b = this->lVal != 0;
		break;
	case MT_ULONG:
		b = this->ulVal != 0;
		break;
	case MT_FLOAT:
		b = this->fltVal != 0;
		break;
	case MT_INT64:
		b = this->i64Val != 0;
		break;
	case MT_UINT64:
		b = this->ui64Val != 0;
		break;
	case MT_DOUBLE:
		b = this->dblVal != 0;
		break;
	case MT_STRING:
		if ( _stricmp( this->pStr, "true" ) == 0 || _stricmp( this->pStr, "yes" ) == 0 || _stricmp( this->pStr, "on" ) == 0 )
		{
			b = true;
		}
		else if ( _stricmp( this->pStr, "false" ) == 0 || _stricmp( this->pStr, "no" ) == 0 || _stricmp( this->pStr, "off" ) == 0 )
		{
			b = false;
		}
		else
		{
			b = this->operator ulong() != 0;
		}
		break;
	case MT_UNICODE:
		if ( _wcsicmp( this->pWStr, L"true" ) == 0 || _wcsicmp( this->pWStr, L"yes" ) == 0 || _wcsicmp( this->pWStr, L"on" ) == 0 )
		{
			b = true;
		}
		else if ( _wcsicmp( this->pWStr, L"false" ) == 0 || _wcsicmp( this->pWStr, L"no" ) == 0 || _wcsicmp( this->pWStr, L"off" ) == 0 )
		{
			b = false;
		}
		else
		{
			b = this->operator ulong() != 0;
		}
		break;
	case MT_ARRAY:
		break;
	case MT_BINARY:
		break;
	default:
		break;
	}
	return b;
}
Mixed::operator byte( void ) const
{
	byte bt = 0U;
	switch ( this->type )
	{
	case MT_BOOLEAN:
		bt = (byte)this->boolVal;
		break;
	case MT_BYTE:
		bt = this->btVal;
		break;
	case MT_SHORT:
		bt = (byte)this->shVal;
		break;
	case MT_USHORT:
		bt = (byte)this->ushVal;
		break;
	case MT_INT:
		bt = (byte)this->iVal;
		break;
	case MT_UINT:
		bt = (byte)this->uiVal;
		break;
	case MT_LONG:
		bt = (byte)this->lVal;
		break;
	case MT_ULONG:
		bt = (byte)this->ulVal;
		break;
	case MT_FLOAT:
		bt = (byte)this->fltVal;
		break;
	case MT_INT64:
		bt = (byte)this->i64Val;
		break;
	case MT_UINT64:
		bt = (byte)this->ui64Val;
		break;
	case MT_DOUBLE:
		bt = (byte)this->dblVal;
		break;
	case MT_STRING:
		bt = (byte)this->operator ulong();
		break;
	case MT_UNICODE:
		bt = (byte)this->operator ulong();
		break;
	case MT_ARRAY:
		break;
	case MT_BINARY:
		break;
	default:
		break;
	}
	return bt;
}
Mixed::operator short( void ) const
{
	switch ( this->type )
	{
	case MT_SHORT:
		return this->shVal;
	default:
		return (short)this->operator long();
	}
}
Mixed::operator ushort( void ) const
{
	switch ( this->type )
	{
	case MT_USHORT:
		return this->ushVal;
	default:
		return (ushort)this->operator ulong();
	}
}
Mixed::operator int( void ) const
{
	switch ( this->type )
	{
	case MT_INT:
		return this->iVal;
	default:
		return (int)this->operator long();
	}
}
Mixed::operator uint( void ) const
{
	switch ( this->type )
	{
	case MT_UINT:
		return this->uiVal;
	default:
		return (uint)this->operator ulong();
	}
}
Mixed::operator long( void ) const
{
	union
	{
		long l;
		ulong ul;
	} tmp;
	tmp.ul = this->operator ulong();
	return tmp.l;
}
Mixed::operator ulong( void ) const
{
	ulong ul = 0UL;
	switch ( this->type )
	{
	case MT_BOOLEAN:
		ul = this->boolVal ? 1U : 0U;
		break;
	case MT_BYTE:
		ul = (ulong)this->btVal;
		break;
	case MT_SHORT:
		ul = (ulong)this->shVal;
		break;
	case MT_USHORT:
		ul = (ulong)this->ushVal;
		break;
	case MT_INT:
		ul = (ulong)this->iVal;
		break;
	case MT_UINT:
		ul = (ulong)this->uiVal;
		break;
	case MT_LONG:
		{
			union
			{
				long l;
				ulong ul;
			} tmp;
			tmp.l = this->lVal;
			ul = tmp.ul;
		}
		break;
	case MT_ULONG:
		ul = this->ulVal;
		break;
	case MT_FLOAT:
		ul = (ulong)this->fltVal;
		break;
	case MT_INT64:
		ul = (ulong)this->i64Val;
		break;
	case MT_UINT64:
		ul = (ulong)this->ui64Val;
		break;
	case MT_DOUBLE:
		ul = (ulong)this->dblVal;
		break;
	case MT_STRING:
		if ( this->str_length > 1 && this->pStr[0] == '0' && ( this->pStr[1] == 'x' || this->pStr[1] == 'X' ) )
		{
			ul = strtoul( this->pStr + 2, NULL, 16 );
		}
		else if ( this->str_length > 1 && this->pStr[0] == '0' )
		{
			ul = strtoul( this->pStr + 1, NULL, 8 );
		}
		else
		{
			ul = strtoul( this->pStr, NULL, 10 );
		}
		break;
	case MT_UNICODE:
		if ( this->str_length > 1 && this->pWStr[0] == L'0' && ( this->pWStr[1] == L'x' || this->pWStr[1] == L'X' ) )
		{
			ul = wcstoul( this->pWStr + 2, NULL, 16 );
		}
		else if ( this->str_length > 1 && this->pWStr[0] == L'0' )
		{
			ul = wcstoul( this->pWStr + 1, NULL, 8 );
		}
		else
		{
			ul = wcstoul( this->pWStr, NULL, 10 );
		}
		break;
	case MT_ARRAY:
		break;
	case MT_BINARY:
		break;
	default:
		break;
	}
	return ul;
}
Mixed::operator float( void ) const
{
	switch ( this->type )
	{
	case MT_FLOAT:
		return this->fltVal;
	default:
		return (float)this->operator double();
	}
}
Mixed::operator int64( void ) const
{
	union
	{
		int64 i64;
		uint64 ui64;
	} tmp;
	tmp.ui64 = this->operator uint64();
	return tmp.i64;
}
Mixed::operator uint64( void ) const
{
	uint64 ui64 = 0U;
	switch ( this->type )
	{
	case MT_BOOLEAN:
		ui64 = this->boolVal ? 1U : 0U;
		break;
	case MT_BYTE:
		ui64 = (uint64)this->btVal;
		break;
	case MT_SHORT:
		ui64 = (uint64)this->shVal;
		break;
	case MT_USHORT:
		ui64 = (uint64)this->ushVal;
		break;
	case MT_INT:
		ui64 = (uint64)this->iVal;
		break;
	case MT_UINT:
		ui64 = (uint64)this->uiVal;
		break;
	case MT_LONG:
		ui64 = (uint64)this->lVal;
		break;
	case MT_ULONG:
		ui64 = (uint64)this->ulVal;
		break;
	case MT_FLOAT:
		ui64 = (uint64)this->fltVal;
		break;
	case MT_INT64:
		ui64 = (uint64)this->i64Val;
		break;
	case MT_UINT64:
		ui64 = this->ui64Val;
		break;
	case MT_DOUBLE:
		ui64 = (uint64)this->dblVal;
		break;
	case MT_STRING:
		if ( this->str_length > 1 && this->pStr[0] == '0' && ( this->pStr[1] == 'x' || this->pStr[1] == 'X' ) )
		{
			ui64 = StrToUint64( this->pStr + 2, 16 );
		}
		else if ( this->str_length > 1 && this->pStr[0] == '0' )
		{
			ui64 = StrToUint64( this->pStr + 1, 8 );
		}
		else
		{
			ui64 = StrToUint64( this->pStr, 10 );
		}
		break;
	case MT_UNICODE:
		if ( this->str_length > 1 && this->pWStr[0] == L'0' && ( this->pWStr[1] == L'x' || this->pWStr[1] == L'X' ) )
		{
			ui64 = StrToUint64( UnicodeToLocal( this->pWStr + 2 ), 16 );
		}

		else if ( this->str_length > 1 && this->pWStr[0] == L'0' )
		{
			ui64 = StrToUint64( UnicodeToLocal( this->pWStr + 1 ), 8 );
		}
		else
		{
			ui64 = StrToUint64( UnicodeToLocal(this->pWStr), 10 );
		}
		break;
	case MT_ARRAY:
		break;
	case MT_BINARY:
		break;
	default:
		break;
	}
	return ui64;
}
Mixed::operator double( void ) const
{
	double dbl = 0.0;
	switch ( this->type )
	{
	case MT_BOOLEAN:
		dbl = this->boolVal ? 1.0 : 0.0;
		break;
	case MT_BYTE:
		dbl = (double)this->btVal;
		break;
	case MT_SHORT:
		dbl = (double)this->shVal;
		break;
	case MT_USHORT:
		dbl = (double)this->ushVal;
		break;
	case MT_INT:
		dbl = (double)this->iVal;
		break;
	case MT_UINT:
		dbl = (double)this->uiVal;
		break;
	case MT_LONG:
		dbl = (double)this->lVal;
		break;
	case MT_ULONG:
		dbl = (double)this->ulVal;
		break;
	case MT_FLOAT:
		dbl = (double)this->fltVal;
		break;
	case MT_INT64:
		dbl = (double)this->i64Val;
		break;
	case MT_UINT64:
		dbl = (double)(int64)this->ui64Val;
		break;
	case MT_DOUBLE:
		dbl = this->dblVal;
		break;
	case MT_STRING:
		if ( this->str_length > 1 && this->pStr[0] == '0' && ( this->pStr[1] == 'x' || this->pStr[1] == 'X' ) )
		{
			dbl = (double)StrToInt64( this->pStr + 2, 16 );
		}
		else if ( this->str_length > 1 && this->pStr[0] == '0' && this->pStr[1] != '.' && ( this->pStr[1] != 'e' || this->pStr[1] != 'E' ) )
		{
			dbl = (double)StrToInt64( this->pStr + 1, 8 );
		}
		else
		{
			dbl = strtod( this->pStr, NULL );
		}
		break;
	case MT_UNICODE:
		if ( this->str_length > 1 && this->pWStr[0] == L'0' && ( this->pWStr[1] == L'x' || this->pWStr[1] == L'X' ) )
		{
			dbl = (double)StrToInt64( UnicodeToLocal( this->pWStr + 2 ), 16 );
		}
		else if ( this->str_length > 1 && this->pWStr[0] == L'0' && ( this->pWStr[1] != L'.' || this->pWStr[1] != L'e' || this->pWStr[1] != L'E' ) )
		{
			dbl = (double)StrToInt64( UnicodeToLocal( this->pWStr + 1 ), 8 );
		}
		else
		{
			dbl = wcstod( this->pWStr, NULL );
		}
		break;
	case MT_ARRAY:
		break;
	case MT_BINARY:
		break;
	default:
		break;
	}
	return dbl;
}

void Mixed::free( void )
{
	switch ( this->type )
	{
	case MT_STRING:
		if ( this->pStr != NULL )
		{
			::free( this->pStr );
		}
		break;
	case MT_UNICODE:
		if ( this->pWStr != NULL )
		{
			::free( this->pWStr );
		}
		break;
	default:
		break;
	}
	memset( this, 0, sizeof( MIXED ) );
}
void Mixed::assign( PCSTR str )
{
	this->type = MT_STRING; // set type as AnsiString
	this->str_length = strlen( str ); // calculate string length
	this->buf_size = ( this->str_length + 1 ) * sizeof( char ); // include '\0'
	this->pStr = (PSTR)malloc( this->buf_size ); // allocate memory spaces
	memset( this->pStr, 0, this->buf_size );
	memcpy( this->pStr, str, this->buf_size ); // copy data
}
void Mixed::assign( PCWSTR str )
{
	this->type = MT_UNICODE; // set type as UnicodeString
	this->str_length = wcslen( str ); // calculate string length
	this->buf_size = ( this->str_length + 1 ) * sizeof( wchar ); // include '\0'
	this->pWStr = (PWSTR)malloc( this->buf_size ); // allocate memory spaces
	memset( this->pWStr, 0, this->buf_size );
	memcpy( this->pWStr, str, this->buf_size ); // copy data
}
void Mixed::assign( bool boolVal )
{
	this->type = MT_BOOLEAN;
	this->boolVal = boolVal;
}
void Mixed::assign( byte btVal )
{
	this->type = MT_BYTE;
	this->btVal = btVal;
}
void Mixed::assign( short shVal )
{
	this->type = MT_SHORT;
	this->shVal = shVal;
}
void Mixed::assign( ushort ushVal )
{
	this->type = MT_USHORT;
	this->ushVal = ushVal;
}
void Mixed::assign( int iVal )
{
	this->type = MT_INT;
	this->iVal = iVal;
}
void Mixed::assign( uint uiVal )
{
	this->type = MT_UINT;
	this->uiVal = uiVal;
}
void Mixed::assign( long lVal )
{
	this->type = MT_LONG;
	this->lVal = lVal;
}
void Mixed::assign( ulong ulVal )
{
	this->type = MT_ULONG;
	this->ulVal = ulVal;
}
void Mixed::assign( float fltVal )
{
	this->type = MT_FLOAT;
	this->fltVal = fltVal;
}
void Mixed::assign( int64 i64Val )
{
	this->type = MT_INT64;
	this->i64Val = i64Val;
}
void Mixed::assign( uint64 ui64Val )
{
	this->type = MT_UINT64;
	this->ui64Val = ui64Val;
}
void Mixed::assign( double dblVal )
{
	this->type = MT_DOUBLE;
	this->dblVal = dblVal;
}

} // namespace winplus
