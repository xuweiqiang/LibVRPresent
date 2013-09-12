//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
#ifndef _DVDIOCTL_H_
#define _DVDIOCTL_H_

//
// DVD Device Control Functions
//

#define IOCTL_DVD_START_SESSION         CTL_CODE(FILE_DEVICE_DVD, 0x0000, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_DVD_READ_KEY              CTL_CODE(FILE_DEVICE_DVD, 0x0001, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_DVD_SEND_KEY              CTL_CODE(FILE_DEVICE_DVD, 0x0002, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_DVD_END_SESSION           CTL_CODE(FILE_DEVICE_DVD, 0x0003, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_DVD_GET_REGION            CTL_CODE(FILE_DEVICE_DVD, 0x0004, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_DVD_SET_REGION            CTL_CODE(FILE_DEVICE_DVD, 0x0005, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_DVD_SET_READ_AHEAD        CTL_CODE(FILE_DEVICE_DVD, 0x0402, METHOD_BUFFERED, FILE_READ_ACCESS)

/*
 * Error codes for DVD copy protection
 */

#define ERROR_DVD_SESSION_FAILURE       0xDD00  /* the AGID (SessionID) is not valid, or there are no AGIDs available in the system*/
#define ERROR_DVD_KEY_NOT_PRESENT       0xDD01  /* the file / disc is not protected with a key*/
#define ERROR_DVD_KEY_NOT_VALID         0xDD02  /* the key provided was not valid            */
#define ERROR_DVD_INVALID_REGION        0xDD03  /* region of the media is incorrect for the player */
#define ERROR_DVD_INVALID_RESET_COUNT   0xDD04  /* no region reset counts available*/

#pragma pack(push)
#pragma pack(1)

typedef ULONG DVD_SESSION_ID, *PDVD_SESSION_ID;

typedef enum DVD_STRUCTURE_FORMAT {
    DvdPhysicalDescriptor,
    DvdCopyrightDescriptor,
    DvdDiskKeyDescriptor,
    DvdBCADescriptor,
    DvdManufacturerDescriptor,
    DvdMaxDescriptor
} DVD_STRUCTURE_FORMAT, *PDVD_STRUCTURE_FORMAT;

typedef struct DVD_READ_STRUCTURE {
    LARGE_INTEGER BlockByteOffset;
    DVD_STRUCTURE_FORMAT Format;
    DVD_SESSION_ID SessionId;
    UCHAR LayerNumber;
} DVD_READ_STRUCTURE, *PDVD_READ_STRUCTURE;

typedef struct _DVD_DESCRIPTOR_HEADER {
    USHORT Length;
    UCHAR Reserved[2];
    UCHAR Data[1];
} DVD_DESCRIPTOR_HEADER, *PDVD_DESCRIPTOR_HEADER;

typedef struct _DVD_LAYER_DESCRIPTOR {
    USHORT Length;
    UCHAR BookVersion : 4;
    UCHAR BookType : 4;
    UCHAR MinimumRate : 4;
    UCHAR DiskSize : 4;
    UCHAR LayerType : 4;
    UCHAR TrackPath : 1;
    UCHAR NumberOfLayers : 2;
    UCHAR Reserved1 : 1;
    UCHAR TrackDensity : 4;
    UCHAR LinearDensity : 4;
    ULONG StartingDataSector;
    ULONG EndDataSector;
    ULONG EndLayerZeroSector;
    UCHAR Reserved5 : 7;
    UCHAR BCAFlag : 1;
    UCHAR Reserved6;
} DVD_LAYER_DESCRIPTOR, *PDVD_LAYER_DESCRIPTOR;

typedef struct _DVD_COPYRIGHT_DESCRIPTOR {
    UCHAR CopyrightProtectionType;
    UCHAR RegionManagementInformation;
    USHORT Reserved;
} DVD_COPYRIGHT_DESCRIPTOR, *PDVD_COPYRIGHT_DESCRIPTOR;

typedef struct _DVD_DISK_KEY_DESCRIPTOR {
    UCHAR DiskKeyData[2048];
} DVD_DISK_KEY_DESCRIPTOR, *PDVD_DISK_KEY_DESCRIPTOR;

typedef struct _DVD_BCA_DESCRIPTOR {
    UCHAR BCAInformation[1];
} DVD_BCA_DESCRIPTOR, *PDVD_BCA_DESCRIPTOR;

