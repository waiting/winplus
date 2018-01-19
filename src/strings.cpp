#include <windows.h>
#include "../include/definitions.hpp"
#include "../include/strings.hpp"
#include <tchar.h>
#include <new>

namespace winplus
{

////////////////////////////////////////////////////////////////////////////////////
// 注意，经试验证明basic_string::resize()不会额外分配NUL字符的空间。
// 所以，即使是空串，也至少要分配一个字符空间存储NUL
// 通常是resize( length + 1 )调用形式
////////////////////////////////////////////////////////////////////////////////////
WINPLUS_FUNC_IMPL(UINT) LocalCharsCount( LocalString const & local )
{
    UINT count = 0U;
    count = MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED | MB_USEGLYPHCHARS,
        local.c_str(),
        local.length(),
        NULL,
        0
    );
    return count;
}

WINPLUS_FUNC_IMPL(UINT) UnicodeMinLength( UnicodeString const & unicode )
{
    UINT length = 0;
    length = WideCharToMultiByte(
        CP_ACP,
        WC_SEPCHARS | WC_COMPOSITECHECK,
        unicode.c_str(),
        unicode.length(),
        NULL,
        0,
        NULL,
        NULL
    );
    return length;
}

WINPLUS_FUNC_IMPL(LocalString) UnicodeToLocal( UnicodeString const & unicode )
{
    LocalString str;
    UINT length = UnicodeMinLength(unicode);
    str.resize( length + 1 );
    WideCharToMultiByte(
        CP_ACP,
        WC_SEPCHARS | WC_COMPOSITECHECK,
        unicode.c_str(),
        unicode.length(),
        &str[0],
        length,
        NULL,
        NULL
    );
    return str.c_str();
}

WINPLUS_FUNC_IMPL(UnicodeString) LocalToUnicode( LocalString const & local )
{
    UnicodeString str;
    UINT count = LocalCharsCount(local);
    str.resize( count + 1 );
    MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED | MB_USEGLYPHCHARS,
        local.c_str(),
        local.length(),
        &str[0],
        count
    );
    return str.c_str();
}

WINPLUS_FUNC_IMPL(String) LocalToString( LocalString const & local )
{
#ifdef UNICODE
    return LocalToUnicode(local);
#else
    return local;
#endif
}

WINPLUS_FUNC_IMPL(String) UnicodeToString( UnicodeString const & unicode )
{
#ifdef UNICODE
    return unicode;
#else
    return UnicodeToLocal(unicode);
#endif
}

WINPLUS_FUNC_IMPL(LocalString) StringToLocal( String const & str )
{
#ifdef UNICODE
    return UnicodeToLocal(str);
#else
    return str;
#endif
}

WINPLUS_FUNC_IMPL(UnicodeString) StringToUnicode( String const & str )
{
#ifdef UNICODE
    return str;
#else
    return LocalToUnicode(str);
#endif
}

WINPLUS_FUNC_IMPL(String) Utf8ToString( AnsiString const & utf8 )
{
    UnicodeString unicode;
    INT cch = MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8.c_str(),
        utf8.length(),
        NULL,
        0
    );
    unicode.resize( cch + 1 );
    MultiByteToWideChar(
        CP_UTF8,
        0,
        utf8.c_str(),
        utf8.length(),
        &unicode[0],
        cch
    );
    return UnicodeToString(unicode.c_str());
}

WINPLUS_FUNC_IMPL(AnsiString) StringToUtf8( String const & str )
{
    UnicodeString unicode = StringToUnicode(str);
    AnsiString utf8;
    INT length = WideCharToMultiByte(
        CP_UTF8,
        0,
        unicode.c_str(),
        unicode.length(),
        NULL,
        0,
        NULL,
        NULL
    );
    utf8.resize( length + 1 );
    WideCharToMultiByte(
        CP_UTF8,
        0,
        unicode.c_str(),
        unicode.length(),
        &utf8[0],
        length,
        NULL,
        NULL
    );
    return utf8.c_str();
}

