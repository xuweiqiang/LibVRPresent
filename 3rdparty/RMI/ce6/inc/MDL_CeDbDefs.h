//
// Copyright (c) 2005  Advanced Micro Device, AMD.  All rights reserved.
// Copyright (c) 2005  BSQUARE Corporation.  All rights reserved.
// 
// Module Name:
//
//     MDL_CeDbDefs.h
//
// Abstract:
//
//     Defines the objects and properties of the Media Data Library (MDL)
//


#if !defined _MTP_CEDBDEFS_H
#define _MTP_CEDBDEFS_H

typedef unsigned char       MDL_UINT8;
typedef unsigned short      MDL_UINT16;
typedef unsigned int        MDL_UINT32;
typedef unsigned long long  MDL_UINT64;


#define MDL_MAX_STRING_LENGTH 255
#define MDL_MAX_DATE_TIME_STRING_LENGTH 23

typedef char mdl_DateTimeString[MDL_MAX_DATE_TIME_STRING_LENGTH];

typedef unsigned int 	mdl_ClientId;
typedef unsigned int 	mdl_PUOI;

//typedef CEOID mdl_PUOI;   // this is how it 
typedef MDL_UINT32 mdl_ClientId;

/**
 * The return value type for all InterfaceFunctions 
 */ 
typedef enum mdl_ResultCodeTag
{
    MDL_SUCCESS,
    MDL_FAILURE,
    MDL_CLIENT_NOT_REGISTERED,
    MDL_ACCESS_DENIED,
    MDL_BLOCKED,
    MDL_OBJECT_NOT_FOUND,
    MDL_DB_NOT_OPEN
} mdl_ResultCodeType;

/**
 *  Passed in a ::mdl_EventStructType structure
 * as an argument to a clients callback function.
 */
typedef enum mdl_EventTypeTag
{
    OBJECT_ADDED,
    OBJECT_REMOVED,
    OBJECT_CHANGED,
    GROUP_DATA_CHANGED,
    ALL_OBJECTS_CHANGED,  /**< This event is sent when a client 
                               has called mdl_SignalAllObjectsChanged(). Clients should assume
                               that any object data that they are maintaining locally is now invalid.
                               This event may be used to force a re-population of the object database 
                               by the MTP implementation client in response to some 
                               access of the device after which we don't know what has changed.*/
    EXTERNAL_CONNECT,
    EXTERNAL_DISCONNECT
} mdl_EventType;


 /**
  * 16 bit wide
  * 
  * All values with bit 15 set to 1	Extension range	MTP Vendor Extension
  */
typedef enum mdl_ScanTypeTag
{
    SCANTYPE_UNUSED=                  0x0000, /**< Unused*/
    PROGRESSIVE=                      0x0001, /**< Indicates progressive frames.*/
    FIELD_INTERLEAVED_UPPER_FIRST=    0x0002, /**< Line interleaved Frames with the Upper field on the first line.*/
    FIELD_INTERLEAVED_LOWER_FIRST=    0x0003, /**< Line interleaved frames with the Lower field on the first line.*/
    FIELD_SINGLE_UPPER_FIRST=         0x0004, /**< Fields are sent as independent samples. The field is indicated (on a per sample basis)*/
    FIELD_SINGLE_LOWER_FIRST=         0X0005, /**< Fields are sent as independent samples. The field is indicated (on a per sample basis*/
    MIXED_INTERLACE=                  0x0006, /**< The content may contain a mix of interlaced modes*/
    MIXED_INTERLACE_AND_PROGRESSIVE=  0x0007  /**< The content may contain a mix of interlaced and progressive modes.   */
} mdl_ScanTypeType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

/**
 *  See Appendix I of \ref MTP_Enhanced for more information on 
 *  MTP object properties.
 * 
 */
