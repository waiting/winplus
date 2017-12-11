//////////////////////////////////////////////////////////////////////////
// Purpose:   主题API调用测试
// Desc:      WinPlus sample 3
// Author:    WT
// Date:      2013-06-05
//////////////////////////////////////////////////////////////////////////

#include <winplus.hpp>
#include <uxtheme.h>
#pragma comment ( lib, "uxtheme" )

int main()
{
	HRESULT hr;
/*
	hr = EnableTheming(TRUE);
	if ( SUCCEEDED(hr) )
	{
		std::cout << "禁用主题成功\n";
	}*/

	hr = SetWindowTheme( GetConsoleWindow(), L"", L"" );
	if ( SUCCEEDED(hr) )
	{
		std::cout << "SetWindowTheme()成功\n";
	}

	unicode_string themeFileName, colorBuff, sizeBuff;
	themeFileName.resize(1024);
	colorBuff.resize(1024);
	sizeBuff.resize(1024);
	hr = GetCurrentThemeName( &themeFileName[0], 1024, &colorBuff[0], 1024, &sizeBuff[0], 1024 );
	if ( SUCCEEDED(hr) )
	{
		std::cout << unicode_to_local(themeFileName) << std::endl;
		std::cout << unicode_to_local(colorBuff) << std::endl;
		std::cout << unicode_to_local(sizeBuff) << std::endl;
	}
	return 0;
}


