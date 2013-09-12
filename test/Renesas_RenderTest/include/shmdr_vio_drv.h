/******************************************************************************
	File:			shmdr_vio_drv.h
	Product:		SH7343 Device Driver(VIO Device Driver)
	Part Number:	R0M7343DDSTD1STC

	Copyright(C) 2003-2005 Renesas Technology Corp. All Rights Reserved.

	Revision List(SH73180):
		v0.01	03-12-18	created
		v0.11	04-02-25	latest version correspondent and changed
		v0.12	04-03-04	shmdr_vio_NtscChangeLcdc deleted
		v0.12a	04-08-04	shmdr_vio_Init added
		v0.14a	04-08-06	comment changed
	Revision List(SH73380):
		v0.01	04-11-25	structure definition changed
				04-12-10	prototype changed
		v0.04	05-02-23	VIO_VEU_BUNDLE status added
		v0.05	05-03-01	vou memory in added
				05-03-17	T_VIO_MULTI_PAR: shoffset svoffset added
		v0.06	05-04-14	T_VIO_VEU_PAR: hfilc vfilc changed
				05-04-14	VIO_PAR_CHECK_MODE changed
				05-04-22	shmdr_vio_NtscChangeLcdc: parameter changed
				05-04-27	VIO_CEU_04QW: added
				05-05-11	VIO_CMPLX_FRAME: value changed
	Revision List(SH7343):
		v0.01	2005-06-07	copied
		v0.03	2005-09-01	BEU-VOU multi windows added
							shmdr_vio_NtscSetMulti added
		v0.04	2005-11-17	VIO_CMPLX_8LINE,VIO_CMPLX_16LINE added
							VIO_INT_DCOF added
							VIO_RGB_13_IN added
******************************************************************************/

#ifndef _SHMDR_VIO_DRV_H
#define _SHMDR_VIO_DRV_H



#define VIO_PAR_CHECK_MODE			1			// VIO parameter check mode SW
												//(1:parameter error check valid )


/*-----------------------  Data definition  --------------------------------*/
// RETURN VALUE
#define		VIO_PASS				(H)1		// forced stop process no
#define		VIO_CHANGE				(H)2		// NTSC output frame change notice
#undef EV_ERR
#define		EV_ERR					(H)-99		// abnormal end
#define		EV_ABORT				(H)-101		// forced stop
#define		EV_NTSC_USED			(H)-102		// NTSC right to use get
#define		EV_RETRY_CONT			(H)-103		// NTSC restart process continuous notice
#define		EV_RETRY_END			(H)-104		// NTSC restart end notice
#define		EV_TIMING_ERR			(H)-105		// set timing wrong
#define		EV_VOU_USED				(H)-106		// VOU right to use get

// VIO_lib_ReqVio
// VIO_lib_FreeVio
#define		VIO_REQ_VEU				(H)0		// VEU right to use
#define		VIO_REQ_CEU				(H)1		// CEU right to use
#define		VIO_REQ_BEU				(H)2		// BEU right to use
#define		VIO_REQ_NTSC			(H)3		// NTSC right to use
#define		VIO_REQ_VOU				(H)4		// VOU right to use
#define 	VIO_REQ_VEU2H1			(H)5		// VEU2H1 right to use	by JH.Jang
// ----------------------------------------------------------------------------
// VIO_lib_CeuCheckStatus
#define		VIO_CEU_STOP			0			// CEU stop
#define		VIO_CEU_RUN				(H)1		// CEU run and capture stop
#define		VIO_CAP_RUN				(H)2		// CEU run and capture

// VIO_lib_CeuSetCam
#define		VIO_CEU_08IN			(UB)0x00	// 8 bit image input pin
#define		VIO_CEU_16IN			(UB)0x01	// 16 bit image input pin

#define		VIO_CEU_UYVY			(UB)0x00	// argument dtary set value
#define		VIO_CEU_VYUY			(UB)0x01	// argument dtary set value
#define		VIO_CEU_YUYV			(UB)0x02	// argument dtary set value
#define		VIO_CEU_YVYU			(UB)0x03	// argument dtary set value

#define		VIO_CEU_VD_H			(UB)0x00	// VD High active
#define		VIO_CEU_VD_L			(UB)0x02	// VD Low active
#define		VIO_CEU_HD_H			(UB)0x00	// HD High active
#define		VIO_CEU_HD_L			(UB)0x01	// HD Low active

#define		VIO_CEU_VEW				(UB)0x00	// image get mode
#define		VIO_CEU_JPG				(UB)0x01	// data get mode

#define		VIO_CEU_04LW			(UB)0x00	// 4 LW unit BUS transfer
#define		VIO_CEU_08LW			(UB)0x01	// 8 LW unit BUS transfer
#define		VIO_CEU_16LW			(UB)0x02	// 16 LW unit BUS transfer
#define		VIO_CEU_32LW			(UB)0x03	// 32 LW unit BUS transfer
#define		VIO_CEU_04QW			(UB)0x00	// 4 QW unit BUS transfer
#define		VIO_CEU_08QW			(UB)0x01	// 8 QW unit BUS transfer
#define		VIO_CEU_16QW			(UB)0x02	// 16 QW unit BUS transfer
#define		VIO_CEU_32QW			(UB)0x03	// 32 QW unit BUS transfer

