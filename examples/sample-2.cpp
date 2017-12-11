//////////////////////////////////////////////////////////////////////////
// Purpose:   �ʿ�ִ�ʾ��
// Desc:      WinPlus sample 2
// Author:    WT
// Date:      2013-05-31
//////////////////////////////////////////////////////////////////////////

#include <winplus.hpp>
using namespace winplus;

#pragma comment( lib, "winplus-lib-MTd" )

int main()
{

    try
    {
        WordsLib lib("F:\\Projects\\myProject\\WordProjects\\words.wl");

        std::cout << "�ʿ�����:" << lib.name << std::endl;
        std::cout << "�ʻ�����:" << lib.wordscount << std::endl;
        std::cout << "�����С:" << lib.itemsize << std::endl;

        /*string w;
        while ( "" != ( w = lib.next() ) )
        {
            std::cout << w << std::endl;
        }*/

        StringArray words;
        std::cout << lib.splitWords( "���������°�ġ����������ĳ������ɴʿ⣬�����ʿ�" , &words ) << std::endl;
        StringArray::iterator it = words.begin();
        for ( ; it != words.end(); ++it )
        {
            std::cout << *it << std::endl;
        }

    }
    catch ( WordsLibException e )
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}


