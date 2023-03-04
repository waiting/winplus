#include <winux.hpp>
#include <winplus.hpp>
using namespace std;
int main()
{
    winplus::DirIterator dir{"."};
    auto iptr = winplus::MakeShared(new int);
    while ( dir.next() )
    {
        cout << dir.getFullPath() << endl;
    }

    return 0;
}