#define		VIO_CEU_PIXDL0			(UB)0x00	// horizontal direction offset cycle number 0
#define		VIO_CEU_PIXDL1			(UB)0x01	// horizontal direction offset cycle number 1
#define		VIO_CEU_PIXDL2			(UB)0x02	// horizontal direction offset cycle number 2
#define		VIO_CEU_PIXDL3			(UB)0x03	// horizontal direction offset cycle number 3

#define		VIO_CEU_SWAP_NO			(UB)0x00	// data swap no
#define		VIO_CEU_SWAP_B			(UB)0x01	// output data byte unit swap
#define		VIO_CEU_SWAP_W			(UB)0x02	// output data word unit swap
#define		VIO_CEU_SWAP_L			(UB)0x04	// output data long word unit swap

// VIO_lib_CeuSetCap
#define		VIO_PROGRESS			(UB)0x00	// progressive capture
#define		VIO_INTERLACE			(UB)0x01	// interlace capture

#define		VIO_CEU_HI_TOP			(UB)0x00	// high field is top field
#define		VIO_CEU_LO_TOP			(UB)0x01	// low field is top field

#define		VIO_CEU_BOTH_SIDE		(UB)0x00	// frame image or both side field image
#define		VIO_CEU_ONE_SIDE		(UB)0x01	// one side field image

#define		VIO_CEU_BOTH_FLD		(UB)0x00	// capture start just after start
#define		VIO_CEU_TOP_FLD			(UB)0x01	// capture start from top field image
#define		VIO_CEU_BTM_FLD			(UB)0x02	// capture start from bottom field image

#define		VIO_CEU_YUV420			(UB)0x00	// YUV420 capture
#define		VIO_CEU_YUV422			(UB)0x01	// YUV422 capture

#define		VIO_CEU_NO				(UB)0x00	// low pass filter apply no
#define		VIO_CEU_LPF				(UB)0x01	// low pass filter apply

#define		VIO_NORMAL				(UB)0x00	// normal write
#define		VIO_BUNDLE				(UB)0x01	// bundle write

// VIO_lib_CeuStart
#define		VIO_CEU_ONECAP			(B)0		// single frame capture
#define		VIO_CEU_SEQCAP			(B)1		// sequential frame capture

// VIO_lib_CeuStop
#define		VIO_CEU_ABORT			(B)1		// forced stop assign

// shdmr_vio_CeuSetCmplx
#define		VIO_NO_CMPLX			(UB)0x00	// complex info no
#define		VIO_CMPLX				(UB)0x01	// complex info get

#define		VIO_CMPLX_FRAME			(UB)0x01	// complex info get by frame
#define		VIO_CMPLX_8LINE			(UB)0x02	// complex info get by 8 lines
#define		VIO_CMPLX_16LINE		(UB)0x03	// complex info get by 16 lines

// interrupt
#define		VIO_INT_NVD				(UB)25		// non VD interrupt
#define		VIO_INT_NHD				(UB)24		// non HD interrupt
#define		VIO_INT_DCOF			(UB)22		// DCOF write interrupt
#define		VIO_INT_VBP				(UB)20		// VBP short interrupt
#define		VIO_INT_IGVS			(UB)18		// VD cycle error IGVS interrupt
#define		VIO_INT_IGHS			(UB)17		// VH cycle error IGHS interrupt
#define		VIO_INT_CDTOF			(UB)16		// data overflow CDTOF interrupt
#define		VIO_INT_CPBE4			(UB)15		// bundle write CPBE4 interrupt
#define		VIO_INT_CPBE3			(UB)14		// bundle write CPBE3 interrupt
#define		VIO_INT_CPBE2			(UB)13		// bundle write CPBE2 interrupt
#define		VIO_INT_CPBE1			(UB)12		// bundle write CPBE1 interrupt
#define		VIO_INT_VD				(UB)9		// VD input interrupt
#define		VIO_INT_HD				(UB)8		// HD input interrupt
#define		VIO_INT_CFE				(UB)1		// field capture end CFE interrupt
#define		VIO_INT_CPE				(UB)0		// frame capture end CPE interrupt


// VIO_lib_StrbSet
#define		VIO_STRB_MANUAL			(B)0x00		// strobe manual mode select
#define		VIO_STRB_AUTO1			(B)0x01		// strobe auto mode 1 select
#define		VIO_STRB_AUTO2			(B)0x02		// strobe auto mode 2 select
#define		VIO_STRB_AUTO3			(B)0x03		// strobe auto mode 3 select

#define		VIO_MAX_STCNT			(UH)0x3FFF	// maximum line number
#define		VIO_NO_STCNT			(UH)0x0000	// line number 0

#define		VIO_MAX_SLCNT			(UH)0x3FFF	// maximum line number
#define		VIO_NO_SLCNT			(UH)0x0000	// line number 0

#define		VIO_POL_HI				(B)0x00		// pole is rising edge
#define		VIO_POL_LO				(B)0x01		// pole is falling edge

// VIO_lib_StrbCtrl
#define		VIO_STRB_OFF			(B)0x00		// strobe flash stop
#define		VIO_STRB_ON				(B)0x01		// strobe flash start