typedef enum mdl_ObjectPropertyNamesTag
{
	FULL_PATH=							0x0001,
    STORAGE_ID=                         0xDC01,
    OBJECT_FORMAT=                      0xDC02,
    PROTECTION_STATUS=                  0xDC03,
    OBJECT_SIZE=                        0xDC04,
    ASSOCIATION_TYPE=                   0xDC05,
    OBJECT_FILENAME=                    0xDC07,
    DATE_CREATED=                       0xDC08,
    DATE_MODIFIED=                      0xDC09,
    KEYWORDS=                           0xDC0A,
    PARENT_OBJECT=                      0xDC0B,
    PERSISTENT_UNIQUE_OBJECT_IDENTIFIER=0xDC41,
    SYNC_ID=                            0xDC42,
    PROPERTY_BAG=                       0xDC43,
    NAME=                               0xDC44,
    CREATED_BY=                         0xDC45,
    ARTIST=                             0xDC46,
    DATE_AUTHORED=                      0xDC47,
    DESCRIPTION=                        0xDC48,
    URL_REFERENCE=                      0xDC49,
    LANGUAGE_LOCALE=                    0xDC4A,
    COPYRIGHT_INFORMATION=              0xDC4B,
    SOURCE=                             0xDC4C,
    ORIGIN_LOCATION=                    0xDC4D,
    DATEADDED=                          0xDC4E,
    NON_CONSUMABLE=                     0xDC4F,
    CORRUPT_UNPLAYABLE=                 0xDC50,
    REPRESENTATIVE_SAMPLE_FORMAT=       0xDC81,
    REPRESENTATIVE_SAMPLE_SIZE=         0xDC82,
    REPRESENTATIVE_SAMPLE_HEIGHT=       0xDC83,
    REPRESENTATIVE_SAMPLE_WIDTH=        0xDC84,
    REPRESENTATIVE_SAMPLE_DURATION=     0xDC85,
    REPRESENTATIVE_SAMPLE_DATA=         0xDC86,
    WIDTH=                              0xDC87,
    HEIGHT=                             0xDC88,
    DURATION=                           0xDC89,
    RATING=                             0xDC8A,
    TRACK=                              0xDC8B,
    GENRE=                              0xDC8C,
    CREDITS=                            0xDC8D,
    LYRICS=                             0xDC8E,
    SUBSCRIPTION_CONTENT_ID=            0xDC8F,
    PRODUCED_BY=                        0xDC90,
    USE_COUNT=                          0xDC91,
    SKIP_COUNT=                         0xDC92,
    LAST_ACCESSED=                      0xDC93,
    PARENTAL_RATING=                    0xDC94,
    META_GENRE=                         0xDC95,
    COMPOSER=                           0xDC96,
    EFFECTIVE_RATING=                   0xDC97,
    SUBTITLE=                           0xDC98,
    ORIGINAL_RELEASE_DATE=              0xDC99,
    ALBUM_NAME=                         0xDC9A,
    ALBUM_ARTIST=                       0xDC9B,
    MOOD=                               0xDC9C,
    DRM_STATUS=                         0xDC9D,
    SUB_DESCRIPTION=                    0xDC9E,
    IS_CROPPED=                         0xDCD1,
    IS_COLOURCORRECTED=                 0xDCD2,
    TOTAL_BIT_RATE=                     0xDE91,
    BITRATE_TYPE=                       0xDE92,
    SAMPLE_RATE=                        0xDE93,
    NUMBER_OF_CHANNELS=                 0xDE94,
    AUDIO_BIT_DEPTH=                    0xDE95,
    SCAN_TYPE=                          0xDE97,
    AUDIO_WAVE_CODEC=                   0xDE99,
    AUDIO_BIT_RATE=                     0xDE9A,
    VIDEO_FOUR_CC_CODEC=                0xDE9B,
    VIDEO_BIT_RATE=                     0xDE9C,
    FRAMES_PER_THOUSAND_SECONDS=        0xDE9D,
    KEY_FRAME_DISTANCE=                 0xDE9E,
    BUFFER_SIZE=                        0xDE9F,
    ENCODING_QUALITY=                   0xDEA0,
    BUY_FLAG=                           0xD901
} mdl_ObjectPropertyNamesType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

  /**
  * 16 bit wide
  * 
  */
typedef enum mdl_NumberOfChannelsTag
{
    CHANNEL_UNUSED= 0x0000,
    MONO=           0x0001, /**< (1 channel) */
    STEREO=         0x0002, /**< (2 channels) */
    CHANNELS_2_1=   0x0003,
    CHANNELS_3=     0x0004,
    CHANNELS_3_1=   0x0005,
    CHANNELS_4=     0x0006,
    CHANNELS_4_1=   0x0007,
    CHANNELS_5=     0x0008,
    CHANNELS_5_1=   0x0009,
    CHANNELS_6=     0x000A,
    CHANNELS_6_1=   0x000B,
    CHANNELS_7=     0x000C,
    CHANNELS_7_1=   0x000D,
    CHANNELS_8=     0x000E,
    CHANNELS_8_1=   0x000F,
    CHANNELS_9=     0x0010,
    CHANNELS_9_1=   0x0011
} mdl_NumberOfChannelsType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

  /**
   * 16 bit wide
   * 
   * All values with bit 15 set to 1	MTP Vendor Extension
   */
typedef enum mdl_BitRateTypeTag
{
    UNUSED_RATE =   0x0000,
    DISCRETE=       0x0001,
    VARIABLE=       0x0002,
    FREE=           0x0003
} mdl_BitRateTypeType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

  /**
   * 16 bit wide
   */
