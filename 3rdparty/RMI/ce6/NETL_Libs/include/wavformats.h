#ifndef __WAV_FORMAT_TYPES_H__
#define __WAV_FORMAT_TYPES_H__


#ifndef WAVE_FORMAT_PCM
  #define WAVE_FORMAT_UNKNOWN 0x0000  /* Microsoft Corporation Unknown or unspecified format  */
  #define WAVE_FORMAT_PCM 0x0001  /* Microsoft Corporation PCM audio in integer format  */
#endif
#define WAVE_FORMAT_PCM_EXTENDED 0xfffe 
#define WAVE_FORMAT_ADPCM 0x0002  /* Microsoft Corporation Microsoft adaptive PCM  */
#define WAVE_FORMAT_IEEE_FLOAT 0x0003  /* Microsoft Corporation PCM audio in IEEE floating-point format  */
#define WAVE_FORMAT_VSELP 0x0004  /* Compaq Computer Corporation VSELP codec for Windows CE 2.0 device  */
#define WAVE_FORMAT_IBM_CVSD 0x0005  /* IBM Corporation Not specified  */
#define WAVE_FORMAT_ALAW 0x0006  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_MULAW 0x0007  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_DTS 0x0008  /* Microsoft Corporation DTS  */
#define WAVE_FORMAT_DRM 0x0009  /* Microsoft Corporation Microsoft Digital Rights Managed encrypted audio  */
#define WAVE_FORMAT_WMSP1 0x000A  /* Microsoft Corporation Speech audio codec  */
#define WAVE_FORMAT_OKI_ADPCM 0x0010  /* OKI Not specified  */
#define WAVE_FORMAT_DVI_ADPCM 0x0011  /* Intel Corporation Not specified  */
#define WAVE_FORMAT_IMA_ADPCM  (WAVE_FORMAT_DVI_ADPCM) /*  Intel Corporation  */
#define WAVE_FORMAT_MEDIASPACE_ADPCM 0x0012  /* VideoLogic Systems Not specified  */
#define WAVE_FORMAT_SIERRA_ADPCM 0x0013  /* Sierra Semiconductor Corporation Not specified  */
#define WAVE_FORMAT_G723_ADPCM 0x0014  /* Antex Electronics Corporation Not specified  */
#define WAVE_FORMAT_DIGISTD 0x0015  /* DSP Solutions, Inc. Not specified  */
#define WAVE_FORMAT_DIGIFIX 0x0016  /* DSP Solutions, Inc. Not specified  */
#define WAVE_FORMAT_DIALOGIC_OKI_ADPCM 0x0017  /* Dialogic Corporation OKI ADPCM chips or firmware  */
#define WAVE_FORMAT_MEDIAVISION_ADPCM 0x0018  /* Media Vision, Inc. ADPCM for Jazz 16 chip set  */
#define WAVE_FORMAT_CU_CODEC 0x0019  /* Hewlett-Packard Company Not specified  */
#define WAVE_FORMAT_HP_DYNAMIC_VOICE 0x001A  /* Hewlett-Packard Company Not specified  */
#define WAVE_FORMAT_YAMAHA_ADPCM 0x0020  /* Yamaha Corporation of America Not specified  */
#define WAVE_FORMAT_SONARC 0x0021  /* Speech Compression Not specified  */
#define WAVE_FORMAT_DSPGROUP_TRUESPEECH 0x0022  /* DSP Group, Inc. Not specified  */
#define WAVE_FORMAT_ECHOSC1 0x0023  /* Echo Speech Corporation Not specified  */
#define WAVE_FORMAT_AUDIOFILE_AF36 0x0024  /* Ahead, Inc. Not specified  */
#define WAVE_FORMAT_APTX 0x0025  /* Audio Processing Technology Not specified  */
#define WAVE_FORMAT_AUDIOFILE_AF10 0x0026  /* Ahead, Inc. Not specified  */
#define WAVE_FORMAT_PROSODY_1612 0x0027  /* Aculab plc Prosody CTI speech card  */
#define WAVE_FORMAT_LRC 0x0028  /* Merging Technologies S.A. Not specified  */
#define WAVE_FORMAT_DOLBY_AC2 0x0030  /* Dolby Laboratories, Inc. Not specified  */
#define WAVE_FORMAT_GSM610 0x0031  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_MSNAUDIO 0x0032  /* Microsoft Corporation Microsoft MSN audio codec  */
#define WAVE_FORMAT_ANTEX_ADPCME 0x0033  /* Antex Electronics Corporation Not specified  */
#define WAVE_FORMAT_CONTROL_RES_VQLPC 0x0034  /* Control Resources Ltd. Not specified.  */
#define WAVE_FORMAT_DIGIREAL 0x0035  /* DSP Solutions, Inc. Not specified  */
#define WAVE_FORMAT_DIGIADPCM 0x0036  /* DSP Solutions, Inc. Not specified.  */
#define WAVE_FORMAT_CONTROL_RES_CR10 0x0037  /* Control Resources Ltd. Not specified  */
#define WAVE_FORMAT_NMS_VBXADPCM 0x0038  /* Natural MicroSystems Corporation Not specified  */
#define WAVE_FORMAT_ROLAND_RDAC 0x0039  /* Roland Corporation Roland RDAC proprietary format  */
#define WAVE_FORMAT_ECHOSC3 0x003A  /* Echo Speech Corporation Proprietary compression format  */
#define WAVE_FORMAT_ROCKWELL_ADPCM 0x003B  /* Rockwell International Corporation Rockwell ADPCM  */
#define WAVE_FORMAT_ROCKWELL_DIGITALK 0x003C  /* Rockwell International Corporation Rockwell DIGITALK  */
#define WAVE_FORMAT_XEBEC 0x003D  /* Xebec Multimedia Solutions Ltd. Proprietary compression format  */
#define WAVE_FORMAT_G721_ADPCM 0x0040  /* Antex Electronics Corporation Not specified  */
#define WAVE_FORMAT_G728_CELP 0x0041  /* Antex Electronics Corporation Not specified  */
#define WAVE_FORMAT_MSG723 0x0042  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_MSG723_1 0x0043  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_MSG729 0x0044  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_SPG726 0x0045  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_MPEG 0x0050  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_RT24 0x0052  /* InSoft, Inc. Not specified  */
#define WAVE_FORMAT_PAC 0x0053  /* InSoft, Inc. Not specified  */
#define WAVE_FORMAT_MPEGLAYER3 0x0055  /* Not specified. ISO/MPEG Layer 3 format tag  */
#define WAVE_FORMAT_LUCENT_G723 0x0059  /* Lucent Technologies Not specified  */
#define WAVE_FORMAT_CIRRUS 0x0060  /* Cirrus Logic, Inc. Not specified  */
#define WAVE_FORMAT_ESPCM 0x0061  /* ESS Technology, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE 0x0062  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_CANOPUS_ATRAC 0x0063  /* Canopus, Co., Ltd. ATRACWAVEFORMAT  */
#define WAVE_FORMAT_G726_ADPCM 0x0064  /* APICOM Not specified  */
#define WAVE_FORMAT_G722_ADPCM 0x0065  /* APICOM Not specified  */
#define WAVE_FORMAT_DSAT_DISPLAY 0x0067  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_VOXWARE_BYTE_ALIGNED 0x0069  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_AC8 0x0070  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_AC10 0x0071  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_AC16 0x0072  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_AC20 0x0073  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_RT24 0x0074  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_RT29 0x0075  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_RT29HW 0x0076  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_VR12 0x0077  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_VR18 0x0078  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_TQ40 0x0079  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_VOXWARE_SC3 0x007A  /* Voxware, Inc. Not specified  */
//#define WAVE_FORMAT_VOXWARE_SC3 0x007B  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_SOFTSOUND 0x0080  /* SoftSound Ltd. Not specified  */
#define WAVE_FORMAT_VOXWARE_TQ60 0x0081  /* Voxware, Inc. Not specified  */
#define WAVE_FORMAT_MSRT24 0x0082  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_G729A 0x0083  /* AT&T Not specified  */
#define WAVE_FORMAT_MVI_MVI2 0x0084  /* Motion Pixels Ltd. Not specified  */
#define WAVE_FORMAT_DF_G726 0x0085  /* Datafusion Systems (Pty) Ltd. Not specified  */
#define WAVE_FORMAT_DF_GSM610 0x0086  /* Datafusion Systems (Pty) Ltd. Not specified  */
#define WAVE_FORMAT_ISIAUDIO 0x0088  /* Iterated Systems, Inc. Not specified  */
#define WAVE_FORMAT_ONLIVE 0x0089  /* OnLive! Technologies, Inc. Not specified  */
#define WAVE_FORMAT_MULTITUDE_FT_SX20 0x008A  /* Multitude, Inc. Not specified  */
#define WAVE_FORMAT_MM_INFOCOM_ITS_ACM_G721 0x008B  /* Infocom ITS A/S G.721 ADPCM  */
#define WAVE_FORMAT_CONVEDIA_G729 0x008C  /* Convedia Corporation Not specified  */
#define WAVEFORMAT_CONGRUENCY_AUDIO_CODEC 0x008D  /* congruency, Inc. Not specified  */
#define WAVE_FORMAT_SBC24 0x0091  /* Siemens Business Communications Systems Not specified  */
#define WAVE_FORMAT_DOLBY_AC3_SPDIF 0x0092  /* Sonic Foundry Not specified  */
#define WAVE_FORMAT_MEDIASONIC_G723 0x0093  /* MediaSonic Ltd. Not specified  */
#define WAVE_FORMAT_PROSODY_8KBPS 0x0094  /* Aculab plc Prosody CTI speech card  */
#define WAVE_FORMAT_ZYXEL_ADPCM 0x0097  /* ZyXEL Communications Corporation ZyXEL ADPCM  */
#define WAVE_FORMAT_PHILIPS_LPCBB 0x0098  /* Philips Speech Processing Not specified  */
#define WAVE_FORMAT_PACKED 0x0099  /* Studer Professional Audio AG Not specified  */
#define WAVE_FORMAT_MALDEN_PHONYTALK 0x00A0  /* Malden Electronics Ltd. Phony Talk  */
#define WAVE_FORMAT_RRLGSM 0x00A1  /* Racal Recorders Racal Recorder GSM  */
#define WAVE_FORMAT_RRLG729A 0x00A2  /* Racal Recorders Racal Recorder G720.a  */
#define WAVE_FORMAT_RRLG7231 0x00A3  /* Racal Recorders Racal G723.1  */
#define WAVE_FORMAT_RRLTETRA_ACLEP 0x00A4  /* Racal Recorders Racal Tetra ACELP  */
#define WAVE_FORMAT_NEC_AAC 0x00B0  /* NEC Corporation NEC AAC  */
#define WAVE_FORMAT_RHETOREX_ADPCM 0x0100  /* Rhetorex, Inc. Rhetorex ADPCM wave format type  */
#define WAVE_FORMAT_IRAT 0x0101  /* BeCubed Software, Inc. Not specified  */
#define WAVE_FORMAT_VIVO_G723 0x0111  /* Vivo Software, Inc. Not specified  */
#define WAVE_FORMAT_VIVO_SIREN 0x0112  /* Vivo Software, Inc. Not specified  */
#define WAVE_FORMAT_PHILIPS_CELP 0x0120  /* Philips Speech Processing Not specified  */
#define WAVE_FORMAT_PHILIPS_GRUNDIG 0x0121  /* Philips Speech Processing Not specified  */
#define WAVE_FORMAT_DIGITAL_G723 0x0123  /* Digital Equipment Corporation Not specified  */
#define WAVE_FORMAT_SANYO_LD_ADPCM 0x0125  /* SANYO Electric Co., Ltd. SANYO LD-ADPCM wave type  */
#define WAVE_FORMAT_SIPROLAB_ACELPNET 0x0130  /* Sipro Lab Telecom Inc. Not specified  */
#define WAVE_FORMAT_SIPROLAB_ACELP4800 0x0131  /* Sipro Lab Telecom Inc. Not specified  */
#define WAVE_FORMAT_SIPROLAB_ACELP8V3 0x0132  /* Sipro Lab Telecom Inc. Not specified  */
#define WAVE_FORMAT_SIPROLAB_G729 0x0133  /* Sipro Lab Telecom Inc. Not specified  */
#define WAVE_FORMAT_SIPROLAB_G729A 0x0134  /* Sipro Lab Telecom Inc. Not specified  */
#define WAVE_FORMAT_SIPROLAB_KELVIN 0x0135  /* Sipro Lab Telecom Inc. Not specified  */
#define WAVE_FORMAT_VOICEAGE_AMR 0x0136  /* VoiceAge Corporation Not specified  */
#define WAVE_FORMAT_G726ADPCM 0x0140  /* Dictaphone Corporation Not specified  */
#define WAVE_FORMAT_CELP68 0x0141  /* Dictaphone Corporation Not specified  */
#define WAVE_FORMAT_CELP54 0x0142  /* Dictaphone Corporation Not specified  */
#define WAVE_FORMAT_QUALCOMM_PUREVOICE 0x0150  /* QUALCOMM, Inc. Not specified  */
#define WAVE_FORMAT_QUALCOMM_HALFRATE 0x0151  /* QUALCOMM, Inc. Not specified  */
#define WAVE_FORMAT_TUBGSM 0x0155  /* Ring Zero Systems, Inc. Related to GSM 6.10  */
#define WAVE_FORMAT_MSAUDIO1 0x0160  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_MSAUDIO2 0x0161  /* Microsoft Corporation Not specified  */
#define WAVE_FORMAT_MSAUDIO3 0x0162  /* Microsoft Corporation Multichannel WMA  */
#define WAVE_FORMAT_UNISYS_NAP_ADPCM 0x0170  /* Unisys Corporation Not specified  */
#define WAVE_FORMAT_UNISYS_NAP_ULAW 0x0171  /* Unisys Corporation Not specified  */
#define WAVE_FORMAT_UNISYS_NAP_ALAW 0x0172  /* Unisys Corporation Not specified  */
#define WAVE_FORMAT_UNISYS_NAP_16K 0x0173  /* Unisys Corporation Not specified  */
#define WAVE_FORMAT_MM_SYCOM_ACM_SYC008 0X0174  /* SyCom Technologies Not specified  */
#define WAVE_FORMAT_MM_SYCOM_ACM_SYC701_G726L 0x0175  /* SyCom Technologies Not specified  */
#define WAVE_FORMAT_MM_SYCOM_ACM_SYC701_CELP54 0x0176  /* SyCom Technologies Not specified  */
#define WAVE_FORMAT_MM_SYCOM_ACM_SYC701_CELP68 0x0177  /* SyCom Technologies Not specified  */
#define WAVE_FORMAT_KNOWLEDGE_ADVENTURE_ADPCM 0x0178  /* Knowledge Adventure, Inc. Not specified  */
#define WAVE_FORMAT_MPEG2AAC 0x0180  /* Fraunhofer IIS Not specified  */
#define WAVE_FORMAT_DTS_DS 0x0190  /* Digital Theater Systems, Inc. Not specified  */
#define WAVE_FORMAT_INNINGS_ADPCM 0x01979  /* Innings Telecom Inc. Not specified  */
#define WAVE_FORMAT_CREATIVE_ADPCM 0x0200  /* Creative Labs, Inc. Not specified  */
#define WAVE_FORMAT_CREATIVE_FASTSPEECH8 0x0202  /* Creative Labs, Inc. Fast Speech 8  */
#define WAVE_FORMAT_CREATIVE_FASTSPEECH10 0x0203  /* Creative Labs, Inc. Fast Speech 10  */
#define WAVE_FORMAT_UHER_ADPCM 0x0210  /* UHER informatik GmbH Not specified  */
#define WAVE_FORMAT_QUARTERDECK 0x0220  /* Quarterdeck Corporation Not specified  */
#define WAVE_FORMAT_ILINK_VC 0x0230  /* I-Link Worldwide Not specified  */
#define WAVE_FORMAT_RAW_SPORT 0x0240  /* Aureal Semiconductor Inc. Not specified  */
#define WAVE_FORMAT_IPI_HSX 0x0250  /* Interactive Products, Inc. Not specified  */
#define WAVE_FORMAT_IPI_RPELP 0x0251  /* Interactive Products, Inc. Not specified  */
#define WAVE_FORMAT_CS2 0x0260  /* Consistent Software Cs2  */
#define WAVE_FORMAT_SONY_SCX 0x0270  /* Sony Corporation Not specified  */
#define WAVE_FORMAT_SONY_SCY 0x0271  /* Sony Corporation Not specified  */
#define WAVE_FORMAT_SONY_ATRAC3 0x0272  /* Sony Corporation Not specified  */
#define WAVE_FORMAT_SONY_SPC 0x0273  /* Sony Corporation Not specified  */
#define WAVE_FORMAT_TELUM 0x0280  /* Telum Inc. Not specified  */
#define WAVE_FORMAT_TELUMIA 0x0281  /* Telum Inc. Not specified  */
#define WAVE_FORMAT_NCVS_ADPCM 0x0285  /* Norcom Electronics Corporation Norcom Voice Systems ADPCM  */
#define WAVE_FORMAT_FM_TOWNS_SND 0x0300  /* Fujitsu Corporation Not specified  */
//#define Reserved 0x0301  /* Fujitsu Corporation Not specified  */
//#define Reserved 0x0302  /* Fujitsu Corporation Not specified  */
//#define Reserved 0x0303  /* Fujitsu Corporation Not specified  */
//#define Reserved 0x0304  /* Fujitsu Corporation Not specified  */
//#define Reserved 0x0305  /* Fujitsu Corporation Not specified  */
//#define Reserved 0x0306  /* Fujitsu Corporation Not specified  */
//#define Reserved 0x0307  /* Fujitsu Corporation Not specified  */
//#define Reserved 0x0308  /* Fujitsu Corporation Not specified  */
#ifndef WAVE_FORMAT_DEVELOPMENT
  #define WAVE_FORMAT_DEVELOPMENT 0x0350  /* Micronas Semiconductors, Inc. Not specified  */
