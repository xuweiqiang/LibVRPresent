REM===========================================================================
REM
REM	platform switchs description"
REM	  1st switch	
REM	  	-WCE	build for WinCE"
REM	  	-X86	build for X86"
REM	  2nd switch
REM		 -nc	no clean."
REM
REM===========================================================================
setlocal ENABLEDELAYEDEXPANSION
set EXITCODE=0
set OutputDir=Output
set DIRNAME=%OutputDir%\LibSubtitle

IF "%1"=="-X86" (
	echo "**************************** Build LibSUB X86 ****************************"
) ELSE IF "%1"=="-WCE" (
	echo "**************************** Build LibSUB WCE ****************************"
) ELSE (
	set EXITCODE=1
)

IF NOT "%EXITCODE%" == "0" GOTO END

REM===============================================================================================================================================
REM
REM 1. clean all project output
REM 2. Remove output directory
REM
REM===============================================================================================================================================
IF "%1"=="-X86" (
	devenv.com .\TestCode\DumpTest\CI_SUB_DLL_Test.sln /clean "Release|Win32"
	set /a EXITCODE+=!ERRORLEVEL!
	devenv.com .\TestCode\DumpTest\CI_SUB_DLL_Test.sln /clean "Debug|Win32"
	set /a EXITCODE+=!ERRORLEVEL!
)

IF NOT "%EXITCODE%" == "0" GOTO END

IF exist ".\%OutputDir%" rd /S /Q ".\%OutputDir%"

mkdir %DIRNAME%
set /a EXITCODE+=%ERRORLEVEL%

IF NOT "%EXITCODE%" == "0" GOTO END

REM=========================================================================================================================
REM
REM 1. build Player SDK
REM 2. copy all *.h / *.lib / *.dll under \sdk to output folder
REM
REM=========================================================================================================================

set /a EXITCODE+=%ERRORLEVEL%
IF NOT "%EXITCODE%" == "0" GOTO END

IF "%1"=="-X86" (
	devenv.com .\TestCode\DumpTest\CI_SUB_DLL_Test.sln /build "Release|Win32"
	set /a EXITCODE+=!ERRORLEVEL!
	devenv.com .\TestCode\DumpTest\CI_SUB_DLL_Test.sln /build "Debug|Win32"
	set /a EXITCODE+=!ERRORLEVEL!
)

IF NOT "%EXITCODE%" == "0" GOTO END

set /a EXITCODE+=%ERRORLEVEL%
IF NOT "%EXITCODE%" == "0" GOTO END

REM=========================================================================================================================
REM
REM 1. copy files to target directory for installer
REM
REM=========================================================================================================================

xcopy ".\include\SUBDecoder.h"  "%DIRNAME%\" /Y /F
set /a EXITCODE+=%ERRORLEVEL%
IF NOT "%EXITCODE%" == "0" GOTO END

IF "%1"=="-X86" (
	xcopy ".\build\Win32\Debug\*.dll"  ".\%DIRNAME%\" /Y
	set /a EXITCODE+=!ERRORLEVEL!
	IF NOT "!EXITCODE!" == "0" GOTO END

	xcopy ".\build\Win32\Release\*.dll"  ".\%DIRNAME%\" /Y
	set /a EXITCODE+=!ERRORLEVEL!
	IF NOT "!EXITCODE!" == "0" GOTO END
) ELSE IF "%1"=="-WCE" (
	xcopy ".\build\WinCE\STANDARDSDK_500 (ARMV4I)\Debug\*.dll"  ".\%DIRNAME%\" /Y 
	set /a EXITCODE+=!ERRORLEVEL!
	IF NOT "!EXITCODE!" == "0" GOTO END

	xcopy ".\build\WinCE\STANDARDSDK_500 (ARMV4I)\Release\*.dll"  ".\%DIRNAME%\" /Y 
	set /a EXITCODE+=!ERRORLEVEL!
	IF NOT "!EXITCODE!" == "0" GOTO END
)

IF NOT "%EXITCODE%" == "0" GOTO END

:END
if NOT "%2"=="-nc" (
	echo "Delete files for customer related"
  
) ELSE (
	echo "Do NOT delete files"
)


exit %EXITCODE%

endlocal