typedef enum mdl_IsColorCorrectedTag
{
    NOT_COLOR_CORRECTED=        0x0000, /**< This image has not been ColorCorrected.*/
    COLOR_CORRECTED=            0x0001, /**< This image has been ColorCorrected.*/
    SHOULD_BE_COLOR_CORRECTED=  0x0002  /**< This image has not been ColorCorrected, and should not be ColorCorrected. */
} mdl_IsColorCorrectedType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

  /**
   * 16 bit wide
   */
typedef enum mdl_IsCroppedTag
{
    NOT_CROPPED=        0x0000, /**< This image has not been cropped.*/
    CROPPED=            0x0001, /**< This image has been cropped.*/
    SHOULD_BE_CROPPED=  0x0002  /**< This image has not been cropped, and should not be cropped. */
} mdl_IsCroppedType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

/**
 * 16 bit wide
 * All other values where Bit 15 is 0	Reserved for MTP
 * All other values where Bit 15 is 1	MTP Vendor Extension range
 */
typedef enum mdl_DrmStatusTag
{
    NO_DRM=   0x0000, 
    HAS_DRM=  0x0001  
} mdl_DrmStatusType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

typedef struct mdl_RepresentativeDataTag
{
	unsigned int NumElements;
	unsigned short * Data;
} mdl_RepresentativeDataType;

typedef struct mdl_RepresentativeDataDBTag
{
	unsigned int NumElements;
	unsigned short  Data[1];
} mdl_RepresentativeDataDBType;	

/**
 * \todo find out what this should be from the PTP spec.
 */
typedef struct mdl_AssociationDescTag
{
	int desc;
} mdl_AssociationDescType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;           

/**
 * 
 * All other values with bit 15 set to 0	Reserved	Unused
 * All other values with bit 15 set to 1 and bit 14 set to 0	Vendor-defined	Undefined
 */
typedef enum mdl_AssociationCodeTag
{
    ASSOCIATION_CODE_UNDEFINED=     0x0000, /**< Undefined */
    GENERIC_FOLDER=                 0X0001, /**< Used by MTP */
    ALBUM=                          0X0002, /**< Reserved */
    TIME_SEQUENCE=                  0x0003, /**< Default Playback Delta */
    HORIZONTAL_PANORAMIC=           0x0004, /**< Unused */
    VERTICAL_PANORAMIC=             0x0005, /**< Unused */
    TWOD_PANORAMIC=                 0x0006, /**< Images per row */
    ANCILLARY_DATA=                 0x0007  /**< Undefined */
} mdl_AssociationCodeType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

/**
 * 16 bit wide 
 */
typedef enum mdl_ProtectionStatusTag
{
    NO_PROTECTION=           0x0000,
    READ_ONLY=               0x0001,
    READ_ONLY_DATA=          0x8002,
    NON_TRANSFERRABLE_DATA=  0x8003
}mdl_ProtectionStatusType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

/**
 * 
 * All other codes with MSN 0011	Undefined	Reserved (PTP)
 * All codes with most significant 5 bytes = 10110	Undefined	MTP vendor extensions
 * All codes with most significant 5 bytes = 10111	Undefined	MTP-defined
 * 
 * This list contains both the PTP and MTP specified values
 */