// shmdr_vio_CeuRegCtrl
#define		VIO_REG_2VD				(B)0x00		// register change by 2 vertical deflection
#define		VIO_REG_1VD				(B)0x01		// register change by 1 vertical deflection

#define		VIO_REG_A				(B)0x00		// register A select
#define		VIO_REG_B				(B)0x01		// register B select
#define		VIO_REG_MRR				(B)0x02		// mirror register select

#define		VIO_REG_FIX				(B)0x00		// register fix
#define		VIO_REG_CHANGE			(B)0x01		// register change

// ----------------------------------------------------------------------------
// VIO_lib_VeuCheckStatus
#define		VIO_VEU_STOP			(H)0		// VEU stop
#define		VIO_VEU_RUN				(H)1		// VEU run
#define		VIO_VEU_BUNDLE			(H)2		// VEU bundle

// YUV format
#define		VIO_YUV420				(UH)0x0000	// YUV420 format
#define		VIO_YUV422				(UH)0x4000	// YUV422 format
#define		VIO_YUV444				(UH)0x8000	// YUV444 format

// RGB input format
#define		VIO_RGB_00_IN			(UH)0x0004	// NO.0
#define		VIO_RGB_01_IN			(UH)0x0104	// NO.1
#define		VIO_RGB_02_IN			(UH)0x0203	// NO.2
#define		VIO_RGB_03_IN			(UH)0x0302	// NO.3
#define		VIO_RGB_07_IN			(UH)0x0704	// NO.7
#define		VIO_RGB_08_IN			(UH)0x0802	// NO.8
#define		VIO_RGB_09_IN			(UH)0x0903	// NO.9
#define		VIO_RGB_10_IN			(UH)0x0A03	// NO.10
#define		VIO_RGB_11_IN			(UH)0x0B03	// NO.11
#define		VIO_RGB_12_IN			(UH)0x0C04	// NO.12
#define		VIO_RGB_13_IN			(UH)0x0D04	// NO.13

// RGB output format
#define		VIO_RGB_00_OUT			(UH)0x0001	// NO.0
#define		VIO_RGB_01_OUT			(UH)0x0102	// NO.1
#define		VIO_RGB_02_OUT			(UH)0x0202	// NO.2
#define		VIO_RGB_06_OUT			(UH)0x0602	// NO.6
#define		VIO_RGB_08_OUT			(UH)0x0804	// NO.8
#define		VIO_RGB_10_OUT			(UH)0x0A04	// NO.10
#define		VIO_RGB_13_OUT			(UH)0x0D04	// NO.13
#define		VIO_RGB_14_OUT			(UH)0x0E04	// NO.14
#define		VIO_RGB_16_OUT			(UH)0x1003	// NO.16
#define		VIO_RGB_17_OUT			(UH)0x1103	// NO.17
#define		VIO_RGB_18_OUT			(UH)0x1203	// NO.18
#define		VIO_RGB_19_OUT			(UH)0x1304	// NO.19
#define		VIO_RGB_20_OUT			(UH)0x1404	// NO.20
#define		VIO_RGB_21_OUT			(UH)0x1503	// NO.21
#define		VIO_RGB_22_OUT			(UH)0x1604	// NO.22
#define		VIO_RGB_23_OUT			(UH)0x1704	// NO.23

// VIO_lib_VeuSetPar
#define		VIO_RATIO_8_1			(UH)0x0200	// * 8/1
#define		VIO_RATIO_4_1			(UH)0x0400	// * 4/1
#define		VIO_RATIO_2_1			(UH)0x0800	// * 2/1
#define		VIO_RATIO_3_2			(UH)0x0AA8	// * 3/2
#define		VIO_RATIO_1_1			(UH)0x0000	// * 1/1
#define		VIO_RATIO_7_8			(UH)0x1248	// * 7/8
#define		VIO_RATIO_3_4			(UH)0x1550	// * 3/4
#define		VIO_RATIO_5_8			(UH)0x1998	// * 5/8
#define		VIO_RATIO_1_2			(UH)0x2000	// * 1/2
#define		VIO_RATIO_3_8			(UH)0x2AA8	// * 3/8
#define		VIO_RATIO_1_3			(UH)0x3000	// * 1/3
#define		VIO_RATIO_1_4			(UH)0x4000	// * 1/4
#define		VIO_RATIO_1_5			(UH)0x5000	// * 1/5
#define		VIO_RATIO_1_6			(UH)0x6000	// * 1/6
#define		VIO_RATIO_1_7			(UH)0x7000	// * 1/7
#define		VIO_RATIO_1_8			(UH)0x8000	// * 1/8
#define		VIO_RATIO_1_16			(UH)0xFFF8	// * 1/16

// VIO_lib_VeuSetPar
#define		VIO_ITU_COLOR			(UB)0x00	// ITU color change
#define		VIO_FULL_COLOR			(UB)0x01	// 8 bit full scale color change