#endif
#define WAVE_FORMAT_CELP833 0x0351  /* Micronas Semiconductors, Inc. Not specified  */
#define WAVE_FORMAT_BTV_DIGITAL 0x0400  /* Brooktree Corporation Brooktree digital audio format  */
#define WAVE_FORMAT_QDESIGN_MUSIC 0x0450  /* QDesign Corporation Not specified  */
#define WAVE_FORMAT_VME_VMPCM 0x0680  /* AT&T Not specified  */
#define WAVE_FORMAT_TPC 0x0681  /* AT&T Not specified  */
#define WAVE_FORMAT_OLIGSM 0x1000  /* Ing. C. Olivetti & C., S.p.A. Not specified  */
#define WAVE_FORMAT_OLIADPCM 0x1001  /* Ing. C. Olivetti & C., S.p.A. Not specified  */
#define WAVE_FORMAT_OLICELP 0x1002  /* Ing. C. Olivetti & C., S.p.A. Not specified  */
#define WAVE_FORMAT_OLISBC 0x1003  /* Ing. C. Olivetti & C., S.p.A. Not specified  */
#define WAVE_FORMAT_OLIOPR 0x1004  /* Ing. C. Olivetti & C., S.p.A. Not specified  */
#define WAVE_FORMAT_LH_CODEC 0x1100  /* Lernout & Hauspie Not specified  */
#define WAVE_FORMAT_LH_CELP 0X1101  /* Lernout & Hauspie Not specified  */
#define WAVE_FORMAT_LH_SB8 0X1102  /* Lernout & Hauspie Not specified  */
#define WAVE_FORMAT_LH_SB12 0X1103  /* Lernout & Hauspie Not specified  */
#define WAVE_FORMAT_LH_SB16 0X1104  /* Lernout & Hauspie Not specified  */
#define WAVE_FORMAT_NORRIS 0x1400  /* Norris Communications, Inc. Not specified  */
#define WAVE_FORMAT_SOUNDSPACE_MUSICOMPRESS 0x1500  /* AT&T Not specified  */
#ifndef WAVE_FORMAT_WMAUDIO_LOSSLESS  
  #define WAVE_FORMAT_WMAUDIO_LOSSLESS 0x163  /* Microsoft Corporation WMA lossless  */