typedef enum mdl_ObjectFormatCodeTag
{
/*--PTP defined---------------------*/
    OBJECT_CODE_UNDEFINED0=         0x3000, /**< Undefined object */
    OBJECT_CODE_ASSOCIATION=        0x3001, /**< Association (for example, a folder) */
    OBJECT_CODE_SCRIPT=             0x3002, /**< Device model-specific script */
    OBJECT_CODE_EXECUTABLE=         0x3003, /**< Device model-specific binary executable */
    OBJECT_CODE_TEXT=               0x3004, /**< Text file */
    OBJECT_CODE_HTML=               0x3005, /**< Hypertext Markup Language file (text) */
    OBJECT_CODE_DPOF=               0x3006, /**< Digital Print Order Format file (text) */
    OBJECT_CODE_AIFF=               0x3007, /**< Audio clip */
    OBJECT_CODE_WAV=                0x3008, /**< Audio clip */
    OBJECT_CODE_MP3=                0x3009, /**< Audio clip */
    OBJECT_CODE_AVI=                0x300A, /**< Video clip */
    OBJECT_CODE_MPEG=               0x300B, /**< Video clip */
    OBJECT_CODE_ASF=                0x300C, /**< Microsoft Advanced Streaming Format (video) */
    OBJECT_CODE_UNKOWN=             0x3800, /**< Unknown image object */
    OBJECT_CODE_EXIF_JPEG=          0x3801, /**< Exchangeable File Format, JEIDA standard */
    OBJECT_CODE_TIFF_EP=            0x3802, /**< Tag Image File Format for Electronic Photography */
    OBJECT_CODE_FLASHPIX=           0x3803, /**< Structured Storage Image Format */
    OBJECT_CODE_BMP=                0x3804, /**< Microsoft Windows Bitmap file */
    OBJECT_CODE_CIFF=               0x3805, /**< Canon Camera Image File Format */
    OBJECT_CODE_UNDEFINED1=         0x3806, /**< Reserved */
    OBJECT_CODE_GIF=                0x3807, /**< Graphics Interchange Format */
    OBJECT_CODE_JFIF_JPEG=          0x3808, /**< File Interchange Format */
    OBJECT_CODE_CD=                 0x3809, /**< PhotoCD Image Pac */
    OBJECT_CODE_ICT=                0x380A, /**< Quickdraw Image Format */
    OBJECT_CODE_PNG=                0x380B, /**< Portable Network Graphics */
    OBJECT_CODE_UNDEFINED2=         0x380C, /**< eserved */
    OBJECT_CODE_TIFF=               0x380D, /**< Tag Image File Format */
    OBJECT_CODE_TIFF_IT=            0x380E, /**< Tag Image File Format for Information Technology (graphic arts) */
    OBJECT_CODE_JP2=                0x380F, /**< JPEG2000 Baseline File Format */
    OBJECT_CODE_JPX=                0x3810, /**< JPEG2000 Extended File Format */
/*--MTP defined---------------------*/
    UNDEFINED_FIRMWARE=             0xB802, 
    WINDOWS_IMAGE_FORMAT=           0xB881, 
    UNDEFINED_AUDIO=                0xB900, 
    WMA=                            0xB901, 
    OGG=                            0xB902, 
    UNDEFINED_VIDEO=                0xB980, 
    WMV=                            0xB981, 
    MP4_CONTAINER=                  0xB982, /**< ISO 14496-1 */
    UNDEFINED_COLLECTION=           0xBA00, 
    ABSTRACT_MULTIMEDIA_ALBUM=      0xBA01, 
    ABSTRACT_IMAGE_ALBUM=           0xBA02, 
    ABSTRACT_AUDIO_ALBUM=           0XBA03, 
    ABSTRACT_VIDEO_ALBUM=           0XBA04, 
    ABSTRACT_AUDIO_VIDEO_PLAYLIST=  0xBA05, 
    ABSTRACT_CONTACT_GROUP=         0xBA06, 
    ABSTRACT_MESSAGE_FOLDER=        0xBA07, 
    ABSTRACT_CHAPTERED_PRODUCTION=  0xBA08, 
    WPL_PLAYLIST=                   0xBA10, 
    M3U_PLAYLIST=                   0XBA11, 
    MPL_PLAYLIST=                   0XBA12, 
    ASX_PLAYLIST=                   0XBA13, 
    PLS_PLAYLIST=                   0xBA14, 
    UNDEFINED_DOCUMENT=             0xBA80, 
    ABSTRACT_DOCUMENT=              0xBA81, 
    UNDEFINED_MESSAGE=              0xBB00, 
    ABSTRACT_MESSAGE=               0xBB01, 
    UNDEFINED_CONTACT=              0xBB80, 
    ABSTRACT_CONTACT=               0xBB81, 
    VCARD2=                         0xBB82, 
    VCARD3=                         0xBB83, 
    UNDEFINED_CALENDER_ITEM=        0xBE00, 
    ABSTRACT_CALENDER_ITEM=         0xBE01, 
    VCALENDAR1=                     0xBE02, 
    VCALENDAR2=                     0xBE03, 
    UNDEFINED_WINDOWS_EXECUTABLE=   0xBE80 
} mdl_ObjectFormatCodeType;//z /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