#define		VIO_MRR_0				(UB)0x00	// VEU mirror no
#define		VIO_MRR_V				(UB)0x20	// VEU up and down mirror
#define		VIO_MRR_H				(UB)0x10	// VEU right and left mirror
#define		VIO_MRR_HV				(UB)0x30	// VEU right and left up and down mirror
#define		VIO_ROT_0				(UB)0x00	// VEU rotate no
#define		VIO_ROT_90				(UB)0x01	// VEU 90 degrees rotate
#define		VIO_ROT_180				(UB)0x30	// VEU 180 degrees rotate
#define		VIO_ROT_270				(UB)0x02	// VEU 270 degrees rotate

#define		VIO_VEU_FLT_NO			(UB)0x00	// filter apply no
#define		VIO_VEU_LPF				(UB)0x01	// low pass filter apply
#define		VIO_VEU_DBLK			(UB)0x02	// deblocking filter apply
#define		VIO_VEU_MED				(UB)0x04	// median filter apply
#define		VIO_VEU_ENH_H			(UB)0x10	// horizontal enhanser filter apply
#define		VIO_VEU_ENH_V			(UB)0x20	// vertical enhanser filter apply
#define		VIO_VEU_ENH_HV			(UB)0x30	// horizontal and vertical enhanser filter apply

#define		VIO_VEU_NO_APCON		(UB)0x00	// appoint color control off
#define		VIO_VEU_APCON			(UB)0x01	// appoint color control on

#define		VIO_ENH_MAX				(UB)0x01	// enhance max scale
#define		VIO_ENH_MID				(UB)0x02	// enhance middle scale
#define		VIO_ENH_MIN				(UB)0x03	// enhance min scale

#define		VIO_LPF_3TAP			(UB)0x00	// low pass filter with 3 tap
#define		VIO_LPF_5TAP			(UB)0x01	// low pass filter with 5 tap

#define		VIO_NO_DITHER			(UB)0x00	// dither apply not
#define		VIO_DITHER				(UB)0x01	// RGB color reduction process dither apply

#define		VIO_SWAP_NO				(UB)0x00	// data swap no
#define		VIO_SWAP_B_IN			(UB)0x01	// input data byte unit swap
#define		VIO_SWAP_W_IN			(UB)0x02	// input data word unit swap
#define		VIO_SWAP_L_IN			(UB)0x04	// input data long word unit swap
#define		VIO_SWAP_B_OUT			(UB)0x10	// output data byte unit swap
#define		VIO_SWAP_W_OUT			(UB)0x20	// output data word unit swap
#define		VIO_SWAP_L_OUT			(UB)0x40	// output data long word unit swap

#define		VIO_ADDR_INC			(UB)0x00	// output data address increment
#define		VIO_ADDR_FIX			(UB)0x01	// output data address fix

// ----------------------------------------------------------------------------
// VIO_lib_BeuCheckStatus
#define		VIO_BEU_STOP			(H)0		// BEU stop
#define		VIO_BEU_RUN				(H)1		// BEU run

// VIO_lib_BeuSetOsdLut
#define		VIO_MAX_LUTSIZE			256			// color number maximum value
#define		VIO_MIN_LUTSIZE			1			// color number minimum value

// VIO_lib_BeuSetLayer
#define		VIO_BEU_LAYER1			(UB)0x01	// layer 1
#define		VIO_BEU_LAYER2			(UB)0x02	// layer 2
#define		VIO_BEU_LAYER3			(UB)0x04	// layer 3
#define		VIO_BEU_LAYER12			(UB)0x03	// layer 1 and 2
#define		VIO_BEU_LAYER23			(UB)0x06	// layer 2 and 3
#define		VIO_BEU_LAYER31			(UB)0x05	// layer 3 and 1
#define		VIO_BEU_LAYER123		(UB)0x07	// layer 1 and 2 and 3

#define		VIO_BEU_NO_OSD			(UB)0x00	// OSD assign no
#define		VIO_BEU_OSD				(UB)0x01	// OSD assign set
#define		VIO_BEU_OSD_CLUT		(UB)0x03	// OSD assign CLUT use set

#define		VIO_BEU_L_ALPHA			(UB)0x00	// layer alpha blend assign set
#define		VIO_BEU_P_ALPHA			(UB)0x01	// pixel alpha blend assign set
#define		VIO_BEU_R_ALPHA			(UB)0x01	// RGB565 alpha blend assign set
#define		VIO_BEU_NO_ALPHA		(UB)0x00	// RGB565 alpha blend assign no

// shmdr_vio_BeuSetMulti
#define		VIO_BEU_MULTI_NO		(UB)0x00	// no
#define		VIO_BEU_MULTI1			(UB)0x01	// multi window 1
#define		VIO_BEU_MULTI2			(UB)0x02	// multi window 2
#define		VIO_BEU_MULTI3			(UB)0x04	// multi window 3
#define		VIO_BEU_MULTI4			(UB)0x08	// multi window 4

#define		VIO_MULTI				(UB)0x00	// multi window select
#define		VIO_BLEND_0x01			(UB)0x01	// blend layer select

#define		VIO_NO_PENETRA			(UB)0x00	// multi window not penetrate
#define		VIO_PENETRA				(UB)0x01	// multi window penetrate