WINPLUS_FUNC_IMPL(INT) StrSplit( AnsiString const & str, AnsiString const & delim, AnsiStringArray * arr )
{
    CHAR * dup;
    CHAR * token;
    INT count;
    dup = _strdup( str.c_str() );
    token = strtok( dup, delim.c_str() );
    count = 0;
    while ( token != NULL )
    {
        count++;
        if ( arr != NULL ) arr->push_back(token);
        token = strtok( NULL, delim.c_str() );
    }
    free(dup);
    return count;
}

WINPLUS_FUNC_IMPL(INT) StrSplit( UnicodeString const & str, UnicodeString const & delim, UnicodeStringArray * arr )
{
    WCHAR * dup;
    WCHAR * token;
    INT count;
    dup = _wcsdup( str.c_str() );
    token = wcstok( dup, delim.c_str() );
    count = 0;
    while ( token != NULL )
    {
        count++;
        if ( arr != NULL ) arr->push_back(token);
        token = wcstok( NULL, delim.c_str() );
    }
    free(dup);
    return count;
}

WINPLUS_FUNC_IMPL(StringArray) StrSplit( String const & str, String const & delim )
{
    StringArray arr;
    StrSplit( str, delim, &arr );
    return arr;
}

WINPLUS_FUNC_IMPL(String) StrJoin( String const & delim, StringArray const & arr )
{
    String res;
    INT count = arr.size();
    INT i;
    for ( i = 0; i < count; i++ )
    {
        if ( i != 0 )
        {
            res += delim;
        }
        res += arr[i];
    }
    return res;
}

WINPLUS_FUNC_IMPL(String) StrInsert( String const & str, INT start, INT end, String const & insert )
{
    String res;
    res += str.substr( 0, start );
    res += insert;
    if ( end < str.length() )
        res += str.substr(end);
    return res;
}

String FormatExV( INT cch, LPCTSTR fmt, va_list args )
{
    String str;
    str.resize(cch+1);
    _vsntprintf( &str[0], cch, fmt, args );
    return str.c_str();
}

String FormatEx( INT cch, LPCTSTR fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    return FormatExV( cch, fmt, args );
}

String Format( LPCTSTR fmt, ... )
{
    va_list args;
    va_start( args, fmt );
    return FormatExV( 1024, fmt, args );
}

WINPLUS_FUNC_IMPL(AnsiString) _StrTrim( AnsiString const & str )
{
    CHAR const * p1, * p2;
    p1 = str.c_str();
    p2 = str.c_str() + str.length();
    while ( p1 < p2 && ( *p1 == ' ' || *p1 == '\r' || *p1 == '\n' || *p1 == '\t' || *p1 == '\v' ) )
    {
        p1++;
    }
    while ( p2 > p1 && ( p2[-1] == ' ' || p2[-1] == '\r' || p2[-1] == '\n' || p2[-1] == '\t' || p2[-1] == '\v' ) )
    {
        p2--;
    }

    return AnsiString( p1, p2 );
}

WINPLUS_FUNC_IMPL(UnicodeString) _StrTrim( UnicodeString const & str )
{
    WCHAR const * p1, * p2;
    p1 = str.c_str();
    p2 = str.c_str() + str.length();
    while ( p1 < p2 && ( *p1 == L' ' || *p1 == L'\r' || *p1 == L'\n' || *p1 == L'\t' || *p1 == L'\v' ) )
    {
        p1++;
    }
    while ( p2 > p1 && ( p2[-1] == L' ' || p2[-1] == L'\r' || p2[-1] == L'\n' || p2[-1] == L'\t' || p2[-1] == L'\v' ) )
    {
        p2--;
    }

    return UnicodeString( p1, p2 );
}