//----------------------------------------------------
// Database property identifiers
// CE property IDs are 32bit values, the lower 16bits is the
// property type as defined by CE, the upper 16bits is our
// unique property code.
#define PID_FULL_LPATH				(MAKELONG(CEVT_LPWSTR, 1))
#define PID_STORAGEID				(MAKELONG(CEVT_UI4, 2))			   // A 32-bit unsigned int, unsigned int,unsigned int
#define PID_OBJECT_FORMAT			(MAKELONG(CEVT_UI4, 3))			   // A 32-bit unsigned int, unsigned int,unsigned int
#define PID_PROTECTION_STATUS		(MAKELONG(CEVT_UI4, 4))			   // A 32-bit unsigned int, unsigned int,unsigned int
#define PID_OBJECTSIZE				(MAKELONG(CEVT_R8, 5))		    	  // A 64 bit signed int ;
#define PID_ASSOCIATION_TYPE		(MAKELONG(CEVT_UI4, 6))				// A 32-bit unsigned int, unsigned int,unsigned int
#define PID_ASSOCIATION_DESC		(MAKELONG(CEVT_I4, 7))				 // A 32-bit unsigned int, unsigned int,unsigned int
#define PID_OBJECT_FILE_NAME		(MAKELONG(CEVT_LPWSTR, 8))
#define PID_DATE_CREATE				(MAKELONG(CEVT_LPWSTR, 9))
#define PID_DATE_MODIFIED			(MAKELONG(CEVT_LPWSTR, 10))
#define PID_KEYWORDS				(MAKELONG(CEVT_LPWSTR, 11))
#define PID_PARENT_OBJECT			(MAKELONG(CEVT_UI4, 12))
#define PID_PERSIS_UNIQ_OBJ_ID		(MAKELONG(CEVT_UI4, 13))		
#define PID_SYNC_ID					(MAKELONG(CEVT_LPWSTR, 14))
#define PID_PROPERTY_BAG			(MAKELONG(CEVT_LPWSTR, 15))
#define PID_NAME					(MAKELONG(CEVT_LPWSTR, 16))
#define PID_CREATEDBY				(MAKELONG(CEVT_LPWSTR, 17))
#define PID_ARTIST					(MAKELONG(CEVT_LPWSTR, 18))
#define PID_DATE_AUTHORED			(MAKELONG(CEVT_LPWSTR, 19))
#define PID_DESCRIPTION				(MAKELONG(CEVT_LPWSTR, 20))
#define PID_URL_REFERENCE			(MAKELONG(CEVT_LPWSTR, 21))
#define PID_LANGUAGE_LOCALE			(MAKELONG(CEVT_LPWSTR, 22))
#define PID_COPYRIGHT_INFORMATION	(MAKELONG(CEVT_LPWSTR, 23))
#define PID_SOURCE					(MAKELONG(CEVT_LPWSTR, 24))
#define PID_ORIGIN_LOCATION			(MAKELONG(CEVT_LPWSTR, 25))
#define PID_DATE_ADDED				(MAKELONG(CEVT_LPWSTR, 26))
#define PID_NON_CONSUMABLE			(MAKELONG(CEVT_UI2, 27))				// A 16 bit unsinged int
#define PID_CORRUPT_UNPLAYABLE		(MAKELONG(CEVT_UI2, 28))				// A 16 bit singed int
#define PID_REP_SAMPLE_FORMAT		(MAKELONG(CEVT_UI4, 29))
#define PID_REP_SAMPLE_SIZE			(MAKELONG(CEVT_UI4, 30))
#define PID_REP_SAMPLE_HEIGHT		(MAKELONG(CEVT_UI4, 31))
#define PID_REP_SAMPLE_WIDTH		(MAKELONG(CEVT_UI4, 32))
#define PID_REP_SAMPLE_DURATION		(MAKELONG(CEVT_UI4, 33))
#define PID_REP_SAMPLE_DATA_TAG_DATA (MAKELONG(CEVT_UI2, 34))
#define PID_WIDTH					(MAKELONG(CEVT_UI4, 35))
#define PID_HEIGHT					(MAKELONG(CEVT_UI4, 36))
#define PID_DURATION				(MAKELONG(CEVT_UI4, 37))
#define PID_RATING					(MAKELONG(CEVT_UI2, 38))
#define PID_TRACK					(MAKELONG(CEVT_UI2, 39))										
#define PID_GENRE					(MAKELONG(CEVT_LPWSTR, 40))
#define PID_CREDITS					(MAKELONG(CEVT_LPWSTR, 41))
#define PID_LYRICS					(MAKELONG(CEVT_LPWSTR, 42))
#define PID_SUBSCRIPTION_CONTENT_ID	(MAKELONG(CEVT_LPWSTR, 43))
#define PID_PRODUCEDBY				(MAKELONG(CEVT_LPWSTR, 44))
#define PID_USE_COUNT				(MAKELONG(CEVT_UI4, 45))
#define PID_SKIP_COUNT				(MAKELONG(CEVT_UI2, 46))
#define PID_LAST_ACCESSED			(MAKELONG(CEVT_LPWSTR, 47))
#define PID_PARENTAL_RATING			(MAKELONG(CEVT_LPWSTR, 48))
#define PID_META_GENRE_TAG			(MAKELONG(CEVT_UI4, 49))
#define PID_COMPOSER				(MAKELONG(CEVT_LPWSTR, 50))
#define PID_EFFECTIVE_RATING		(MAKELONG(CEVT_UI2, 51))
#define PID_SUBTITLE				(MAKELONG(CEVT_LPWSTR, 52))
#define PID_ORIG_RELEASE_DATE		(MAKELONG(CEVT_LPWSTR, 53))
#define PID_ALBUM_NAME				(MAKELONG(CEVT_LPWSTR, 54))
#define PID_ALBUM_ARTIST			(MAKELONG(CEVT_LPWSTR, 55))
#define PID_MOOD					(MAKELONG(CEVT_LPWSTR, 56))				
#define PID_DRM_STATUS				(MAKELONG(CEVT_UI4, 57))
#define PID_SUB_DESCRIPTION			(MAKELONG(CEVT_LPWSTR, 58))
#define PID_ISCROPPED				(MAKELONG(CEVT_UI4, 59))
#define PID_ISCOLOURE_CORRECTED		(MAKELONG(CEVT_UI4, 60))
#define PID_TOTAL_BITRATE			(MAKELONG(CEVT_UI4, 61))
#define PID_BITRATE_TYPE			(MAKELONG(CEVT_UI4, 62))
#define PID_SAMPLE_RATE				(MAKELONG(CEVT_UI4, 63))
#define PID_NUMBER_OF_CHANNELS		(MAKELONG(CEVT_UI4, 64))		
#define PID_AUDIO_BITDEPTH			(MAKELONG(CEVT_UI4, 65))
#define PID_SCAN_TYPE				(MAKELONG(CEVT_UI4, 66))
#define PID_AUDIO_WAVE_CODEC		(MAKELONG(CEVT_UI4, 67))
#define PID_AUDIO_BITRATE			(MAKELONG(CEVT_UI4, 68))			
#define PID_VIDEO_FOUR_CCCODEC		(MAKELONG(CEVT_UI4, 69))
#define PID_VIDEO_BIT_RATE		    (MAKELONG(CEVT_UI4, 70))
#define PID_FRAMES_PER_THOUSANDS_SEC (MAKELONG(CEVT_UI4, 71))
#define PID_KEY_FRAME_DISTANCE		(MAKELONG(CEVT_UI4, 72))
#define PID_BUFFER_SIZE				(MAKELONG(CEVT_UI4, 73))
#define PID_ENCODING_QUALITY		(MAKELONG(CEVT_UI4, 74))
#define PID_BUY_FLAG				(MAKELONG(CEVT_UI2, 75))	
#define PID_REP_SAMPLE_DATA_TAG_NUM_ELEM	(MAKELONG(CEVT_UI4, 76))

