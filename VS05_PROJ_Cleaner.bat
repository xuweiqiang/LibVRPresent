@echo on
@echo Precessing, Please Wait...
@for /r . %%a in (*.vcproj) do @call VS05_PROJ_Cleaner.exe "%%a"

@pause