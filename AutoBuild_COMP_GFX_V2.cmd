setlocal enabledelayedexpansion
set EXITCODE=0
set TMPDIR=lib
set OUTPUTDIR=Output

IF "%1"=="-X86_SW" (
	echo "**************************** Build for X86_SW ****************************"
) ELSE IF "%1"=="-X86_HW" (
	echo "**************************** Build for X86_HW ****************************"
) ELSE IF "%1"=="-CE5_SW" (
	echo "**************************** Build for CE5_SW ****************************"
) ELSE IF "%1"=="-CE6_SW" (
	echo "**************************** Build for CE6_SW ****************************"
) ELSE IF "%1"=="-CE6_TITIANII" (
	echo "**************************** Build for CE6_TITIANII ****************************"
) ELSE (
	echo "**************************** Unknown build ****************************"
	set EXITCODE=1
	GOTO END
)

REM===============================================================================================================================================
REM
REM 1. clean all project output
REM 2. Remove Output Dir
REM
REM===============================================================================================================================================

if exist %OUTPUTDIR% rmdir /Q /S %OUTPUTDIR% & set /a EXITCODE+=%ERRORLEVEL% & IF NOT "%EXITCODE%" == "0" GOTO END
if exist %TMPDIR% rmdir /Q /S %TMPDIR% & set /a EXITCODE+=%ERRORLEVEL% & IF NOT "%EXITCODE%" == "0" GOTO END

IF "%1"=="-X86_SW" (
devenv.com build_v2\LibVRPresent.sln /Clean "X86_SW|Win32"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|Win32"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build_v2\LibVRPresent.sln /Build "X86_SW|Win32"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|Win32"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_V2_Win32_X86_SW.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_Win32_X86_SW.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_Win32.exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-X86_HW" (
devenv.com build_v2\LibVRPresent.sln /Clean "X86_HW|Win32"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|Win32"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build_v2\LibVRPresent.sln /Build "X86_HW|Win32"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|Win32"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_Win32_X86_HW.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_V2_Win32_X86_HW.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_DXVA.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_Win32.exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-CE5_SW" (
devenv.com build_v2\LibVRPresent.sln /Clean "CE5_SW|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build_v2\LibVRPresent.sln /Build "CE5_SW|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_V2_STANDARDSDK_500 (ARMV4I)_CE5_SW.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARDSDK_500 (ARMV4I)_CE5_SW.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARDSDK_500 (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-CE6_SW" (
devenv.com build_v2\LibVRPresent.sln /Clean "CE6_SW|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build_v2\LibVRPresent.sln /Build "CE6_SW|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARD_CE6_SDK (ARMV4I)_CE6_SW.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_V2_STANDARD_CE6_SDK (ARMV4I)_CE6_SW.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARD_CE6_SDK (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-CE6_TITIANII" (
devenv.com build_v2\LibVRPresent.sln /Clean "CE6_TITIANII|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build_v2\LibVRPresent.sln /Build "CE6_TITIANII|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARD_CE6_SDK (ARMV4I)_CE6_TITIANII.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_V2_STANDARD_CE6_SDK (ARMV4I)_CE6_TITIANII.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARD_CE6_SDK (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE (
	echo "**************************** Unknown build ****************************"
	set EXITCODE=1
	GOTO END
)


xcopy ".\inc\GfxPresent.h"  	.\Output\ /Y
xcopy ".\test\LibVrPresent_Test\res\SUB_720x36.YUV"  	.\Output\ /Y
xcopy ".\test\LibVrPresent_Test\res\TTX_480x520.YUV"  	.\Output\ /Y

set /a EXITCODE+=%ERRORLEVEL%
IF NOT "%EXITCODE%" == "0" GOTO END

:END
exit %EXITCODE%