// VIO_lib_BeuSetPar
#define		VIO_BEU_ORDER321		(UB)0x00	// layer front 321 order
#define		VIO_BEU_ORDER231		(UB)0x01	// layer front 231 order
#define		VIO_BEU_ORDER312		(UB)0x02	// layer front 312 order
#define		VIO_BEU_ORDER132		(UB)0x03	// layer front 132 order
#define		VIO_BEU_ORDER213		(UB)0x04	// layer front 213 order
#define		VIO_BEU_ORDER123		(UB)0x05	// layer front 123 order

#define		VIO_BEU_PAD_P			(UB)0x00	// output data PAD part is BPKFR_PAD
#define		VIO_BEU_PAD_IN			(UB)0x01	// output data PAD part is input alpha value

#define		VIO_BEU_MEM				(UB)0x01	// memory output mode
#define		VIO_BEU_VOU				(UB)0x02	// VOU output mode
#define		VIO_BEU_MEM_VOU			(UB)0x03	// memory and VOU output mode
#define		VIO_BEU_LCD				(UB)0x04	// LCD output mode
#define		VIO_BEU_MEM_LCD			(UB)0x05	// memory and LCD output mode

#define		VIO_BEU_ADDR_SET		(UB)0x00	// address set by each field
#define		VIO_BEU_ADDR_AUTO		(UB)0x01	// address auto set

#define		VIO_BEU_FULL_LINE		(UB)0x00	// output full line
#define		VIO_BEU_SKIP_LINE		(UB)0x01	// output odd line

#define		VIO_BLEND				(UB)0x00	// blend operation
#define		VIO_LUSTER				(UB)0x01	// luster operation

#define		VIO_LUST_ADD			(UB)0x00	// add luster operation
#define		VIO_LUST_REPLACE		(UB)0x01	// replace luster operation
#define		VIO_LUST_SUB			(UB)0x02	// subtract luster operation

// ----------------------------------------------------------------------------
// VIO_lib_NtscCheckStatus
#define		VIO_NTSC_STOP			0			// NTSC stop
#define		VIO_NTSC_RUN			(H)1		// NTSC run
#define		VIO_NTSC2_STOP			(H)2		// NTSC2 stop
#define		VIO_NTSC2_RUN			(H)3		// NTSC2 run

// VIO_lib_NtscEnable
#define		VIO_NTSC_CLKP			(UB)0x00	// pixel clock rising edge synchronize
#define		VIO_NTSC_CLKN			(UB)0x01	// pixel clock falling edge synchronize

#define		VIO_NTSC_HD_H			(UB)0x00	// HD High active
#define		VIO_NTSC_HD_L			(UB)0x02	// HD Low active
#define		VIO_NTSC_VD_H			(UB)0x00	// VD High active
#define		VIO_NTSC_VD_L			(UB)0x01	// VD Low active

#define		VIO_NTSC_16IF			(UB)0x00	// 16 bit interface
#define		VIO_NTSC_08IF			(UB)0x01	// 8 bit interface
#define		VIO_NTSC_08IF_REC656	(UB)0x03	// 8 bit interface (Rec656)

#define		VIO_NTSC_HD_O			(UB)0x00	// HSYNC normal output
#define		VIO_NTSC_VDH_O			(UB)0x01	// valid data period only HSYNC output
#define		VIO_NTSC_VDL_O			(UB)0x02	// VSYNC negate period only HSYNC output

// shmdr_vio_NtscSetSrc
#define		VIO_NTSC_FIELD			(UB)0x00	// operate by field
#define		VIO_NTSC_FRAME			(UB)0x01	// operate by frame

#define		VIO_NTSC_BEU			(B)0x00		// beu input
#define		VIO_NTSC_MEMORY			(B)0x01		// memory input

// VIO_lib_NtscSetPar
#define		VIO_NTSC_HRATIO_1_1		(UH)0x0000	// horizontal direction same size
#define		VIO_NTSC_HRATIO_9_8		(UH)0x8000	// horizontal direction 1.125 times
#define		VIO_NTSC_HRATIO_9_4		(UH)0x8010	// horizontal direction 2.25 times
#define		VIO_NTSC_HRATIO_2_1		(UH)0x8020	// horizontal direction 2 times
#define		VIO_NTSC_HRATIO_4_1		(UH)0x8030	// horizontal direction 4 times

#define		VIO_NTSC_VRATIO_1_1		(UH)0x0000	// vertical direction same size
#define		VIO_NTSC_VRATIO_2_1		(UH)0x4000	// vertical direction 2 times size
#define		VIO_NTSC_VRATIO_4_1		(UH)0x4001	// vertical direction 4 times size

#define		VIO_NTSC_IMAGE			(UB)0x00	// color bar output no
#define		VIO_NTSC_COLOR_BAR		(UB)0x01	// color bar output set

// interrupt
#define		VIO_INT_FIELD_END		(W)17		// field end interrupt
#define		VIO_INT_VSYNC_FIELD		(W)16		// vsync field interrupt
#define		VIO_INT_VSYNC_FRAME		(W)2		// vsync frame interrupt

// VIO_lib_NtscStop
#define		VIO_NTSC_ABORT			(B)0x01		// forced stop

// VIO_lib_NtscChangeLcdc
#define		VIO_NTSC_MODE			(B)0x00		// NTSC mode select
#define		VIO_LCDC_MODE			(B)0x01		// LCDC mode select