WINPLUS_FUNC_IMPL(UnicodeString) LoadStringExW( HMODULE module, UINT id )
{
    UnicodeString result;
    WORD blockId = ( id >> 4 ) + 1;
    WORD strResIndex = id % 16;
    HRSRC resource = FindResource( module, MAKEINTRESOURCE(blockId), RT_STRING );
    if ( resource != NULL )
    {
        DWORD size = SizeofResource( module, resource );
        HGLOBAL loadedRes = LoadResource( module, resource );
        LPBYTE data = (LPBYTE)LockResource(loadedRes);
        DWORD offset = 0;
        INT index = 0;
        while ( offset + sizeof(WORD) <= size )
        {
            WORD w = *(WORD *)( data + offset );
            offset += sizeof(WORD);
            if ( w > 0 )
            {
                if ( index == strResIndex )
                {
                    UnicodeString str;
                    str.resize(w+1);
                    CopyMemory( &str[0], data + offset, w * sizeof(WCHAR) );
                    result = str.c_str();
                    break;
                }
                offset += w * sizeof(WCHAR);
            }
            index++;
        }
        FreeResource(loadedRes);
    }
    return result.c_str();
}

WINPLUS_FUNC_IMPL(String) LoadStringEx( HMODULE module, UINT id )
{
    return UnicodeToString( LoadStringExW( module, id ) );
}

WINPLUS_FUNC_IMPL(String) LoadString( UINT id )
{
    return LoadStringEx( GetModuleHandle(NULL), id );
}

// internal functions ****************************************************

inline static bool is_oct( char ch )
{
    return ch >= '0' && ch <= '7';
}
inline static bool is_special( char ch )
{
    return ch == 'a' || ch == 'b' || ch == 't' || ch == 'n' || ch == 'v' || ch == 'f' || ch == 'r';
}
inline static bool is_hex( char ch )
{
    return ch >= '0' && ch <= '9' || ch >= 'A' && ch <= 'F' || ch >= 'a' && ch <= 'f';
}
inline static bool in_charlist( char ch, const char * charlist )
{
    return AnsiString(charlist).find( ch ) != AnsiString::npos;
}
inline static char string_to_char( const char * number, INT base )
{
    char * endptr;
    return (char)strtol( number, &endptr, base );
}
inline static char special_to_char( char special )
{
    switch ( special )
    {
    case 'a':// 响铃(BEL)  07/7
        special = '\a';
        break;
    case 'b':// 退格符(BS) 08H/8
        special = '\b';
        break;
    case 't':// 水平制表符(HT) 09H/9
        special = '\t';
        break;
    case 'n':// 换行符(LF) 0AH/10
        special = '\n';
        break;
    case 'v':// 垂直制表(VT) 0B/11
        special = '\v';
        break;
    case 'f':// 换页符(FF) 0CH/12
        special = '\f';
        break;
    case 'r':// 回车符(CR) 0DH/13
        special = '\r';
        break;
    }
    return special;
}

WINPLUS_FUNC_IMPL(AnsiString) AddSlashes( AnsiString const & str, AnsiString const & charlist )
{
    AnsiString slashes = "";
    for ( AnsiString::const_iterator it = str.begin(); it != str.end(); it++ )
    {
        char ch = *it;
        if ( charlist.find(ch) != AnsiString::npos )
        {
            AnsiString slash = "";
            switch ( ch )
            {
            case '\a':// 响铃(BEL)  07/7
                slash = "\\a";
                break;
            case '\b':// 退格符(BS) 08H/8
                slash = "\\b";
                break;
            case '\t':// 水平制表符(HT) 09H/9
                slash = "\\t";
                break;
            case '\n':// 换行符(LF) 0AH/10
                slash = "\\n";
                break;
            case '\v':// 垂直制表(VT) 0B/11
                slash = "\\v";
                break;
            case '\f':// 换页符(FF) 0CH/12
                slash = "\\f";
                break;
            case '\r':// 回车符(CR) 0DH/13
                slash = "\\r";
                break;
            default:
                {
                    if ( is_special(ch) || ch <= ' ' || ch & (unsigned char)0x80 )
                    {
                        char s[8] = { 0 };
                        sprintf( s, "\\x%02x", (unsigned)( ch & 0xFF ) );
                        slash = s;
                    }
                    else
                    {
                        slash += '\\';
                        slash += ch;
                    }
                }
                break;
            }
            slashes += slash;
        }
        else
        {
            slashes += ch;
        }
    }
    return slashes;
}

