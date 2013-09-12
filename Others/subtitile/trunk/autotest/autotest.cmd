@echo off
xcopy /Y libSUBDecoder.dll D:\SUB_AutoTest\
d:
cd D:\SUB_AutoTest\
call Auto_YUVCompare.bat
exit ERRORLEVEL