#define NUM_DB_PROPS			75  // should be 75 but we aren't adding association data yet... IMR TODO
#define PROPERTY_COUNT_TASK		76
//----------
			
#define MAX_STRING_LENGTH		129
/**
 * 16 bit wide
 * 
 * All values with bit 15 set to 1	MTP Vendor Extension
 */
typedef enum mdl_MetaGenreTag
{
    NOTUSED=                        0x0000,
    GENERIC_MUSIC_AUDIO_FILE=       0x0001,
    GENERIC_NON_MUSIC_AUDIO_FILE=   0x0011,
    SPOKEN_WORD_AUDIO_BOOK_FILES=   0x0012,
    SPOKEN_WORD_FILES=              0x0013, /**< (non-Audio book)*/
    SPOKEN_WORD_NEWS=               0x0014,
    SPOKEN_WORD_TALK_SHOWS=         0x0015,
    GENERIC_VIDEO_FILE=             0X0021,
    NEWS_VIDEO_FILE=                0x0022,
    MUSIC_VIDEO_FILE=               0x0023,
    HOME_VIDEO_FILE=                0x0024,
    FEATURE_FILM_VIDEO_FILE=        0x0025,
    TELEVISION_SHOW_VIDEO_FILE=     0x0026,
    TRAINING_EDUCATIONAL_VIDEO_FILE=0x0027,
    PHOTO_MONTAGE_VIDEO_FILE=       0x0028,
    GENERIC=                        0x0030 /**< non-audio, non-video*/
} mdl_MetaGenreType;// /** \cond GCC */MAKE_ENUM_16_BITS/** \endcond */;

/*

typedef struct TaskRecord//_PhoneRecord
{
	ushort psz_priority;
	ushort psz_freq;
	FILETIME	ft;
	ulong psz_cmd;
	TCHAR psz_cmd_param[265];
	ulong psz_msg;

}TASKRECORD, *LPTSKRECORD;
*/

