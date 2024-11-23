#include <winux.hpp>
#include <winplus.hpp>
using namespace std;
int main()
{
    winplus::Locale loc;
    winplus::Registry reg{ TEXT(R"(HKEY_CURRENT_USER\QB\TablePanel\msgjug.msgjug_users(u))") };
    winplus::String name;
    winplus::Mixed v;
    while ( reg.enumValues( &name, &v ) )
    {
        wcout << name << TEXT(", ") << winplus::TypeStringW(v) << TEXT(", ") << v << endl;
    }
    if ( reg.setValue( TEXT("multi2"), winplus::$a{ 123, 456, 789 } ) )
    {
        wcout << TEXT("Ok\n");
    }
    return 0;
}