#define		VIO_NTSC_LCDC_VSYNC		(UB)0x00	// HSYNC assert synchronize VSYNC select
#define		VIO_NTSC_SAME_VSYNC		(UB)0x01	// VSYNC mode same as NTSC selected

#define		VIO_NTSC_HSYNC_FIX		(UB)0x00	// VSYNC set with HSYNC assert synchronize
#define		VIO_NTSC_HSYNC_SET		(UB)0x01	// VSYNC mode same as NTSC selected

// VIO_lib_NtscClbCtrl
#define		VIO_CLB_OFF				(B)0x00		// color bar output stop
#define		VIO_CLB_ON				(B)0x01		// color bar output start


/*-----------------------  structure  -------------------------------------*/

// CEU camera I/F set
typedef struct {
	UB	fldpol;			// capture field id select
	UB	dtif;			// digital image input pin select
	UB	dtary;			// camera input YUV order assign
	UB	jpg;			// image get or data get assign
	UB	vdhpol;			// camera synchronized signal polarity assign
	UB	fdrp;			// frame drop number assign
	UB	mtcm;			// M-BUS transfer unit assign
	UB	ifs;			// capture input method select
	UB	cim;			// capture image select
	UB	fci;			// capture start image select
} T_VIO_CEU_CAM_IF;

// CEU input image set
typedef struct {
	UH	hoffset;		// camera input horizontal offset
	UH	voffset;		// camera input vertical offset
	UH	hsize;			// camera capture horizontal size
	UH	vsize;			// camera capture vertical size
} T_VIO_CEU_IN_PAR;

// CEU output image set
typedef struct {
	UH	dhratio;		// camera capture image horizontal reduce ratio assign
	UH	dvratio;		// camera capture image vertical reduce ratio assign
	UH	dhsize;			// output horizontal clip size
	UH	dvsize;			// output vertical clip size
	UB	cds;			// output YUV422/YUV420 assign
	UB	lpf;			// low pass filter assign
	UB	cbe;			// bundle write select
	UB	cafix;			// output address fix mode assign
	UB	cswap;			// output data swap assign
} T_VIO_CEU_OUT_PAR;

// CEU output buffer set
typedef struct {
	UB	*dbaddry;		// Y component output buffer address
	UB	*dbaddrc;		// C component output buffer address
	UH	dbhsize;		// output buffer horizontal size
	UH	dhoffset;		// output buffer horizontal offset
	UH	dvoffset;		// output buffer vertical offset
	UB	*dbaddrby;		// bottom field y component output buffer address
	UB	*dbaddrbc;		// bottom field c component output buffer address
	UH	dbvsize2;		// bundle write memory output line
	UB	*dbaddray2;		// bundle write y component output buffer address
	UB	*dbaddrac2;		// bundle write c component output buffer address
	UB	*dbaddrby2;		// bundle write bottom field y component output buffer address
	UB	*dbaddrbc2;		// bundle write bottom field c component output buffer address
} T_VIO_CEU_OUT_BUF;

// ----------------------------------------------------------------------------
// VEU input image set
typedef struct {
	UB	*sbaddry;		// input buffer Y/RGB address
	UB	*sbaddrc;		// input buffer C address
	UH	sbhsize;		// input buffer horizontal size
	UH	shoffset;		// input buffer horizontal offset
	UH	svoffset;		// input buffer vertical offset
	UH	svhsize;		// input image horizontal size
	UH	svvsize;		// input image vertical size
	UH	ryform;			// input RGB/YUV format assign
	UW	veuchn;			// input using VEU2H channel for converting image	by JH.jang
} T_VIO_VEU_IN_PAR;

// VEU output image set
typedef struct {
	UB	*dbaddry;		// output buffer Y/RGB address
	UB	*dbaddrc;		// output buffer C address
	UH	dbhsize;		// output buffer horizontal size
	UH	dhoffset;		// output buffer horizontal offset
	UH	dvoffset;		// output buffer vertical offset
	UH	ryform;			// output RGB/YUV format assign
	UB	pad;			// output RGB data PAD set
} T_VIO_VEU_OUT_PAR;

// VEU process set
typedef struct {
	UH	dhratio;		// horizontal expand/reduct magnification assign
	UH	dvratio;		// vertical expand/reduct magnification assign
	UH	dvhsize;		// output clip size (horizontal )
	UH	dvvsize;		// output clip size (vertical )
	UB	vbe;			// bundle read mode
	UH	vbss;			// bundle source size
	UB	dith;			// dither applied or not assign
	UB	tm;				// color transform mode
	UB	fil;			// filter assign
	UB	enscl;			// enhance scale
	UB	tpn;			// low pass filter tap count
	UW	hfilc;			// horizontal filter factor assign
	UW	vfilc;			// vertical filter factor assign
	UB	mirror;			// mirror assign
	UB	rotation;		// rotate assign
	UB	apcon;			// appoint color control
	UW	apcolor;		// appoint color
	UW	chgcolor;		// change color
	UB	vafix;			// VEM output address fix mode assign
	UB	vswap;			// VEM input output data swap assign
	UW	veuchn;			// VEU2H channel	by JH.jang
} T_VIO_VEU_PAR;