typedef struct _DVD_MANUFACTURER_DESCRIPTOR {
    UCHAR ManufacturingInformation[2048];
} DVD_MANUFACTURER_DESCRIPTOR, *PDVD_MANUFACTURER_DESCRIPTOR;

typedef struct _DVD_RPC_KEY {
    UCHAR UserResetsAvailable:3;
    UCHAR ManufacturerResetsAvailable:3;
    UCHAR TypeCode:2;
    UCHAR RegionMask;
    UCHAR RpcScheme;
    UCHAR Reserved2[1];
} DVD_RPC_KEY, * PDVD_RPC_KEY;

typedef struct _DVD_SET_RPC_KEY {
    UCHAR PreferredDriveRegionCode;
    UCHAR Reserved[3];
} DVD_SET_RPC_KEY, * PDVD_SET_RPC_KEY;

typedef struct _DVD_ASF {
    UCHAR Reserved0[3];
    UCHAR SuccessFlag:1;
    UCHAR Reserved1:7;
} DVD_ASF, * PDVD_ASF;

typedef struct _DVD_REGION {
     UCHAR CopySystem;
     UCHAR RegionData;                      // current media region (not playable when set)
     UCHAR SystemRegion;                    // current drive region (playable when set)
     UCHAR ResetCount;                      // number of resets available
} DVD_REGION, *PDVD_REGION;


//
// Predefined (Mt. Fuji) key sizes
// Add sizeof(DVD_COPY_PROTECT_KEY) to get allocation size for
// the full key structure
//

#define DVD_CHALLENGE_KEY_LENGTH    (12 + sizeof(DVD_COPY_PROTECT_KEY))
#define DVD_BUS_KEY_LENGTH          (8 + sizeof(DVD_COPY_PROTECT_KEY))
#define DVD_TITLE_KEY_LENGTH        (8 + sizeof(DVD_COPY_PROTECT_KEY))
#define DVD_DISK_KEY_LENGTH         (2048 + sizeof(DVD_COPY_PROTECT_KEY))
#define DVD_RPC_KEY_LENGTH          (sizeof(DVD_RPC_KEY) + sizeof(DVD_COPY_PROTECT_KEY))
#define DVD_SET_RPC_KEY_LENGTH      (sizeof(DVD_SET_RPC_KEY) + sizeof(DVD_COPY_PROTECT_KEY))
#define DVD_ASF_LENGTH              (sizeof(DVD_ASF) + sizeof(DVD_COPY_PROTECT_KEY))

/////////////////////////////////


typedef enum {
    DvdAGID,
    DvdChallengeKey = 0x01,
    DvdBusKey1,
    DvdBusKey2,
    DvdTitleKey,
    DvdAuthenticationSuccessFlag,
    DvdSetRPC = 0x06,
    DvdGetRPC = 0x08,
    DvdDriveKey = 0x80
} DVD_KEY_TYPE;

typedef ULONG DVD_SESSION_ID, *PDVD_SESSION_ID;

typedef struct _DVD_COPY_PROTECT_KEY {
    HANDLE hDrive;
    ULONG KeyLength;
    DVD_SESSION_ID SessionId;
    DVD_KEY_TYPE KeyType;
    ULONG StartAddr;
    ULONG KeyFlags;

    ULONG hfile;
    ULONG Reserved;             // alignment for NT

    UCHAR KeyData[1];
} DVD_COPY_PROTECT_KEY, *PDVD_COPY_PROTECT_KEY;

typedef DVD_COPY_PROTECT_KEY DVD_READ_KEY, *PDVD_READ_KEY;
typedef DVD_COPY_PROTECT_KEY DVD_SEND_KEY, *PDVD_SEND_KEY;

//
// Predefined (Mt. Fuji) key sizes 
// Add sizeof(DVD_COPY_PROTECT_KEY) to get allocation size for 
// the full key structure
//

#define DVD_CHALLENGE_KEY_LENGTH    (12 + sizeof(DVD_COPY_PROTECT_KEY))
#define DVD_BUS_KEY_LENGTH          (8 + sizeof(DVD_COPY_PROTECT_KEY))
#define DVD_TITLE_KEY_LENGTH        (8 + sizeof(DVD_COPY_PROTECT_KEY))
#define DVD_DISK_KEY_LENGTH      (2048 + sizeof(DVD_COPY_PROTECT_KEY))

