# Microsoft Developer Studio Project File - Name="3DEngine" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=3DEngine - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "3DEngine.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "3DEngine.mak" CFG="3DEngine - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "3DEngine - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "3DEngine - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "3DEngine - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib ddraw.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "3DEngine - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib ddraw.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "3DEngine - Win32 Release"
# Name "3DEngine - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ccontrol.cpp
# End Source File
# Begin Source File

SOURCE=.\cdecorator_cisector.cpp
# End Source File
# Begin Source File

SOURCE=.\cengine_base.cpp
# End Source File
# Begin Source File

SOURCE=.\cengine_bsp.cpp
# End Source File
# Begin Source File

SOURCE=.\cengine_gportal.cpp
# End Source File
# Begin Source File

SOURCE=.\cengine_sportal.cpp
# End Source File
# Begin Source File

SOURCE=.\cgraph.cpp
# End Source File
# Begin Source File

SOURCE=.\ciengine.cpp
# End Source File
# Begin Source File

SOURCE=.\cisector.cpp
# End Source File
# Begin Source File

SOURCE=.\ckeyboard.cpp
# End Source File
# Begin Source File

SOURCE=.\cmain.cpp
# End Source File
# Begin Source File

SOURCE=.\cmouse.cpp
# End Source File
# Begin Source File

SOURCE=.\common.cpp
# End Source File
# Begin Source File

SOURCE=.\cplayer.cpp
# End Source File
# Begin Source File

SOURCE=.\csimplybridge.cpp
# End Source File
# Begin Source File

SOURCE=.\csimplydoor.cpp
# End Source File
# Begin Source File

SOURCE=.\csimplyplatform.cpp
# End Source File
# Begin Source File

SOURCE=.\csimplysector.cpp
# End Source File
# Begin Source File

SOURCE=.\csimplyteleport.cpp
# End Source File
# Begin Source File

SOURCE=.\cswitchsector.cpp
# End Source File
# Begin Source File

SOURCE=.\ctexturefollow.cpp
# End Source File
# Begin Source File

SOURCE=.\cunit.cpp
# End Source File
# Begin Source File

SOURCE=.\cvideo.cpp
# End Source File
# Begin Source File

SOURCE=.\cwallmap.cpp
# End Source File
# Begin Source File

SOURCE=.\cwnd_main.cpp
# End Source File
# Begin Source File

SOURCE=.\localmath.cpp
# End Source File
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ccontrol.h
# End Source File
# Begin Source File

SOURCE=.\cdecorator_cisector.h
# End Source File
# Begin Source File

SOURCE=.\cengine_base.h
# End Source File
# Begin Source File

SOURCE=.\cengine_bsp.h
# End Source File
# Begin Source File

SOURCE=.\cengine_gportal.h
# End Source File
# Begin Source File

SOURCE=.\cengine_sportal.h
# End Source File
# Begin Source File

SOURCE=.\cgraph.h
# End Source File
# Begin Source File

SOURCE=.\ciengine.h
# End Source File
# Begin Source File

SOURCE=.\cisector.h
# End Source File
# Begin Source File

SOURCE=.\ckeyboard.h
# End Source File
# Begin Source File

SOURCE=.\cmain.h
# End Source File
# Begin Source File

SOURCE=.\cmouse.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\cplayer.h
# End Source File
# Begin Source File

SOURCE=.\csimplybridge.h
# End Source File
# Begin Source File

SOURCE=.\csimplydoor.h
# End Source File
# Begin Source File

SOURCE=.\csimplyplatform.h
# End Source File
# Begin Source File

SOURCE=.\csimplysector.h
# End Source File
# Begin Source File

SOURCE=.\csimplyteleport.h
# End Source File
# Begin Source File

SOURCE=.\cswitchsector.h
# End Source File
# Begin Source File

SOURCE=.\ctexturefollow.h
# End Source File
# Begin Source File

SOURCE=.\cunit.h
# End Source File
# Begin Source File

SOURCE=.\cvideo.h
# End Source File
# Begin Source File

SOURCE=.\cwallmap.h
# End Source File
# Begin Source File

SOURCE=.\cwnd_main.h
# End Source File
# Begin Source File

SOURCE=.\localmath.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
