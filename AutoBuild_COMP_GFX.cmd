setlocal enabledelayedexpansion
set EXITCODE=0
set TMPDIR=lib
set OUTPUTDIR=Output

IF "%1"=="-CE5" (
	echo "**************************** Build for CE5 ****************************"
) ELSE IF "%1"=="-CE6" (
	echo "**************************** Build for CE6 ****************************"
) ELSE IF "%1"=="-X86" (
	echo "**************************** Build for X86 ****************************"
) ELSE IF "%1"=="-MX31" (
	echo "**************************** Build for MX31 ****************************"
) ELSE IF "%1"=="-MT3351" (
	echo "**************************** Build for MT3351 ****************************"
) ELSE IF "%1"=="-SAMSUNG2416" (
	echo "**************************** Build for SAMSUNG2416 ****************************"
) ELSE IF "%1"=="-SAMSUNG2450" (
	echo "**************************** Build for SAMSUNG2450 ****************************"
) ELSE IF "%1"=="-APOLLO_SAMSUNG2450" (
	echo "**************************** Build for APOLLO_SAMSUNG2450 ****************************"
) ELSE IF "%1"=="-TITIANII" (
	echo "**************************** Build for TITIANII ****************************"
) ELSE IF "%1"=="-A4_NEWOS" (
	echo "**************************** Build for A4_NEWOS ****************************"
) ELSE IF "%1"=="-A4_OLDOS" (
	echo "**************************** Build for A4_OLDOS ****************************"
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

IF "%1"=="-CE5" (
devenv.com build\LibGfxContext.sln /Clean "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARDSDK_500 (ARMV4I)_Release.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARDSDK_500 (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-CE6" (
devenv.com build\LibGfxContext.sln /Clean "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARD_CE6_SDK (ARMV4I)_Release.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARD_CE6_SDK (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-X86" (
devenv.com build\LibGfxContext.sln /Clean "Release|Win32"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|Win32"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "Release|Win32"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|Win32"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_Win32_Release.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_Win32.exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-MX31" (
devenv.com build\LibGfxContext.sln /Clean "MX31|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "MX31|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARDSDK_500 (ARMV4I)_MX31.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARDSDK_500 (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-MT3351" (
devenv.com build\LibGfxContext.sln /Clean "MT3351|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "MT3351|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARDSDK_500 (ARMV4I)_MT3351.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARDSDK_500 (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-SAMSUNG2416" (
devenv.com build\LibGfxContext.sln /Clean "SAMSUNG_2416|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "SAMSUNG_2416|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARDSDK_500 (ARMV4I)_SAMSUNG_2416.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARDSDK_500 (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-SAMSUNG2450" (
devenv.com build\LibGfxContext.sln /Clean "SAMSUNG_2450|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "SAMSUNG_2450|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARDSDK_500 (ARMV4I)_SAMSUNG_2450.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARDSDK_500 (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-APOLLO_SAMSUNG2450" (
devenv.com build\LibGfxContext.sln /Clean "APOLLO_SAMSUNG2450|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "APOLLO_SAMSUNG2450|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARDSDK_500 (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARDSDK_500 (ARMV4I)_APOLLO_SAMSUNG2450.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARDSDK_500 (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-TITIANII" (
devenv.com build\LibGfxContext.sln /Clean "TITAN_II|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "TITAN_II|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARD_CE6_SDK (ARMV4I)_TITAN_II.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARD_CE6_SDK (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-A4_NEWOS" (
devenv.com build\LibGfxContext.sln /Clean "SIRF_A4_NEWOS|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "SIRF_A4_NEWOS|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARD_CE6_SDK (ARMV4I)_SIRF_A4_NEWOS.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARD_CE6_SDK (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE IF "%1"=="-A4_OLDOS" (
devenv.com build\LibGfxContext.sln /Clean "SIRF_A4_OLDOS|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Clean "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com build\LibGfxContext.sln /Build "SIRF_A4_OLDOS|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
devenv.com test\LibVrPresent_Test.sln /Build "Release|STANDARD_CE6_SDK (ARMV4I)"
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVRPresent_STANDARD_CE6_SDK (ARMV4I)_SIRF_A4_OLDOS.dll"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
xcopy ".\lib\LibVrPresent_Test_STANDARD_CE6_SDK (ARMV4I).exe"  	.\Output\ /Y
set /a EXITCODE+=!ERRORLEVEL!
) ELSE (
	echo "**************************** Unknown build ****************************"
	set EXITCODE=1
	GOTO END
)


set /a EXITCODE+=!ERRORLEVEL!
IF NOT "%EXITCODE%" == "0" GOTO END

xcopy ".\inc\GfxPresent.h"  	.\Output\ /Y
xcopy ".\test\LibVrPresent_Test\res\SUB_720x36.YUV"  	.\Output\ /Y
xcopy ".\test\LibVrPresent_Test\res\TTX_480x520.YUV"  	.\Output\ /Y

set /a EXITCODE+=%ERRORLEVEL%
IF NOT "%EXITCODE%" == "0" GOTO END

:END
exit %EXITCODE%