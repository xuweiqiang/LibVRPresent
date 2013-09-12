********************************************************************************
* File Structure
* Created by Toby
* 2008/07/28
********************************************************************************
gfx
��  CodeClean.bat	(clean the code by removing temporary files (*.obj, *.svn etc) 
��  VS05_PROJ_Cleaner.bat
��  VS05_PROJ_Cleaner.exe
��
����3rdparty
��  ����FreeScale_MX31 (FreeScale iMX31 ADS need ddraw library)
��  ��      ddraw.lib
��  ��      dxguid.lib
��  ��
��  ����IPP
��  ��  ����pca_wmmx (PXA270/310 CPU needed IPP library)
��  ��  ��      ippIP_WMMX50PPC_d.lib
��  ��  ��      ippIP_WMMX50PPC_r.lib
��  ��  ��
��  ��  ����win32 (IA32/64 CPU needed IPP library)
��  ��          ippcc.lib
��  ��          ippccmerged.lib
��  ��
��  ����TitianEVB (Titan I need ddraw library)
��  ��      ddraw.lib
��  ��      dxguid.lib
��  ��      GenericVideoRender.lib
��  ��
��  ����WIN32  (Win32 need ddraw library)
��          ddraw.lib
��          dxguid.lib
��
����bin 
����build
��  ��  LibGfxContext.sln (Solution files to build libgfx dll library)
��  ��
��  ����LibGfxContext
��  ��      LibGfxContext.vcproj
��  ��
��  ����LibGfxPresent
��          LibGfxPresent.vcproj
��
����doc
����inc (Header files for external user to use libgfx library)
��      GfxContext.h
��      GfxPresent.h
��
����lib (Lib files for external user to use libgfx library)
����src (Source files to build libgfx library)
��  ����LibGfxContext
��  ��      GfxOverlay.cpp
��  ��      GfxOverlay.h
��  ��      GfxTitan.cpp
��  ��      GfxTitan.h
��  ��      GfxWrapper.cpp
��  ��      GfxWrapper.h
��  ��      Gx.h
��  ��      GxBridge.cpp
��  ��      GxBridge.h
��  ��      GxEmu.cpp
��  ��      line_interpolation_arm.asm
��  ��      McCom.h
��  ��      TitanRenderSDK.h
��  ��
��  ����LibGfxPresent
��          AlphaBlending.cpp
��          AlphaBlending.h
��          AlphaBlendingSub.asm
��          AlphaBlendingSub.S
��          arial.cpp
��          arial.h
��          GfxPresent.cpp
��          GfxPresentOvl.cpp
��          GfxPresentRGB.cpp
��          ImageLib.h
��          yuv2rgb.cpp
��          yuv420torgb565_arm.asm
��          yuv420torgb565_resize_rotate_arm.asm
��
����test
    ��  LibVrPresent_Test.sln (Solution files for all test)
    ��
    ����LibVrPresent_Test (Test for rendering YUV)
    ��  ��  LibVrPresent_Test.cpp
    ��  ��  LibVrPresent_Test.h
    ��  ��  LibVrPresent_Test.rc
    ��  ��  LibVrPresent_Test.vcproj
    ��  ��  LibVrPresent_TestDlg.cpp
    ��  ��  LibVrPresent_TestDlg.h
    ��  ��  ReadMe.txt
    ��  ��  resource.h
    ��  ��  stdafx.cpp
    ��  ��  stdafx.h
    ��  ��  WIN32CECONFIG.H
    ��  ��  WIN32CONFIG.h
    ��  ��
    ��  ����res
    ��          LibVrPresent_Test.ico
    ��          LibVrPresent_Test.rc2
    ��
    ����LineQuality_Test (Test for Drawing Line)
    ��  ��  LibVrPresent_Test.rc
    ��  ��  LineQuality_Test.cpp
    ��  ��  LineQuality_Test.h
    ��  ��  LineQuality_Test.vcproj
    ��  ��  LineQuality_TestDlg.cpp
    ��  ��  LineQuality_TestDlg.h
    ��  ��  ReadMe.txt
    ��  ��  resource.h
    ��  ��  stdafx.cpp
    ��  ��  stdafx.h
    ��  ��  WIN32CECONFIG.H
    ��  ��  WIN32CONFIG.h
    ��  ��
    ��  ����res
    ��          LibVrPresent_Test.ico
    ��          LibVrPresent_Test.rc2
    ��
    ����ResolutionChange (Tools for testing overlay)
            ReadMe.txt
            resolutionchange.cpp
            ResolutionChange.h
            ResolutionChange.ico
            ResolutionChange.rc
            ResolutionChange.vcproj
            Resource.h
            small.ico
            stdafx.cpp
            stdafx.h