WINPLUS_FUNC_IMPL(AnsiString) StripSlashes( AnsiString const & str, AnsiString const & charlist )
{
    AnsiString result = "";
    for ( AnsiString::const_iterator it = str.begin(); it != str.end(); )
    {
        const char current = *it;
        if ( current == '\\' )
        {
            it++; // skip '\\'
            if ( it != str.end() ) // \后有字符
            {
                for ( ; it != str.end(); it++ )
                {
                    char ch0 = *it; // ch0表示\后的一个字符
                    if ( is_oct(ch0) )
                    {
                        AnsiString ch0s = "";
                        for ( ; it != str.end(); it++ )
                        {
                            ch0 = *it;
                            if ( is_oct(ch0) && ch0s.length() < 3 )
                            {
                                ch0s += ch0;
                            }
                            else
                            {
                                break;
                            }
                        }
                        if ( ch0s.length() > 0 )
                        {
                            char c1 = string_to_char( ch0s.c_str(), 8 );
                            if ( in_charlist( c1, charlist.c_str() ) )
                            {
                                result += c1;
                            }
                            else
                            {
                                result += '\\';
                                result += ch0s;
                            }
                        }

                        break;
                    }
                    else if ( ch0 == 'x' ) // is x 16进制
                    {
                        it++; // skip 'x'
                        if ( it != str.end() )
                        {
                            if ( is_hex(*it) )
                            {
                                AnsiString ch0s = "";
                                for ( ; it != str.end(); it++ )
                                {
                                    ch0 = *it;
                                    if ( is_hex(ch0) && ch0s.length() < 2 )
                                    {
                                        ch0s += ch0;
                                    }
                                    else
                                    {
                                        break;
                                    }
                                }

                                if ( ch0s.length() > 0 )
                                {
                                    char c2 = string_to_char( ch0s.c_str(), 16 );
                                    if ( in_charlist( c2, charlist.c_str() ) )
                                    {
                                        result += c2;
                                        break;
                                    }
                                    else
                                    {
                                        result += '\\';
                                        result += 'x';
                                        result += ch0s;
                                        break;
                                    }
                                }
                                break;
                            }
                            else // x后不是16进制字符
                            {
                                result += '\\';
                                result += 'x';
                                break;
                            }
                        }
                        else // 后面已经没有字符
                        {
                            result += '\\';
                            result += 'x';
                            break;
                        }
                    }
                    else if ( is_special(ch0) )
                    {
                        char special = 0;
                        special = special_to_char(ch0);
                        // 查看CharList是否含有此字符
                        if ( in_charlist( special, charlist.c_str() ) )
                        {
                            result += special;
                            it++; // skip ch0
                            break;
                        }
                        else
                        {
                            result += '\\';
                            result += ch0;
                            it++; // skip ch0
                            break;
                        }
                    }
                    else if ( in_charlist( ch0, charlist.c_str() ) )
                    {
                        result += ch0;
                        it++; // skip ch0
                        break;
                    }
                    else // 都不是
                    {
                        result += '\\';
                        break;
                    }
                } // end for loop
            }
            else // \后没有字符
            {
                result += current;
            }
        }
        else // 当前字符不是 \ 
        {
            result += current;
            it++;
        }
    }
    return result;
}