/**
 * Each object in the library has a set of properties that are 
 * defined by this structure. 
 * The only property that is unique to an object is 
 * mdl_ObjectPropertiesTag::PersistentUniqueObjectIdentifier, and this is the
 * value that is used to recover all other properties of an object.
 * 
 * Object properties can be accessed using the following functions
 * mdl_SetObjectProperty(), mdl_GetObjectProperty(),
 * mdl_SetAllPropertiesForObject(), mdl_GetAllPropertiesForObject.
 * 
 * The Object Properties are described in \ref MTP_Enhanced.
 * (Appendix I section 2) 
 * Some of the properties exist in two places within the MTP datasets.
 * Where a field exists in the PTP ObjectInfo Dataset under a
 * different name, that name is shown in the comment. The MTP media
 * library will ensure that the values at the different locations
 * remain synchronised.
 * 
 */
typedef struct mdl_ObjectPropertiesTag
{
	TCHAR lpszFullPath[255] ; /**< The absolute filesystem path to the object */

    unsigned int StorageID;    
    mdl_ObjectFormatCodeType ObjectFormat; 
    mdl_ProtectionStatusType ProtectionStatus; 
    UINT64 ObjectSize; /**< called ObjectCompressedSize in the PTP ObjectInfo dataset */
    mdl_AssociationCodeType AssociationType;
    mdl_AssociationDescType AssociationDesc;
    TCHAR lpszObjectFileName[MDL_MAX_STRING_LENGTH];/**< called Filename in the PTP ObjectInfo dataset */
    TCHAR lpszDateCreated[MDL_MAX_STRING_LENGTH];
    TCHAR lpszDateModified[MDL_MAX_STRING_LENGTH];
    TCHAR lpszKeywords[MDL_MAX_STRING_LENGTH];
    unsigned int ParentObject;
    unsigned int PersistentUniqueObjectIdentifier; /**< This is the only field that has a value that is unique
    to a particular object, as such it is the field that is used to index all others */
    TCHAR lpszSyncID[MDL_MAX_STRING_LENGTH];/**< should not be changed by device */
    TCHAR lpszPropertyBag[MDL_MAX_STRING_LENGTH]; /**< defined by host, transparent to device */
    TCHAR lpszName[MDL_MAX_STRING_LENGTH];
    TCHAR lpszCreatedBy[MDL_MAX_STRING_LENGTH];
    TCHAR lpszArtist[MDL_MAX_STRING_LENGTH];
    TCHAR lpszDateAuthored[MDL_MAX_STRING_LENGTH];
    TCHAR lpszDescription[MDL_MAX_STRING_LENGTH]; /**< it is a (wide) string, but it is specified as an array of uint16 - don't know why*/
    TCHAR lpszURLReference[MDL_MAX_STRING_LENGTH];
    TCHAR lpszLanguageLocale[MDL_MAX_STRING_LENGTH];
    TCHAR lpszCopyrightInformation[MDL_MAX_STRING_LENGTH];/**< it is a (wide) string, but it is specified as an array of uint16 - don't know why*/
    TCHAR lpszSource[MDL_MAX_STRING_LENGTH];
    TCHAR lpszOriginLocation[MDL_MAX_STRING_LENGTH];
    TCHAR lpszDateAdded[MDL_MAX_STRING_LENGTH];
    unsigned short NonConsumable;
    unsigned short CorruptUnplayable;
    mdl_ObjectFormatCodeType RepresentativeSampleFormat; /**< ThumbFormat in the PTP ObjectInfo dataset */
    unsigned int RepresentativeSampleSize;  /**< ThumbCompressedSize in the PTP ObjectInfo dataset */
    unsigned int RepresentativeSampleHeight;/**< ThumbPixHeight in the PTP ObjectInfo dataset */
    unsigned int RepresentativeSampleWidth; /**< ThumbPixWidth in the PTP ObjectInfo dataset */
    unsigned int RepresentativeSampleDuration;
    mdl_RepresentativeDataType RepresentativeSampleData;
    unsigned int Width; /**< ImagePixWidth in the PTP ObjectInfo dataset */
    unsigned int Height; /**< ImagePixHeight in the PTP ObjectInfo dataset */
    unsigned int Duration;
    unsigned short Rating;
    unsigned short Track;
    TCHAR lpszGenre[MDL_MAX_STRING_LENGTH];
    TCHAR lpszCredits[MDL_MAX_STRING_LENGTH];
    TCHAR lpszLyrics[MDL_MAX_STRING_LENGTH];
    TCHAR lpszSubscriptionContentID[MDL_MAX_STRING_LENGTH];
    TCHAR lpszProducedBy[MDL_MAX_STRING_LENGTH];
    unsigned int UseCount;
    unsigned short SkipCount;
    TCHAR lpszLastAccessed[MDL_MAX_STRING_LENGTH];
    TCHAR lpszParentalRating[MDL_MAX_STRING_LENGTH];
    enum mdl_MetaGenreTag MetaGenre;
    TCHAR lpszComposer[MDL_MAX_STRING_LENGTH];
    unsigned short EffectiveRating;
    TCHAR lpszSubtitle[MDL_MAX_STRING_LENGTH];
    TCHAR lpszOriginalReleaseDate[MDL_MAX_STRING_LENGTH];
    TCHAR lpszAlbumName[MDL_MAX_STRING_LENGTH];
    TCHAR lpszAlbumArtist[MDL_MAX_STRING_LENGTH];
    TCHAR lpszMood[MDL_MAX_STRING_LENGTH];
    mdl_DrmStatusType DRMStatus;
    TCHAR lpszSubDescription[MDL_MAX_STRING_LENGTH];
    mdl_IsCroppedType IsCropped;
    mdl_IsColorCorrectedType IsColourCorrected;
    unsigned int TotalBitRate;
    mdl_BitRateTypeType BitRateType;
    unsigned int SampleRate;
    mdl_NumberOfChannelsType NumberOfChannels;
    unsigned int AudioBitDepth;
    mdl_ScanTypeType ScanType;
    unsigned int AudioWAVECodec; /**< as specified here http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnwmt/html/registeredfourcccodesandwaveformats.asp */
    unsigned int AudioBitRate;
    unsigned int VideoFourCCCodec;/**< as specified here http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnwmt/html/registeredfourcccodesandwaveformats.asp */
    unsigned int VideoBitRate;
    unsigned int FramesPerThousandSeconds;
    unsigned int KeyFrameDistance;
    unsigned int BufferSize;
    unsigned int EncodingQuality;
    unsigned short BuyFlag; /**< \todo need to check the data type for this property, in the DRM documenation */
} mdl_ObjectPropertiesType, *LP_MTPOBJECT_PROP;
/**
 * Library clients are notified of changes to the library  
 * by calling the callback function specified by their call to 
 * mdl_RegisterLibraryClient() with a pointer to a structure of type
 * #mdl_EventTag as an argument. The structures that this pointer
 * points to will be valid until all clients have returned from 
 * their callback functions.
 * 
 */ 
