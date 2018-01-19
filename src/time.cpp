#include <windows.h>
#include "../include/definitions.hpp"
#include "../include/time.hpp"
#include "../include/strings.hpp"

namespace winplus
{

// 1970-01-01 00:00:00的ULARGE_INTEGER描述
static ULARGE_INTEGER __Time1970( void )
{
    SYSTEMTIME st1970 = {0};
    st1970.wYear = 1970;
    st1970.wMonth = 1;
    st1970.wDay = 1;
    st1970.wHour = 0;
    st1970.wMinute = 0;
    st1970.wSecond = 0;
    
    FILETIME ft1970;
    ULARGE_INTEGER time1970;
    SystemTimeToFileTime( &st1970, &ft1970 );
    CopyMemory( &time1970, &ft1970, sizeof(time1970) );
    return time1970;
}

WINPLUS_FUNC_IMPL(ULONGLONG) GetUtcTimeMs( void )
{
    SYSTEMTIME st;
    FILETIME ft;
    ULARGE_INTEGER time;
    ULARGE_INTEGER timeRes;
    GetSystemTime(&st);
    SystemTimeToFileTime( &st, &ft );
    CopyMemory( &time, &ft, sizeof(time) );
    timeRes.QuadPart = time.QuadPart - __Time1970().QuadPart;
    return timeRes.QuadPart / 10000;
}

WINPLUS_FUNC_IMPL(UINT) GetUtcTime( void )
{
    return (UINT)( GetUtcTimeMs() / 1000 );
}

WINPLUS_FUNC_IMPL(SYSTEMTIME) GetUtcSysTime( ULONGLONG utcTimeMs )
{
    ULONGLONG ullTime = utcTimeMs * 10000;
    ULARGE_INTEGER time;
    time.QuadPart = ullTime + __Time1970().QuadPart;
    FILETIME ft;
    CopyMemory( &ft, &time, sizeof(ft) );
    SYSTEMTIME systime;
    FileTimeToSystemTime( &ft, &systime );
    return systime;
}

WINPLUS_FUNC_IMPL(SYSTEMTIME) GetLocalSysTime( ULONGLONG utcTimeMs )
{
    ULONGLONG ullTime = utcTimeMs * 10000;
    ULARGE_INTEGER time;
    time.QuadPart = ullTime + __Time1970().QuadPart;
    FILETIME ft,localft;
    CopyMemory( &ft, &time, sizeof(ft) );
    FileTimeToLocalFileTime( &ft, &localft );
    SYSTEMTIME systime;
    FileTimeToSystemTime( &localft, &systime );
    return systime;
}

static String CB_ReplaceFunc( MultiMatch const * matchObj, INT item, LPVOID extra )
{
    SYSTEMTIME time = *(SYSTEMTIME *)extra;

    switch ( item )
    {
    case 0:
        return Format( "%04u", time.wYear );
    case 1:
        return Format( "%02u", time.wMonth );
    case 2:
        return Format( "%02u", time.wDay );
    case 3:
        return Format( "%02u", time.wHour );
    case 4:
        return Format( "%02u", time.wMinute );
    case 5:
        return Format( "%02u", time.wSecond );
    case 6:
        return Format( "%03u", time.wMilliseconds );
    }

    return TEXT("");
}

WINPLUS_FUNC_IMPL(String) FormatTime( String const & fmt, SYSTEMTIME const & time )
{
    String mat[] = { "<yr>", "<mo>", "<dy>", "<h>", "<m>", "<s>", "<ms>" };
    return MultiMatch( mat, countof(mat), CB_ReplaceFunc, (LPVOID)&time ).replace(fmt);
}

} // namespace winplus