/* KMP字符串匹配算法 next值 */
template < typename _IndexType >
inline static std::vector< _IndexType > _Templ_KmpCalcNext( String const & substr )
{
    _IndexType length = ( _IndexType )substr.length();
    std::vector< _IndexType > next( length + 1 );
    _IndexType j = 0, k = -1;
    next[0] = -1;
    while ( j < length )
    {
        if ( k == -1 || substr[j] == substr[k] )
        {
            j++;
            k++;
            if ( substr[j] != substr[k] )
                next[j] = k;
            else
                next[j] = next[k];
        }
        else
        {
            k = next[k];
        }
    }
    return next;
}

// class MultiMatch ---------------------------------------------------------
MultiMatch::MultiMatch( String matches[], uint m, String replaces[], uint n )
{
    _construct();
    init( StringArray( matches, matches + m ), StringArray( replaces, replaces + n ) );
}

MultiMatch::MultiMatch( String matches[], uint count, ReplaceFuncType replaceFunc, void * extra /*= NULL */ )
{
    _construct();
    init( StringArray( matches, matches + count ), replaceFunc, extra );
}

MultiMatch::MultiMatch( StringArray const & matches, StringArray const & replaces )
{
    _construct();
    init( matches, replaces );
}

MultiMatch::MultiMatch( StringArray const & matches, ReplaceFuncType replaceFunc, void * extra /*= NULL */ )
{
    _construct();
    init( matches, replaceFunc, extra );
}

MultiMatch::MultiMatch()
{
    _construct();
}

void MultiMatch::init( StringArray const & matches, StringArray const & replaces )
{
    _matchItems = matches;
    _replaceItems = replaces;
    for ( StringArray::const_iterator it = _matchItems.begin(); it != _matchItems.end(); it++ )
    {
        _nextVals.push_back( KmpCalcNext(*it) );
    }
    _replaceFunc = NULL;
    _extra = NULL;
}

void MultiMatch::init( StringArray const & matches, ReplaceFuncType replaceFunc, void * extra )
{
    _matchItems = matches;
    
    for ( StringArray::const_iterator it = _matchItems.begin(); it != _matchItems.end(); it++ )
    {
        _nextVals.push_back( KmpCalcNext(*it) );
    }
    _replaceFunc = replaceFunc;
    _extra = extra;
}

int MultiMatch::addMatchReplacePair( String const & match, String const & replace )
{
    _matchItems.push_back(match);
    _nextVals.push_back( KmpCalcNext(match) );
    _replaceItems.push_back(replace);
    return (int)_nextVals.size();
}

int MultiMatch::addMatch( String const & match )
{
    _matchItems.push_back(match);
    _nextVals.push_back( KmpCalcNext(match) );
    return (int)_nextVals.size();
}

MultiMatch::ReplaceFuncType MultiMatch::setReplaceFunc( ReplaceFuncType newReplaceFunc )
{
    ReplaceFuncType oldFunc = _replaceFunc;
    _replaceFunc = newReplaceFunc;
    return oldFunc;
}

MultiMatch::MatchResult MultiMatch::search( String const & str, int offset ) const
{
    MatchResult res = { -1, -1 };
    int count = _matchItems.size();
    std::vector<MatchState> states(count);
    TCHAR const * mainStr = str.c_str() + offset;
    int i; // 主字符串当前字符位置
    bool nomove = false; // 不移动i
    memset( &states[0], 0, count * sizeof(MatchState) );
    i = 0;
    while ( mainStr[i] )
    {
        for ( int curr = 0; curr < count; curr++ ) // 各个匹配项进行匹配
        {
            if ( i < states[curr].markpos )
            {
            }
            else
            {
                String const & currItem = _matchItems[curr];
                int currItemLen = (int)currItem.length();
                if ( states[curr].j < currItemLen )
                {
                    if ( states[curr].j == -1 || mainStr[i] == currItem[states[curr].j] )
                    {
                        states[curr].j++;
                        states[curr].markpos = i + 1;
                        if ( states[curr].j == currItemLen ) // 表示当前项已经匹配成功
                        {
                            res.item = curr;
                            res.pos = i + 1 - currItemLen + offset;
                            goto RETURN;
                        }
                    }
                    else
                    {
                        states[curr].j = _nextVals[curr][states[curr].j];
                        nomove = true;
                    }
                }
                else // 表示已经匹配成功
                {
                    res.item = curr;
                    res.pos = i - currItemLen + offset;
                    goto RETURN;
                }
            }
        }
        if ( nomove )
        {
            nomove = false;
        }
        else
        {
            i++;
        }
    }
RETURN:
    return res;

}

