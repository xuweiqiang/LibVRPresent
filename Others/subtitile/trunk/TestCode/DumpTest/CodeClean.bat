@echo on
@echo Clean VS2005 Solution, Please Wait...
@for /r . %%a in (*.sln) do @call devenv "%%a" /Clean "Debug"
@for /r . %%a in (*.sln) do @call devenv "%%a" /Clean "Release"

@echo Delete Win32\Debug ...
@for /r . %%a in (.) do @if exist "%%a\Win32\Debug" rd /s /q "%%a\Win32\Debug"
@echo Delete Win32\Release ...
@for /r . %%a in (.) do @if exist "%%a\Win32\Release" rd /s /q "%%a\Win32\Release"
@echo Pocket PC 2003 (ARMV4) ...
@for /r . %%a in (.) do @if exist "%%a\Pocket PC 2003 (ARMV4)" rd /s /q "%%a\Pocket PC 2003 (ARMV4)"
@echo Delete Windows Mobile 5.0 Pocket PC SDK (ARMV4I) ...
@for /r . %%a in (.) do @if exist "%%a\Windows Mobile 5.0 Pocket PC SDK (ARMV4I)" rd /s /q "%%a\Windows Mobile 5.0 Pocket PC SDK (ARMV4I)"
@echo Delete Windows MX31Mobility (ARMV4I) ...
@for /r . %%a in (.) do @if exist "%%a\MX31Mobility (ARMV4I)" rd /s /q "%%a\MX31Mobility (ARMV4I)"
@echo Delete WWindows Mobile 6 Professional SDK (ARMV4I) ...
@for /r . %%a in (.) do @if exist "%%a\Windows Mobile 6 Professional SDK (ARMV4I)" rd /s /q "%%a\Windows Mobile 6 Professional SDK (ARMV4I)"
@echo Delete Debug ...
@for /r . %%a in (.) do @if exist "%%a\debug" rd /s /q "%%a\debug"
@echo Delete CVS ...
@for /r . %%a in (.) do @if exist "%%a\CVS" rd /s /q "%%a\CVS"

@echo On
@attrib -r -s -h *.suo /s
@attrib -r -s -h *.user /s
@attrib -r -s -h *.obj /s
@attrib -r -s -h *.bak /s
@attrib -r -s -h *.ncb /s
@attrib -r -s -h *.plg /s
@attrib -r -s -h *.idb /s
@attrib -r -s -h *.pdb /s
@attrib -r -s -h *.opt /s
@attrib -r -s -h *.aps /s
@attrib -r -s -h *.pch /s
@del BuildLog.htm *.suo *.user *.obj *.bak *.ncb *.plg *.idb *.pdb *.opt *.aps *.pch/s

@echo Do you want to clean up .svn folder? Y--Yes, N--No
@set choice=
@set /p choice=Choice:
@if not '%choice%'=='' set choice=%choice:~0,1%
@if '%choice%'=='Y' goto Delete_SVN
@if '%choice%'=='y' goto Delete_SVN
@if '%choice%'=='N' goto Skip
@if '%choice%'=='n' goto Skip
:Skip
@echo Not Delete .svn ...
@goto End
:Delete_SVN
@echo Delete .svn ...
@for /r . %%a in (.) do @if exist "%%a\.svn" rd /s /q "%%a\.svn"
@goto End

:End
@pause