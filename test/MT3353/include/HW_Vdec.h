/*****************************************************************************
*  Copyright Statement:
*  --------------------
*  This software is protected by copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of MediaTek Inc. (C) 2008 MediaTek Inc.
*
*  BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
*  THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
*  RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO BUYER ON
*  AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
*  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
*  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
*  NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
*  SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
*  SUPPLIED WITH THE MEDIATEK SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH
*  THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO
*  NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S
*  SPECIFICATION OR TO CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
*  BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
*  LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
*  AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
*  OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY BUYER TO
*  MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
*  THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
*  WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT OF
*  LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING THEREOF AND
*  RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN FRANCISCO, CA, UNDER
*  THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE (ICC).
*
*****************************************************************************/

/*******************************************************************************
*
* Filename:
* ---------
* file HW_Vdec.h
*
* Project:
* --------
*   CNB
*
* Description:
* ------------
*
*
* Author:
* -------
*
*
*------------------------------------------------------------------------------
* $Revision: #1 $
* $Modtime:$
* $Log:$
*
*******************************************************************************/

#ifndef _MSDK_VDEC_H_
#define _MSDK_VDEC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <GDef.h>

typedef GVOID* HVDECINST;

typedef enum _VCODECID
{
    VCODEC_ID_UNKNOWN,	//< unknow type, used for debug
	VCODEC_ID_MP2V,		//< mpeg 1/2
	VCODEC_ID_MP4V,		//< mpeg 4

    VCODEC_ID_DIVX311,	//< Divx 3.11
	VCODEC_ID_DIVX4,	//< Divx 4
    VCODEC_ID_DIVX6,	//< Divx 5/6
	
	VCODEC_ID_WMV1,		//< WMV7
    VCODEC_ID_WMV2,		//< WMV8
    VCODEC_ID_WMV3,		//< WMV9
	VCODEC_ID_VC1,		//< VC1
    VCODEC_ID_H263,		//< H.263
	VCODEC_ID_SH263,	//< H.263 Sorenson version
	VCODEC_ID_RV30,		//< Real Video 8
	VCODEC_ID_RV40,		//< Real Video 9,10
    VCODEC_ID_H264		//< H.264
} VCODECID_T;

#define SEQ_HDR       1 << 16    ///< Access unit include a sequence header
#define GOP_HDR       1 << 17    ///< Access unit include a GOP header
#define SEQ_END       1 << 18    ///< Access unit include a sequence end
#define ANGLE_END     1 << 19    ///< Access unit include a angle end
#define SEQ_PS       1 << 20    ///< Access unit include a sequence parameter set in H264
#define PIC_PS       1 << 21    ///< Access unit include a picture parameter set in H264
#define SEI             1 << 22    ///< Access unit include a supplement enhancement information in H264
#define REF_PIC       1 << 23    ///< Access unit is a reference picture in H264
#define IDR_PIC       1 << 24    ///< Access unit is a IDR picture in H264
#define ENTRY_PTR   1 << 25    ///< Access unit is a Entry Pointer in WMV
#define AUTO_PAUSE 1<<26
#define MULTISLICE_PIC    1<<27    ///Access unit is a multi-slice picture (only for H264)
#define SUB_SEQ_PS    1<<28    ///Access unit is a multi-slice picture (only for H264)
#define PREFIX_NAL    1<<29    ///Access unit is a multi-slice picture (only for H264)

/*! \name H264 Picture Coding Type
* @{
*/
#define I_SLICE                0x64   ///< slice type 2 as I slice
#define P_SLICE                0x65   ///< slice type 0 as P slice
#define B_SLICE                0x66   ///< slice type 1 as B slice
#define SI_SLICE               0x67   ///< slice type 4 as SI slice
#define SP_SLICE               0x68   ///< slice type 3 as SP slice
#define I_ALL_SLICE            0x6a   ///< slice type 7 as I slice, all I type slices in this picture
#define P_ALL_SLICE            0x6b   ///< slice type 5 as P slice, all P type slices in this picture
#define B_ALL_SLICE            0x6c   ///< slice type 6 as B slice, all B type slices in this picture
#define SI_ALL_SLICE           0x6d   ///< slice type 9 as I slice, all SI type slices in this picture
#define SP_ALL_SLICE           0x6f   ///< slice type b as P slice, all SP type slices in this picture

#define I_TYPE                 1
#define P_TYPE                 2
#define B_TYPE                 3
#define D_TYPE                 4

#define TOP_FLD_PIC		       1
#define BTM_FLD_PIC			   2
#define FRM_PIC				   3
 
typedef struct
{
  GVOID      *pvInputBuff;//存放数据的起始地址
  GUINT32    u4DataSize;  //buffer大小
  GUINT32    u4Height;    //当前帧所代表的图像的高
  GUINT32    u4Width;     //当前帧所代表的图像的宽
  GUINT32    u4PicType;   //I B P帧等
  GUCHAR     ucPicStruct; //帧类型，场类型(上半场，下半场)
  GUINT64    u8PTS;       //此帧显示时间（-1 video decoder 做调整）
  GUINT32    u4Duration;  //此帧显示时长
  GUINT32    u4BitRate;   //比特率
  GUINT32    u4FFRate;    //快进使用标志，1、2、4分别代码正常解码，2倍速4倍速的解码
  GBOOL      fgSeek;      //是否为seek后的帧
}VDECINFO_INPUT_T;

typedef struct
{
  GVOID     *pvOutputBuff;//输出buffer
  GUINT32   u4OutBufSize; //buffer大小
  GUINT32   u4Height;     //解码后得到宽
  GUINT32   u4Width;      //解码后得到高
  GUINT32   u8Pts;        //decoder 调整后的pts
  GBOOL		fgDisplay;    //此输出帧是否可以显示，TRUE：此解码后的帧可以给display 显示，（H264用）
  GBOOL     fgNoNeedData; //下一帧是否需要输入解码数据（H264用）
}VDECINFO_OUTPUT_T;

HVDECINST Vdec_CreateInstance(VCODECID_T eCodecId);
GBOOL     Vdec_SetInputData(HVDECINST hVdecInst,VDECINFO_INPUT_T* prInputInfo);
GBOOL	  Vdec_GetOutputData(HVDECINST hVdecInst,VDECINFO_OUTPUT_T* prOutputInfo);
GBOOL	  Vdec_FreeOutputBuf(HVDECINST hVdecInst,GVOID* pvOutputBuf);
GBOOL	  Vdec_ReleaseInstance(HVDECINST hVdecInst);

#ifdef __cplusplus
}
#endif

#endif //_MSDK_VDEC_H_