//
// Used with IOCTL_DVD_END_SESSION to end all DVD sessions at once
//

#define DVD_END_ALL_SESSIONS ((DVD_SESSION_ID) 0xffffffff)

//
// CGMS Copy Protection Flags
//

#define DVD_CGMS_RESERVED_MASK      0x00000078

#define DVD_CGMS_COPY_PROTECT_MASK  0x00000018
#define DVD_CGMS_COPY_PERMITTED     0x00000000
#define DVD_CGMS_COPY_ONCE          0x00000010
#define DVD_CGMS_NO_COPY            0x00000018

#define DVD_COPYRIGHT_MASK          0x00000040
#define DVD_NOT_COPYRIGHTED         0x00000000
#define DVD_COPYRIGHTED             0x00000040

#define DVD_SECTOR_PROTECT_MASK     0x00000020
#define DVD_SECTOR_NOT_PROTECTED    0x00000000
#define DVD_SECTOR_PROTECTED        0x00000020

typedef struct _DVD_REGIONCE {
     HANDLE hDrive;
     UCHAR CopySystem;              
     UCHAR RegionData;              // current media region         
     UCHAR SystemRegion;            // current drive region
     UCHAR ResetCount;              // number of resets available
} DVD_REGIONCE, *PDVD_REGIONCE;    


typedef struct _DVD_SET_REGION {
     UCHAR RegionData;              // new drive region
     UCHAR Reserved[3];             
} DVD_SET_REGION, *PDVD_SET_REGION;    

typedef struct _DVD_READ_AHEAD { 
     ULONG      BufferLength;
     HANDLE     DVDSRA_HFileSrc;            // open handle to file on volume
     ULONGLONG  DVDSRA_SrcOffs;         // offset in the source file
     HANDLE     DVDSRA_HFileDest;           // open handle of target file
     ULONGLONG  DVDSRA_DestOffs;        // offset in the target file
     ULONGLONG  DVDSRA_DestCnt;         // number of bytes to read ahead in targer
    
} DVD_READ_AHEAD, *PDVD_READ_AHEAD;

/*

Note: the following variable must be localized to match the region of media to be
played back.  Allowable values are a single binary 1 in bits 0-6

*/

#define DEFAULT_DECODER_REGION 0x01                 // US region




// Packet for DVD

typedef  struct _RKCDB { /* */
    UCHAR   OpCode;
    UCHAR   Lun;
    ULONG   LBA;
    USHORT  Reserved;
    USHORT  AllocLen;
    UCHAR   KeyFmt;
    UCHAR   NACA;
} RKCDB, *PRKCDB;


//
// report key returned data structures
//

typedef struct _RKFMT_AGID{ /* */
    USHORT  Len;
    UCHAR   Reserved[5];
    UCHAR   agid;
} RKFMT_AGID, *PRKFMT_AGID;

typedef struct _RKFMT_CHLGKEY{ /* */
    USHORT  Len;
    UCHAR   Reserved[2];
    UCHAR   chlgkey[10];
    UCHAR   Reserved2[2];
} RKFMT_CHLGKEY, *PRKFMT_CHLGKEY;

typedef struct _RKFMT_DISC{ /* */
    ULONG   Len;
    UCHAR   DiscKey[2048];
} RKFMT_DISC, *PRKFMT_DISC;

typedef struct _RKFMT_BUS{ /* */
    USHORT  Len;
    UCHAR   Reserved[2];
    UCHAR   bus[5];
    UCHAR   Reserved2[3];
} RKFMT_BUS, *PRKFMT_BUS;

typedef struct _RKFMT_TITLE{ /* */
    USHORT  Len;
    UCHAR   Reserved[2];
    UCHAR   cgms;
    UCHAR   title[5];
    UCHAR   Reserved2[2];
} RKFMT_TITLE, *PRKFMT_TITLE;

typedef struct _RKFMT_RPC { /* */
    USHORT Len;
    UCHAR Reserved[2];
    UCHAR ResetCounts;
    UCHAR SystemRegion;
    UCHAR RPCVer;
    UCHAR Reserved2;
} RKFMT_RPC, *PRKFMT_RPC;

