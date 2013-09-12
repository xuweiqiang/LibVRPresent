//********************************************************************
//VEU module hader file
//		by Benson Chuang
//
//********************************************************************
//Component:	
//  The VEU driver developed by third party, this header file is used for VEU.
// Some of source is modefied by Benson Chuang
//********************************************************************
#ifndef __PRE_DEFINE_H
#define __PRE_DEFINE_H

/******************************************************************************
* INCLUDE FILES
******************************************************************************/

/******************************************************************************
* CONSTANT DEFINITIONS
******************************************************************************/
#define E_OK					0
#define E_ERROR                 -1
#define E_NO_MEM			    -100  
#define E_TIMEOUT               -50
#define	E_NO_RESOURCE			-102
#define	E_INVALID_VALUE			-103

/*---- internal error class ----*/
#define E_SYS			(-5)			/* system error						*/

/*---- no support error class ----*/
#define E_NOSPT			(-9)			/* no support function				*/
#define E_RSFN			(-10)			/* reserved function code number	*/
#define E_RSATR			(-11)			/* reserved attribute code number	*/

/*---- parameter error class ----*/
#define E_PAR			(-17)			/* parameter error					*/
#define E_ID			(-18)			/* reserved id number				*/

/*---- context error class ----*/
#define E_CTX			(-25)			/* context error					*/
#define E_MACV			(-26)			/* memory access violation			*/
#define E_OACV			(-27)			/* object access violation			*/
#define E_ILUSE			(-28)			/* service call illegal use			*/

/*---- resource insufficiency error class ----*/
#define E_NOMEM			(-33)			/* no memory						*/
#define E_NOID			(-34)			/* no ID							*/

/*---- object status error class ----*/
#define E_OBJ			(-41)			/* object status error				*/
#define E_NOEXS			(-42)			/* object non existent				*/
#define E_QOVR			(-43)			/* queuing over flow				*/

/*---- wait release error class ----*/
#define E_RLWAI			(-49)			/* wait status forced release		*/
#define E_TMOUT			(-50)			/* time out							*/
#define E_DLT			(-51)			/* delete object					*/
#define E_CLS			(-52)			/* status change of wait object		*/

/*---- warning class ----*/
#define E_WBLK			(-57)			/* non blocking receipt				*/
#define E_BOVR			(-58)			/* buffer over flow					*/

#define TA_NULL			0				/* no object attribute specify		*/

#define TMO_POL			0				/* polling							*/
#define TMO_FEVR		(-1)			/* forever wait						*/
#define TMO_NBLK		(-2)			/* non blocking						*/

#define E_OK			0				/* normal end						*/

/******************************************************************************
* MACRO DEFINITIONS
******************************************************************************/
#ifndef  NULL
	#ifdef  __cplusplus
		#define  NULL           (0)
	#else
		#define  NULL           ((void *)0)
	#endif
#endif

#define MERCD(ercd) ((ER)((B)(ercd)))

#define SERCD(ercd) ((ercd) >> 8)

/******************************************************************************
* TYPE DEFINITIONS
******************************************************************************/
typedef char			B;				/* signed 8 bit integer				*/
typedef short			H;				/* signed 16 bit integer			*/
typedef long			W;				/* signed 32 bit integer			*/

typedef unsigned char	UB;				/* unsigned 8 bit integer			*/
typedef unsigned short	UH;				/* unsigned 16 bit integer			*/
typedef unsigned long	UW;				/* unsigned 32 bit integer			*/

typedef char			VB;				/* variable data type (8 bit)		*/
typedef short			VH;				/* variable data type (16 bit)		*/
typedef long			VW;				/* variable data type (32 bit)		*/

typedef void			*VP;			/* pointer to variable data type	*/
typedef void			(*FP)();		/* program start address			*/

typedef W				FN;				/* function code					*/
typedef W				ER;				/* error code						*/
typedef H				ID;				/* object ID (xxxid)				*/
typedef UW				ATR;			/* attribute						*/
typedef UW				STAT;			/* object status					*/
typedef UW				MODE;			/* action mode						*/
typedef H				PRI;			/* task priority					*/
//typedef UW				SIZE;			/* memory area size					*/

typedef W				TMO;			/* time out							*/
typedef UW				RELTIM;			/* relative time					*/

typedef struct	systim {				/* system clock						*/
			UH			utime;				/* current date/time (upper)	*/
/***		VH			_Hrsv;				   reserved					  ***/
			UW			ltime;				/* current date/time (lower)	*/
		} SYSTIM;

typedef W				VP_INT;			/* integer or pointer to var. data	*/

typedef W				ER_BOOL;		/* error code or bool value			*/
typedef W				ER_ID;			/* error code or object ID			*/
typedef W				ER_UINT;		/* error code or unsigned integer	*/

enum {
	E_UAV_START 			= -200,
	E_UAV_PARAM 			= -201,
	E_UAV_NO_START_CODE 	= -202,
	E_UAV_DECODE_FAIL		= -203,
	E_UAV_NO_FRAME			= -204,
	
} ;

typedef signed char 		int8;
typedef signed short int 	int16;
typedef signed int 			int32;

typedef unsigned char 		uint8;
typedef unsigned short int 	uint16;
typedef unsigned int 		uint32;

typedef signed char 		sint8;
typedef signed short int 	sint16;
typedef signed int 			sint32;

typedef long long 			int64;
typedef unsigned long long 	uint64;


typedef int					boolean_t ;

typedef int					ret_t ;
typedef unsigned int		ID_t ;

typedef uint32				addr_t ;

#ifndef _SIZE_T
#define _SIZE_T
//typedef unsigned long size_t;
#endif

//typedef unsigned short		wchar_t ;

typedef int (*fn_int_t)(void);      /* integer function type */
typedef void (*fn_void_t)(void);    /* void function type */
typedef void (*fn_voids_t)(char *);    /* void function type */


#endif	//__PRE_DEFINE_H

