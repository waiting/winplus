# Microsoft Developer Studio Project File - Name="DynamicLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=DynamicLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DynamicLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DynamicLib.mak" CFG="DynamicLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DynamicLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "DynamicLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DynamicLib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../lib"
# PROP BASE Intermediate_Dir "../bin/release/DynamicLib/objs"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "../bin/release/DynamicLib/objs"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DYNAMICLIB_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXIST_WINPLUS_DLL" /D "WINPLUS_DLL_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../lib/winplus.dll"

!ELSEIF  "$(CFG)" == "DynamicLib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../lib"
# PROP BASE Intermediate_Dir "../bin/debug/DynamicLib/objs"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../lib"
# PROP Intermediate_Dir "../bin/debug/DynamicLib/objs"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "DYNAMICLIB_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "EXIST_WINPLUS_DLL" /D "WINPLUS_DLL_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../lib/winplus-d.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DynamicLib - Win32 Release"
# Name "DynamicLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\compoundfile.cpp
# End Source File
# Begin Source File

SOURCE=..\src\console.cpp
# End Source File
# Begin Source File

SOURCE=..\src\definitions.cpp
# End Source File
# Begin Source File

SOURCE=..\src\graphics.cpp
# End Source File
# Begin Source File

SOURCE=..\src\ini.cpp
# End Source File
# Begin Source File

SOURCE=..\src\md5.cpp
# End Source File
# Begin Source File

SOURCE=..\src\picture.cpp
# End Source File
# Begin Source File

SOURCE=..\src\resource.cpp
# End Source File
# Begin Source File

SOURCE=..\src\shell.cpp
# End Source File
# Begin Source File

SOURCE=..\src\strings.cpp
# End Source File
# Begin Source File

SOURCE=..\src\system.cpp
# End Source File
# Begin Source File

SOURCE=..\src\time.cpp
# End Source File
# Begin Source File

SOURCE=..\src\winctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\src\winplus.def
# End Source File
# Begin Source File

SOURCE=..\src\wordslib.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\include\compoundfile.hpp
# End Source File
# Begin Source File

SOURCE=..\include\console.hpp
# End Source File
# Begin Source File

SOURCE=..\include\definitions.hpp
# End Source File
# Begin Source File

SOURCE=..\include\graphics.hpp
# End Source File
# Begin Source File

SOURCE=..\include\ini.hpp
# End Source File
# Begin Source File

SOURCE=..\include\md5.hpp
# End Source File
# Begin Source File

SOURCE=..\include\picture.hpp
# End Source File
# Begin Source File

SOURCE=..\include\resource.hpp
# End Source File
# Begin Source File

SOURCE=..\include\shell.hpp
# End Source File
# Begin Source File

SOURCE=..\include\strings.hpp
# End Source File
# Begin Source File

SOURCE=..\include\system.hpp
# End Source File
# Begin Source File

SOURCE=..\include\time.hpp
# End Source File
# Begin Source File

SOURCE=..\include\winctrl.hpp
# End Source File
# Begin Source File

SOURCE=..\include\winplus.hpp
# End Source File
# Begin Source File

SOURCE=..\include\wordslib.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
