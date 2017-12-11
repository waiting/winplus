# Microsoft Developer Studio Project File - Name="StaticLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=StaticLib - Win32 MD Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "StaticLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "StaticLib.mak" CFG="StaticLib - Win32 MD Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "StaticLib - Win32 MT Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "StaticLib - Win32 MT Release" (based on "Win32 (x86) Static Library")
!MESSAGE "StaticLib - Win32 MD Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "StaticLib - Win32 MD Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "StaticLib - Win32 MT Debug"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin/debug/StaticLib"
# PROP Intermediate_Dir "../bin/debug/StaticLib/MTd"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/winplus-lib-MTd.lib"

!ELSEIF  "$(CFG)" == "StaticLib - Win32 MT Release"

# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin/release/StaticLib"
# PROP Intermediate_Dir "../bin/release/StaticLib/MT"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"../lib/winplus-lib-MT.lib"

!ELSEIF  "$(CFG)" == "StaticLib - Win32 MD Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "../bin/debug/StaticLib"
# PROP BASE Intermediate_Dir "../bin/debug/StaticLib/MDd"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "../bin/debug/StaticLib"
# PROP Intermediate_Dir "../bin/debug/StaticLib/MDd"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"../lib/winplus-lib-MDd.lib"

!ELSEIF  "$(CFG)" == "StaticLib - Win32 MD Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "../bin/release/StaticLib"
# PROP BASE Intermediate_Dir "../bin/release/StaticLib/MD"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin/release/StaticLib"
# PROP Intermediate_Dir "../bin/release/StaticLib/MD"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD LIB32 /nologo /out:"../lib/winplus-lib-MD.lib"

!ENDIF 

# Begin Target

# Name "StaticLib - Win32 MT Debug"
# Name "StaticLib - Win32 MT Release"
# Name "StaticLib - Win32 MD Debug"
# Name "StaticLib - Win32 MD Release"
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
# End Target
# End Project