/*MultiMatch::MatchResult MultiMatch::greedSearch( String const & str, int offset ) const
{
    MatchResult res = { -1, -1 };
    int count = _matchItems.size();
    String strRegex = StrJoin( "|", _matchItems );
    boost::regex r(strRegex);
    MatchResult res = { -1, -1 };
    int count = _matchItems.size();
    std::vector<MatchState> states(count);
    char const * mainStr = str.c_str() + offset;
    int i = 0; // 主字符串当前字符位置
    bool nomove = false; // 不移动i
    memset( &states[0], 0, count * sizeof(MatchState) );
    bool hasMatch = false;//有匹配项
    int matched;
    while ( mainStr[i] )
    {
        bool matchOneChar = false;//有一个字符匹配
        matched = 0;
        for ( int curr = 0; curr < count; curr++ ) // 各个匹配项进行匹配
        {

            if ( i < states[curr].markpos )
            {
            }
            else
            {
                String const & currItem = _matchItems[curr];
                int currItemLen = (int)currItem.length();
                if ( states[curr].j < currItemLen )
                {
                    if ( states[curr].j == -1 || mainStr[i] == currItem[states[curr].j] )
                    {
                        states[curr].j++;
                        states[curr].markpos = i + 1;
                        matchOneChar = true;
                        if ( states[curr].j == currItemLen ) // 表示当前项已经匹配成功
                        {
                            MatchResult mr;
                            mr.item = curr;
                            mr.pos = i + 1 - currItemLen + offset;
                            hasMatch = true;
                            res = mr;
                            matched++;
                            continue;
                        }
                    }
                    else
                    {
                        states[curr].j = _nextVals[curr][states[curr].j];
                        nomove = true;
                    }
                }
                else
                {

                }
            }
        }

        if ( matched == 1 )
            nomove = false;

        if ( hasMatch && ( !matchOneChar ) && matched == 0 )//有匹配项时，并且没有字符匹配时，检查匹配数组是否为空，然后跳出
            break;

        if ( nomove )
            nomove = false;
        else
            i++;
    }

    return res;

}//*/

String MultiMatch::replace( String const & str ) const
{
    String s = TEXT("");
    int offset = 0;
    int length = (int)str.length();
    MatchResult r;
    MatchResult ( MultiMatch:: * fnSearch )( String const & str, int offset ) const;
    fnSearch = &MultiMatch::search;
    r = (this->*fnSearch)( str.c_str() + offset, 0 );
    while ( r.pos != -1 )
    {
        s += String( str.c_str() + offset, str.c_str() + offset + r.pos );
        if ( _replaceFunc )
            s += _replaceFunc( this, r.item, _extra );
        else
            s += _replaceItems[r.item];
        offset += r.pos + _matchItems[r.item].length();
        r = (this->*fnSearch)( str.c_str() + offset, 0 );
    }
    s += str.c_str() + offset;
    return s;
}

String MultiMatch::getMatchItem( int item ) const
{
    return _matchItems[item];
}

String MultiMatch::getReplaceItem( int item ) const
{
    if ( _replaceFunc )
        return _replaceFunc( this, item, _extra );
    else
        return _replaceItems[item];
}

void MultiMatch::_construct()
{
    _replaceFunc = NULL;
    _extra = NULL;
}

} // namespace winplus