// ----------------------------------------------------------------------------
// input layer set
typedef struct {
	UB	*sbaddry;		// input buffer Y/RGB address
	UB	*sbaddrc;		// input buffer C address
	UB	*sbaddra;		// input buffer alpha plane address
	UH	sbhsize;		// input buffer horizontal size
	UH	shoffset;		// input buffer horizontal offset
	UH	svoffset;		// input buffer vertical offset
	UH	svhsize;		// input image horizontal size
	UH	svvsize;		// input image vertical size
	UH	chlc;			// horizontal blend offset
	UH	cvlc;			// vertical blend offset
	UH	ryform;			// input RGB/YUV format assign
	UB	amux;			// alpha value layer/pixel unit select
	UB	ap;				// layer unit alpha value assign
	UB	dith;			// dither applied or not assign
	UB	in1tm;			// full scale color transform
} T_VIO_LAYER_PAR;

// GRAPHIC display set
typedef struct {
	UB	sape1;			// argument rgyb1 alpha blend assign or not
	UW	aprgyb1;		// alpha blend assign color
	UB	sape2;			// argument rgyb2 alpha blend assign or not
	UW	aprgyb2;		// alpha blend assign color
	UH	tphs;			// OSD image horizontal tile pattern
	UH	tpvs;			// OSD image vertical tile pattern
	UB	mod;			// OSD image assign
} T_VIO_GRAPHIC_PAR;

// OSD image assign
typedef struct {
	UH	tphs;			// OSD image horizontal tile pattern
	UH	tpvs;			// OSD image vertical tile pattern
	UB	mod;			// OSD image assign
} T_VIO_OSD_PAR;

// multi window input layer set
typedef struct {
	UB	*sbaddry;		// input buffer Y/RGB address
	UB	*sbaddrc;		// input buffer C address
	UH	sbhsize;		// input buffer horizontal size
	UH	shoffset;		// input buffer horizontal offset
	UH	svoffset;		// input buffer vertical offset
	UH	svhsize;		// input image horizontal size
	UH	svvsize;		// input image vertical size
	UH	chlc;			// horizontal multi window offset
	UH	cvlc;			// vertical multi window offset
	UH	ryform;			// input RGB/YUV format assign
} T_VIO_MULTI_PAR;

// multi window penetrate set
typedef struct {
	UB	spic;			// multi window or blend window select
	UB	sapem1;			// argument rgyb1 alpha blend assign or not
	UW	rgyb1;			// alpha blend assign color
	UB	sapem2;			// argument rgyb2 alpha blend assign or not
	UW	rgyb2;			// alpha blend assign color
} T_VIO_PENETRA_PAR;

// layer order assign
typedef struct {
	UB	luse;			// using layer assign
	UB	lay;			// input1,2,3 layer order assign
	UB	pwd;			// parent screen assign
	UB	dataswap;		// input data swap assign
	UB	multi_use;		// using multi window assign
} T_VIO_ORDER_PAR;

// BEU output image set
typedef struct {
	UB	*dbaddry;		// output buffer Y/RGB address
	UB	*dbaddrc;		// output buffer C address
	UH	dbhsize;		// output buffer horizontal size
	UH	dhoffset;		// output buffer horizontal offset
	UH	dvoffset;		// output buffer vertical offset
	UH	ryform;			// output RGB/YUV format assign
	UB	pxa;			// RGB output PAD part select
	UB	pad;			// PAD part assign
	UB	tm;				// color transform mode
	UB	fxad;			// output address fix mode assign
	UB	dataswap;		// output data swap assign
	UB	mt;				// output mode
	UB	atf;			// odd/even address auto change
	UB	vskip;			// input image read skip
} T_VIO_BEU_OUT_PAR;

// ----------------------------------------------------------------------------
// NTSC synchronize clock set
typedef struct {
	UB	md;				// 16 or 8bit bus I/F  assign
	UB	ckpl;			// pixel clock polarity assign
	UB	hvpol;			// horizontal /vertical synchronized signal polarity assign
	UB	hsc;			// HSYNC control assign
	UB	hw;				// HSYNC assert width
	UB	vw;				// VSYNC assert width
} T_VIO_NTSC_CLK_PAR;

// NTSC input image set
typedef struct {
	UB	*sbaddry;		// input buffer Y/RGB address
	UB	*sbaddrc;		// input buffer C address
	UH	sbhsize;		// input buffer horizontal size
	UH	ryform;			// input RGB/YUV format assign
	UB	dataswap;		// input data swap assign
	UB	om;				// input mode select field/frame timing
	UB	is;				// input source select
} T_VIO_NTSC_IN_PAR;

// NTSC output image set
typedef struct {
	UW	bcr;			// back color (YCbCr)
	UH	hsz;			// blend image horizontal size
	UH	vsz;			// blend image vertical size
	UH	hratio;			// horizontal direction expand ratio
	UH	vratio;			// vertical direction expand ratio
	UH	hds;			// horizontal display size
	UH	vds;			// vertical display size
	UH	hdp;			// horizontal direction display position
	UH	vdp;			// vertical direction display position
	UH	hvp;			// horizontal direction valid pixel start position
	UH	vvp;			// vertical direction valid pixel start position
	UB	atf;			// odd/even address auto change
	UB	vskip;			// input image read skip
} T_VIO_NTSC_OUT_PAR;


