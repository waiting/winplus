//////////////////////////////////////////////////////////////////////////
// Purpose:   枚举文件示例
// Desc:      WinPlus sample 1
// Author:    WT
// Date:      2013-05-30
// URL:       http://www.x86pro.com/winplus/samples/1
//////////////////////////////////////////////////////////////////////////

#include <winplus.hpp>
using namespace winplus;
#pragma comment( lib, "winplus-MTd" )

int main()
{
	StringArray files;
	EnumFiles( "F:\\Projects\\winplus\\src", StrSplitComma("cpp,h,c,hpp"), &files );
	EnumFiles( "F:\\Projects\\winplus\\include", StrSplitComma("cpp,h,c,hpp"), &files );
	std::cout << dirSep << std::endl;

	int i;
	for ( i = 0; i < files.size(); ++i )
	{
		std::cout << files[i] << std::endl;
	}

	return 0;
}