typedef struct mdl_EventStructTag
{
    mdl_ClientId EventOriginator;
    mdl_EventType EventType; /**< Specifies the type of change that happened to the library */
    mdl_PUOI Object;  /**< The Object that has been changed 
                           (not used in the case of  EventType = #GROUP_DATA_CHANGED | #ALL_OBJECTS_CHANGED) */
    MDL_UINT8 NumPropertiesChanged;  /**< The number of properties that have changed in the Object specified. 
										  Not valid when EventType = #GROUP_DATA_CHANGED | #ALL_OBJECTS_CHANGED |#OBJECT_ADDED*/
    mdl_ObjectPropertyNamesType *PropertiesChanged; /**< Points to the array of properties that have changed in the Object specified 
														 Not valid when EventType = #GROUP_DATA_CHANGED | #ALL_OBJECTS_CHANGED |#OBJECT_ADDED*/
    struct mdl_EventStructTag *NextEvent;  /**< If there were changes to multiple objects then this points 
                                                to another structure defining the change, NextEvent == NULL 
                                                indicates that there are no other changes */
} mdl_EventStructType;

/**
 * Used when calling mdl_RequestLibraryAccess() to specify
 * whether mdl_RequestLibraryAccess() should block if the 
 * library is being accessed by another client.
 * 
 */ 
typedef enum mdl_RequestTag
{
    MDL_BLOCKING,
    MDL_NON_BLOCKING
} mdl_RequestType;

typedef struct mdl_GroupDataTag
{
    MDL_UINT8                    NumGroup1Properties;
    mdl_ObjectPropertyNamesType *Group1Properties;
    MDL_UINT8                    NumGroup2Properties;
    mdl_ObjectPropertyNamesType *Group2Properties;
    MDL_UINT8                    NumGroup3Properties;
    mdl_ObjectPropertyNamesType *Group3Properties;
    MDL_UINT8                    NumGroup4Properties;
    mdl_ObjectPropertyNamesType *Group4Properties;
    MDL_UINT8                    NumGroup5Properties;
    mdl_ObjectPropertyNamesType *Group5Properties;
    MDL_UINT8                    NumGroup6Properties;
    mdl_ObjectPropertyNamesType *Group6Properties;
    MDL_UINT8                    NumGroup7Properties;
    mdl_ObjectPropertyNamesType *Group7Properties;
    MDL_UINT8                    NumGroup8Properties;
    mdl_ObjectPropertyNamesType *Group8Properties;
} mdl_GroupDataType;

#endif//_MTP_CEDBDEFS_H