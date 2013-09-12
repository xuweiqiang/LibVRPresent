<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: libmp4v2 - Win32 (WCE ARMV4I) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP56D.tmp" with contents
[
/nologo /W3 /Zi /Od /I "./include" /D "DEBUG" /D _WIN32_WCE=500 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_AT4X0A_Wince_SDK_500_1037" /D "ARMV4I" /D UNDER_CE=500 /D "UNICODE" /D "_UNICODE" /D "_LIB" /FR"ARMV4IDbg/" /Fp"ARMV4IDbg/libmp4v2.pch" /YX /Fo"ARMV4IDbg/" /Fd"ARMV4IDbg/" /QRarch4T /QRinterwork-return /MC /c 
"D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4.cpp"
"D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP56D.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP56E.tmp" with contents
[
/nologo /out:"ARMV4IDbg\libmp4v2.lib" 
.\ARMV4IDbg\3gp.obj
.\ARMV4IDbg\atom_amr.obj
.\ARMV4IDbg\atom_avc1.obj
.\ARMV4IDbg\atom_avcC.obj
.\ARMV4IDbg\atom_d263.obj
.\ARMV4IDbg\atom_damr.obj
.\ARMV4IDbg\atom_dref.obj
.\ARMV4IDbg\atom_elst.obj
.\ARMV4IDbg\atom_enca.obj
.\ARMV4IDbg\atom_encv.obj
.\ARMV4IDbg\atom_free.obj
.\ARMV4IDbg\atom_ftyp.obj
.\ARMV4IDbg\atom_hdlr.obj
.\ARMV4IDbg\atom_hinf.obj
.\ARMV4IDbg\atom_hnti.obj
.\ARMV4IDbg\atom_href.obj
.\ARMV4IDbg\atom_mdat.obj
.\ARMV4IDbg\atom_mdhd.obj
.\ARMV4IDbg\atom_meta.obj
.\ARMV4IDbg\atom_mp4s.obj
.\ARMV4IDbg\atom_mp4v.obj
.\ARMV4IDbg\atom_mvhd.obj
.\ARMV4IDbg\atom_ohdr.obj
.\ARMV4IDbg\atom_root.obj
.\ARMV4IDbg\atom_rtp.obj
.\ARMV4IDbg\atom_s263.obj
.\ARMV4IDbg\atom_sdp.obj
.\ARMV4IDbg\atom_smi.obj
.\ARMV4IDbg\atom_sound.obj
.\ARMV4IDbg\atom_standard.obj
.\ARMV4IDbg\atom_stbl.obj
.\ARMV4IDbg\atom_stdp.obj
.\ARMV4IDbg\atom_stsc.obj
.\ARMV4IDbg\atom_stsd.obj
.\ARMV4IDbg\atom_stsz.obj
.\ARMV4IDbg\atom_tfhd.obj
.\ARMV4IDbg\atom_tkhd.obj
.\ARMV4IDbg\atom_treftype.obj
.\ARMV4IDbg\atom_trun.obj
.\ARMV4IDbg\atom_udta.obj
.\ARMV4IDbg\atom_url.obj
.\ARMV4IDbg\atom_urn.obj
.\ARMV4IDbg\atom_video.obj
.\ARMV4IDbg\atom_vmhd.obj
.\ARMV4IDbg\descriptors.obj
.\ARMV4IDbg\isma.obj
.\ARMV4IDbg\mp4.obj
.\ARMV4IDbg\mp4atom.obj
.\ARMV4IDbg\mp4container.obj
.\ARMV4IDbg\mp4descriptor.obj
.\ARMV4IDbg\mp4file.obj
.\ARMV4IDbg\mp4file_io.obj
.\ARMV4IDbg\mp4info.obj
.\ARMV4IDbg\mp4meta.obj
.\ARMV4IDbg\mp4property.obj
.\ARMV4IDbg\mp4track.obj
.\ARMV4IDbg\mp4util.obj
.\ARMV4IDbg\ocidescriptors.obj
.\ARMV4IDbg\odcommands.obj
.\ARMV4IDbg\qosqualifiers.obj
.\ARMV4IDbg\rtphint.obj
]
Creating command line "link.exe -lib @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP56E.tmp"
<h3>Output Window</h3>
Compiling...
mp4.cpp
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4.cpp(49) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4.cpp(694) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4.cpp(862) : warning C4800: 'unsigned char' : forcing value to bool 'true' or 'false' (performance warning)
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4.cpp(1672) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4.cpp(1839) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned short', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4.cpp(1855) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned short', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4.cpp(2392) : error C2039: 'GetNextSyncSample' : is not a member of 'MP4File'
        d:\evc\filters\mp4_splitter\mp4v2\mp4file.h(42) : see declaration of 'MP4File'
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4.cpp(3099) : warning C4244: 'argument' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
mp4file.cpp
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(566) : warning C4530: C++ exception handler used, but unwind semantics are not enabled. Specify -GX
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(710) : warning C4101: 'rc' : unreferenced local variable
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(1383) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned short', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(1859) : warning C4244: 'initializing' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2106) : warning C4244: 'initializing' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2223) : error C2039: 'GetNextSyncSampleId' : is not a member of 'MP4File'
        d:\evc\filters\mp4_splitter\mp4v2\mp4file.h(42) : see declaration of 'MP4File'
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2226) : error C2065: 'm_pTracks' : undeclared identifier
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2226) : error C2065: 'FindTrackIndex' : undeclared identifier
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2226) : error C2109: subscript requires array or pointer type
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2227) : error C2227: left of '->GetNextSyncSample' must point to class/struct/union
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2385) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned char', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2395) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned char', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2405) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned char', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2415) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned char', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2425) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned char', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2517) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned char', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2521) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned char', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2576) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'int', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2656) : warning C4244: '=' : conversion from 'unsigned __int64' to 'unsigned char', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2659) : warning C4244: '=' : conversion from 'unsigned __int64' to 'unsigned char', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2666) : warning C4244: '=' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2693) : warning C4244: 'argument' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2697) : warning C4244: 'argument' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2719) : warning C4244: 'argument' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(2721) : warning C4244: 'argument' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
D:\evc\Filters\MP4_SPLITTER\mp4v2\mp4file.cpp(3165) : warning C4244: 'return' : conversion from 'unsigned __int64' to 'unsigned int', possible loss of data
Error executing clarm.exe.
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP56F.tmp" with contents
[
/nologo /o"ARMV4IDbg/libmp4v2.bsc" 
.\ARMV4IDbg\3gp.sbr
.\ARMV4IDbg\atom_amr.sbr
.\ARMV4IDbg\atom_avc1.sbr
.\ARMV4IDbg\atom_avcC.sbr
.\ARMV4IDbg\atom_d263.sbr
.\ARMV4IDbg\atom_damr.sbr
.\ARMV4IDbg\atom_dref.sbr
.\ARMV4IDbg\atom_elst.sbr
.\ARMV4IDbg\atom_enca.sbr
.\ARMV4IDbg\atom_encv.sbr
.\ARMV4IDbg\atom_free.sbr
.\ARMV4IDbg\atom_ftyp.sbr
.\ARMV4IDbg\atom_hdlr.sbr
.\ARMV4IDbg\atom_hinf.sbr
.\ARMV4IDbg\atom_hnti.sbr
.\ARMV4IDbg\atom_href.sbr
.\ARMV4IDbg\atom_mdat.sbr
.\ARMV4IDbg\atom_mdhd.sbr
.\ARMV4IDbg\atom_meta.sbr
.\ARMV4IDbg\atom_mp4s.sbr
.\ARMV4IDbg\atom_mp4v.sbr
.\ARMV4IDbg\atom_mvhd.sbr
.\ARMV4IDbg\atom_ohdr.sbr
.\ARMV4IDbg\atom_root.sbr
.\ARMV4IDbg\atom_rtp.sbr
.\ARMV4IDbg\atom_s263.sbr
.\ARMV4IDbg\atom_sdp.sbr
.\ARMV4IDbg\atom_smi.sbr
.\ARMV4IDbg\atom_sound.sbr
.\ARMV4IDbg\atom_standard.sbr
.\ARMV4IDbg\atom_stbl.sbr
.\ARMV4IDbg\atom_stdp.sbr
.\ARMV4IDbg\atom_stsc.sbr
.\ARMV4IDbg\atom_stsd.sbr
.\ARMV4IDbg\atom_stsz.sbr
.\ARMV4IDbg\atom_tfhd.sbr
.\ARMV4IDbg\atom_tkhd.sbr
.\ARMV4IDbg\atom_treftype.sbr
.\ARMV4IDbg\atom_trun.sbr
.\ARMV4IDbg\atom_udta.sbr
.\ARMV4IDbg\atom_url.sbr
.\ARMV4IDbg\atom_urn.sbr
.\ARMV4IDbg\atom_video.sbr
.\ARMV4IDbg\atom_vmhd.sbr
.\ARMV4IDbg\descriptors.sbr
.\ARMV4IDbg\isma.sbr
.\ARMV4IDbg\mp4.sbr
.\ARMV4IDbg\mp4atom.sbr
.\ARMV4IDbg\mp4container.sbr
.\ARMV4IDbg\mp4descriptor.sbr
.\ARMV4IDbg\mp4file.sbr
.\ARMV4IDbg\mp4file_io.sbr
.\ARMV4IDbg\mp4info.sbr
.\ARMV4IDbg\mp4meta.sbr
.\ARMV4IDbg\mp4property.sbr
.\ARMV4IDbg\mp4track.sbr
.\ARMV4IDbg\mp4util.sbr
.\ARMV4IDbg\ocidescriptors.sbr
.\ARMV4IDbg\odcommands.sbr
.\ARMV4IDbg\qosqualifiers.sbr
.\ARMV4IDbg\rtphint.sbr]
Creating command line "bscmake.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP56F.tmp"
Creating browse info file...
<h3>Output Window</h3>



<h3>Results</h3>
libmp4v2.lib - 6 error(s), 28 warning(s)
</pre>
</body>
</html>
