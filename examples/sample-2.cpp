//////////////////////////////////////////////////////////////////////////
// Purpose:   词库分词示例
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

        std::cout << "词库名称:" << lib.name << std::endl;
        std::cout << "词汇数量:" << lib.wordscount << std::endl;
        std::cout << "词项大小:" << lib.itemsize << std::endl;

        /*string w;
        while ( "" != ( w = lib.next() ) )
        {
            std::cout << w << std::endl;
        }*/

        StringArray words;
        std::cout << lib.splitWords( "给你玩玩新版的。你可以用你的程序生成词库，更换词库" , &words ) << std::endl;
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


