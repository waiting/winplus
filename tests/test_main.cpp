#include <winux.hpp>
#include <winplus.hpp>
using namespace std;

inline static int __MbsOneChar( char const * & p, char ch[5], bool isUtf8 )
{
    int cch = 0;

    if ( isUtf8 )
    {
        ch[0] = *p;
        cch++;
        if ( ( ch[0] & 0xF0 ) == 0b11110000 )
        {
            p++;
            ch[1] = *p;
            cch++;
            p++;
            ch[2] = *p;
            cch++;
            p++;
            ch[3] = *p;
            cch++;
        }
        else if ( ( ch[0] & 0xE0 ) == 0b11100000 )
        {
            p++;
            ch[1] = *p;
            cch++;
            p++;
            ch[2] = *p;
            cch++;
        }
        else if ( ( ch[0] & 0xC0 ) == 0b11000000 )
        {
            p++;
            ch[1] = *p;
            cch++;
        }
    }
    else // Local code
    {
        ch[0] = *p;
        cch++;
        if ( ch[0] & 0x80 )
        {
            p++;
            ch[1] = *p;
            cch++;
        }
    }

    return cch;
}

int main()
{
    winplus::Locale loc;
    char const * str = u8"药物少女😀的呢喃🐕";
    auto p = str;
    while ( *p )
    {
        char ch[5] = { 0 };
        cout << __MbsOneChar( p, ch, true ) << "\n";
        p++;
    }


    winplus::Registry reg{ TEXT(R"(HKEY_CURRENT_USER\QB\TablePanel\msgjug.msgjug_users(u))") };
    winplus::String name;
    winplus::Mixed v;
    while ( reg.enumValues( &name, &v ) )
    {
        cout << name << TEXT(", ") << winplus::TypeStringA(v) << TEXT(", ") << v << endl;
    }
    if ( reg.setValue( TEXT("multi2"), winplus::$a{ 123, 456, 789 } ) )
    {
        cout << TEXT("Ok\n");
    }
    return 0;
}
