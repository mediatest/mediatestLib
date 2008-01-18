@echo off

rem --- Clean up ---
del MediaInfo_Lib_Source.7z
rmdir MediaInfo_Lib_Source /S /Q
mkdir MediaInfo_Lib_Source


rem --- Copying : Include ---
xcopy ..\Source\MediaInfo\MediaInfo.h MediaInfo_Lib_Source\Include\MediaInfo\
xcopy ..\Source\MediaInfo\MediaInfoList.h MediaInfo_Lib_Source\Include\MediaInfo\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.h MediaInfo_Lib_Source\Include\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.def MediaInfo_Lib_Source\Include\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.pas MediaInfo_Lib_Source\Include\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.cs MediaInfo_Lib_Source\Include\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.jsl MediaInfo_Lib_Source\Include\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.vb MediaInfo_Lib_Source\Include\MediaInfoDLL\
xcopy ..\Source\MediaInfoDLL\MediaInfoDLL.java MediaInfo_Lib_Source\Include\MediaInfoDLL\

rem --- Copying : Documentation ---
mkdir Doc
cd ..\Source\Doc
..\..\..\Shared\Binary\Win32\Doxygen\Doxygen Doxygen
cd ..\..\Release
mkdir MediaInfo_Lib_Source\Doc\
xcopy Doc\*.* MediaInfo_Lib_Source\Doc\
rmdir Doc /S /Q
xcopy ..\Source\Doc\*.html MediaInfo_Lib_Source\ /S

@rem --- Copying : Sources ---
xcopy ..\Source\*.cpp MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.h MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.rc MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.def MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.pas MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.cs MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.jsl MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.vb MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.java MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.txt MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.csv MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.dfm MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\*.au3 MediaInfo_Lib_Source\Source\ /S
xcopy ..\Source\Doc\* MediaInfo_Lib_Source\Source\Doc\ /S

@rem --- Copying : Projects ---
xcopy ..\Project\*.bpg MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.bpf MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.bpr MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.dfm MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.cpp MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.bdsgroup MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.bdsproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.dpr MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.pas MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.dof MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.dev MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.sln MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.cs MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.csproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.jsl MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vjsproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vbproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vb MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vc MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.vcproj MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.ico MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.workspace MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.cbp MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.resx MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\*.rc MediaInfo_Lib_Source\Project\ /S
xcopy ..\Project\BCB\*.h MediaInfo_Lib_Source\Project\BCB\ /S
xcopy ..\Project\MSVC\*.h MediaInfo_Lib_Source\Project\MSVC\ /S
xcopy ..\Project\BCB\*.res MediaInfo_Lib_Source\Project\BCB /S
xcopy ..\Project\Delphi\*.res MediaInfo_Lib_Source\Project\Delphi /S
xcopy ..\Project\MSVB\*.rc MediaInfo_Lib_Source\Project\MSVB\ /S
xcopy "..\Project\MSVB\Example\My Project\*.*" "MediaInfo_Lib_Source\Project\MSVB\Example\My Project\" /Y
xcopy "..\Project\MSVB\Example VB6\*.*" "MediaInfo_Lib_Source\Project\MSVB\Example VB6\" /Y
xcopy ..\Project\GNU\* MediaInfo_Lib_Source\Project\GNU\ /S
xcopy ..\Project\Java\*.bat MediaInfo_Lib_Source\Project\Java\ /S
xcopy ..\Project\Java\*.sh MediaInfo_Lib_Source\Project\Java\ /S
xcopy ..\Project\Java\*.java MediaInfo_Lib_Source\Project\Java\ /S
xcopy ..\Project\Java\*.txt MediaInfo_Lib_Source\Project\Java\ /S

@rem --- Copying : Release ---
xcopy *.txt MediaInfo_Lib_Source\Release\
xcopy *.bat MediaInfo_Lib_Source\Release\
xcopy *.ogg MediaInfo_Lib_Source\Release\
xcopy BCB\*.txt MediaInfo_Lib_Source\Release\BCB\ /S
xcopy MSVC\*.txt MediaInfo_Lib_Source\Release\MSVC\ /S
xcopy GCC_MinGW32\*.txt MediaInfo_Lib_Source\Release\GCC_MinGW32\ /S
xcopy GCC_Linux_i386\*.txt MediaInfo_Lib_Source\Release\GCC_Linux_i386\ /S

@rem --- Copying : Contrib ---
xcopy ..\Contrib\* MediaInfo_Lib_Source\Contrib\ /S

rem --- Copying : Information files ---
copy ..\*.txt MediaInfo_Lib_Source\
copy ..\*.html MediaInfo_Lib_Source\

rem --- Copying : CVS files ---
copy ..\*.cvsignore MediaInfo_Lib_Source\


rem --- Compressing Archive ---
if "%2"=="SkipCompression" goto SkipCompression
move MediaInfo_Lib_Source MediaInfoLib
..\..\Shared\Binary\Win32\7-zip\7z a -r -t7z -mx9 MediaInfo_Lib_Source.7z MediaInfoLib\*
move MediaInfoLib MediaInfo_Lib_Source
:SkipCompression

rem --- Clean up ---
if "%1"=="SkipCleanUp" goto SkipCleanUp
rmdir MediaInfo_Lib_Source /S /Q
:SkipCleanUp