typedef struct _RKFMT_RPC_SET { /* */
    USHORT Len;
    UCHAR Reserved[2];
    UCHAR SystemRegion;
    UCHAR Reserved2[3];
} RKFMT_RPC_SET, *PRKFMT_RPC_SET;

typedef struct _RDVDFMT_Copy { /* */
    ULONG Len;
    ULONG Data;
} RDVDFMT_Copy, *PRDVDFMT_Copy;

typedef struct _RDVDCDB { /* */
    UCHAR   OpCode;
    UCHAR   Lun;
    UCHAR   RMDLBA[4];
    UCHAR   Layer;
    UCHAR   Format;
    USHORT  Len;
    UCHAR   agid;
    UCHAR   NACA;
} RDVDCDB, *PRDVDCDB;

typedef struct _RASRB { /* */
    UCHAR OpCode;
    UCHAR Lun;
    ULONG StartAddr;
    ULONG NextAddr;
    USHORT Reserved;
} RASRB, *PRASRB;

typedef struct _GESN { /*  */
    UCHAR OpCode;
    UCHAR Lun;
    UCHAR Reserved[2];
    UCHAR NotifyClass;
    USHORT Reserved2;
    UCHAR ListLen[2];
    UCHAR Reserved3;
} GESN, *PGESN;

typedef struct _GET_CONFIG { /* */
    UCHAR OpCode;
    UCHAR Lun;
    UCHAR FeatureNum[2];
    UCHAR Reserved[3];
    USHORT Len;
    UCHAR Reserved2;
} GET_CONFIG, *PGET_CONFIG;

#define GETCONFIGRT_ALL         0
#define GETCONFIGRT_CURRENT     1
#define GETCONFIGRT_ONE         2

typedef struct _CONFIG_HEADER { /* */
    UCHAR DataLength[4];
    UCHAR Reserved[2];
    UCHAR CurrentProfile[2];
} CONFIG_HEADER, *PCONFIG_HEADER;

typedef struct _CONFIG_MORPH { /* */
    UCHAR FeatureCode[2];
    UCHAR Version;
    UCHAR AddLength;
    UCHAR Async;
    UCHAR Reserved[3];
} CONFIG_MORPH, *PCONFIG_MORPH;

#define CONFIG_CLASS_MORPH 0x2

//
// Media Event Status Notification return structure
//

typedef struct _MESN_HDR{ /* */
    USHORT DataLen;
    UCHAR   Class;
    UCHAR   SuportedClasses;
} MESN_HDR, *PMESN_HDR;

typedef struct _GESN_DATA{ /* */
    UCHAR   Event;
    UCHAR   MediaStatus;
    UCHAR   StartSlot;
    UCHAR   EndSlot;
} GESN_DATA, *PGESN_DATA;

typedef struct _GESN_RET{ /* */
    MESN_HDR hdr;
    GESN_DATA data;
} GESN_RET, *PGESN_RET;

//
// notification class bits
//

#define MEDIA_STATUS_CLASS 1 << 4
#define MEDIA_STATUS_CLASS_BIT 4

//
// media event formats
//

#define MEDIAE_NO_EVENT  0
#define MEDIAE_EJECT_REQ 1
#define MEDIAE_NEWMEDIA  2
#define MEDIAE_REMOVAL   3
//
// Read DVD structure formats
//

#define DVDSTRUC_FMT_PHYS       0x0
#define DVDSTRUC_FMT_COPY       0x1
#define DVDSTRUC_FMT_DISCKEY    0x2
#define DVDSTRUC_FMT_BCA        0x3
#define DVDSTRUC_FMT_MFG        0x4

//
// DVD specific command opcode
//

#define DVDOP_REPORT_KEY        0xA4
#define DVDOP_SEND_KEY          0xA3
#define DVDOP_READ_DVD_STRUC    0xAD
#define DVDOP_SET_READ_AHEAD    0xA7
#define DVDOP_GESN              0x4a
#define DVDOP_GET_CONFIG        0x46

//
// DVD specific error codes
//

#define DVDADSENSE_COPYPROT         0x6F
#define DVDADSENSE_RESOURCE_FAIL    0x55

#define DVDADSQ_KEYNOTPRESENT       0x01
#define DVDADSQ_KEYNOTVALID         0x02
#define DVDADSQ_REGIONFAILURE       0x04
#define DVDADSQ_REGIONCOUNT         0x05

#pragma pack(pop)

#endif //_DVDIOCTL_H_