#endif
#ifndef WAVE_FORMAT_WMASPDIF  
  #define WAVE_FORMAT_WMASPDIF 0x164  /* Microsoft Corporation WMA Pro over S/PDIF  */
#endif
#define WAVE_FORMAT_SONICFOUNDRY_LOSSLESS 0x1971  /* Sonic Foundry Not specified  */
#define WAVE_FORMAT_DVM 0x2000  /* FAST Multimedia AG Not specified  */
#define WAVE_FORMAT_MSCE 0x2500  /* Microsoft Corporation Reserved rangle to 0x2600  */
#define WAVE_FORMAT_DIVIO_AAC 0x4143  /* Divio, Inc. Divio's AAC  */
#define WAVE_FORMAT_NOKIA_AMR 0x4201  /* Nokia Mobile Phones Nokia adaptive multirate  */
#define WAVE_FORMAT_DIVIO_G726 0x4243  /* Divio, Inc. Divio's G726  */
#define WAVE_FORMAT_3COM_NBX 0x7000  /* 3Com Corporation Not specified  */
#define WAVE_FORMAT_NTT_DOCOMO_AMR_NO_SID 0x7A21  /* Microsoft Corporation Adaptive multirate  */
#define WAVE_FORMAT_NTT_DOCOMO_AMR_WITH_SID 0x7A22  /* Microsoft Corporation AMR with silence detection  */
#define WAVE_FORMAT_COMVERSEINFOSYS_G723_1 0xA100  /* Comverse Infosys Ltd. Not specified  */
#define WAVE_FORMAT_COMVERSEINFOSYS_AVQSBC 0xA101  /* Comverse Infosys Ltd. Not specified  */
#define WAVE_FORMAT_COMVERSEINFOSYS_OLDSBC 0xA102  /* Comverse Infosys Ltd. Not specified  */
#define WAVE_FORMAT_SYMBOLTECH_G729A 0xA103  /* Symbol Technologies Canada Symbol Technology's G729A  */
#define WAVE_FORMAT_VOICEAGE_AMR_WB 0xA104  /* VoiceAge Corporation Not specified  */
#define WAVE_FORMAT_ITI_G726 0xA105  /* Ingenient Technologies, Inc. Ingenient's G726  */
#ifndef WAVE_FORMAT_AAC  
  #define WAVE_FORMAT_AAC 0x00FF  /* Not specified. */ 
#endif 
#define WAVE_FORMAT_AAC1 0xA106  /* Not specified. ISO/MPEG-4 advanced audio Coding  */ 
#define WAVE_FORMAT_ESLG726 0xA107  /* Encore Software Ltd. Encore Software Ltd's G726  */
#define WAVE_FORMAT_WMSP2 0xb  /* Microsoft Corporation Windows Media RT Voice  */

#endif
