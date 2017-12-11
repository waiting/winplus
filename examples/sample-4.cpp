//////////////////////////////////////////////////////////////////////////
// Purpose:   INI¶ÁÐ´ÊµÀý
// Desc:      WinPlus sample 4
// Author:    WT
// Date:      2013-06-07
//////////////////////////////////////////////////////////////////////////

#include <winplus.hpp>
using namespace winplus;

int main()
{
	RECT rc = Window_GetRect( GetConsoleWindow() );
	Ini( ModulePath() + "\\sample-4.Ini" )
	("Test")
		.set( "exepath", ModulePath() )
		.set( "curpath", GetCurrentDir() )
		.ends()
	("Window")
		.set("x", rc.left )
		.set("y", rc.top )
		.set("width", rc.right - rc.left )
		.set("height", rc.bottom - rc.top )
		//.set("rect", &rc, sizeof(rc) )
		.ends()
	;
	return 0;
}