/*----------------------- Driver functions Prototype ------------------------*/
// common function
#ifdef	__cplusplus
extern "C" {
#endif	/*__cplusplus*/

void shmdr_vio_Init( void );
ER shmdr_vio_Start( void );
ER shmdr_vio_Stop( void );
ER shmdr_vio_ReqVio( H req_id, TMO tmout );
ER shmdr_vio_FreeVio( H req_id );

// ----------------------------------------------------------------------------
// CEU
ER shmdr_vio_CeuCheckStatus( void );
ER shmdr_vio_CeuSetCam( T_VIO_CEU_CAM_IF *ceu_cam_if );
ER shmdr_vio_CeuSetCap(
	T_VIO_CEU_IN_PAR 	*ceu_in_par,
	T_VIO_CEU_OUT_PAR	*ceu_out_par,
	T_VIO_CEU_OUT_BUF	*ceu_out_buf );
ER shmdr_vio_CeuSetCmplx( UB dcoe, UB dcal, UW *dbaddrcp );
ER shmdr_vio_CeuSetIntCb( UW interrupt, FP function, UH cycle );
ER shmdr_vio_CeuStart( B ctrl );
ER shmdr_vio_CeuStop( B ctrl );
ER shmdr_vio_StrbSet( B mode, UH stcnt, UH slcnt, UB svpol, UB shpol, UB sepol );
ER shmdr_vio_StrbCtrl( B strb_sw );

// VEU
ER shmdr_vio_VeuCheckStatus( void );
ER shmdr_vio_Veu2h1CheckStatus( void );		//by JH.Jang
ER shmdr_vio_VeuSetPar(
	T_VIO_VEU_IN_PAR 	*veu_in_par,
	T_VIO_VEU_OUT_PAR 	*veu_out_par,
	T_VIO_VEU_PAR 		*veu_par );
ER shmdr_vio_Veu2h1SetPar(
	T_VIO_VEU_IN_PAR 	*veu_in_par,
	T_VIO_VEU_OUT_PAR 	*veu_out_par,
	T_VIO_VEU_PAR 		*veu_par );
ER shmdr_vio_SetIntVbendCb ( FP function );
ER shmdr_vio_VeuStart( FP function );
ER shmdr_vio_Veu2h1Start( FP function );
ER shmdr_vio_VeuRestart( void );
ER shmdr_vio_VeuAbort( void );
ER shmdr_vio_Veu2h1Abort( void );			//by JH.Jang

// ----------------------------------------------------------------------------
// BEU
ER shmdr_vio_BeuCheckStatus( void );
ER shmdr_vio_BeuSetOsdLut( UW *osd_clut, UW size );
ER shmdr_vio_BeuSetLayer(
	UB					lay_num,
	T_VIO_LAYER_PAR	*layer_par,
	T_VIO_GRAPHIC_PAR	*graphic_par );
ER shmdr_vio_BeuSetMulti(
	UB					multi_num,
	T_VIO_MULTI_PAR		*multi_par,
	T_VIO_PENETRA_PAR	*penetra_par );
ER shmdr_vio_BeuSetLust( UB brsel, UB mode );
ER shmdr_vio_BeuSetPar(
	T_VIO_ORDER_PAR	*order_info,
	T_VIO_BEU_OUT_PAR	*beu_out_par );
ER shmdr_vio_BeuStart( FP function );
ER shmdr_vio_BeuAbort( void );

// ----------------------------------------------------------------------------
// NTSC
ER shmdr_vio_NtscCheckStatus( void );
ER shmdr_vio_NtscEnable( T_VIO_NTSC_CLK_PAR *clk_par );
ER shmdr_vio_NtscDisable( void );
ER shmdr_vio_NtscSetOsdLut( UW *osd_clut, UW size );
ER shmdr_vio_NtscSetLayer(
	UB					lay_num,
	T_VIO_LAYER_PAR	*layer_par,
	T_VIO_GRAPHIC_PAR	*graphic_par );
ER shmdr_vio_NtscSetSrc(T_VIO_NTSC_IN_PAR *ntsc_in_par);
ER shmdr_vio_NtscSetMulti(
	UB					multi_num,
	T_VIO_MULTI_PAR		*multi_par,
	T_VIO_PENETRA_PAR	*penetra_par );
ER shmdr_vio_NtscSetPar(
	T_VIO_ORDER_PAR		*order_info,
	T_VIO_NTSC_OUT_PAR	*ntsc_out_par,
	UW					unique_key);
ER shmdr_vio_NtscSetIntCb(W interrupt, FP function);
ER shmdr_vio_NtscStart( FP function );
ER shmdr_vio_NtscStart2(void);
ER shmdr_vio_NtscStop( void );
ER shmdr_vio_NtscRestart( void );
ER shmdr_vio_NtscChangeLcdc(B mode, UB vsa, UB his, UH hi);
ER shmdr_vio_NtscClbCtrl( B ctrl, UH hds, UH vds, UH hvp, UH vvp );

#ifdef	__cplusplus
}
#endif	/*__cplusplus*/

#endif		// _SHMDR_VIO_DRV_H


