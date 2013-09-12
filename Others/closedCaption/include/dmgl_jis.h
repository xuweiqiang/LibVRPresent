#ifndef _DMGL_JIS_H_
#define _DMGL_JIS_H_

#include "isdbt_cc.h"

//C0
#define NUL 	0x00
#define BEL		0x07
#define APB		0x08
#define	APF		0x09
#define APD		0x0A
#define APU		0x0B
#define CS		0x0C
#define APR		0x0D
#define LS1		0x0E
#define LS0		0x0F

#define PAPF	0x16
#define CAN		0x18
#define SS2		0x19
#define ESC		0x1B
#define APS		0x1C
#define SS3		0x1D
#define RS		0x1E
#define US		0x1F

//GL
#define SP		0x20
#define DEL		0x7F

//C1
#define BKF		0x80
#define RDF		0x81
#define GRF		0x82
#define YLF		0x83
#define BLF		0x84
#define MGF		0x85
#define CNF		0x86
#define WHF		0x87
#define SSZ		0x88
#define MSZ		0x89
#define NSZ		0x8A
#define TIME    0x9D 
//#define SZX	0x8B


#define FLC		0x91
#define TIM		0x9D

/*
typedef enum {
	KANJI,
	ALPHANUMERIC,
	HIRAGANA,
	KATAKANA,
	MOSAICA,
	MOSAICB,
	MOSAICC,
	MOSAICD,
	P_ALPHANUMERIC,
	P_HIRAGANA,
	P_KATAGANA,
	JISX0201KATAGANA,
	JISCHANGEKNAJI1,
	JISCHANGEKNAJI2,
	ADDSIGN,
	DRCS_FONT //user defined, get from stream
} GSetName;
*/

typedef enum {
	G0,
	G1,
	G2,
	G3
} CodeElement;

int CharacterDecode_JIS(unsigned int *pDst, int dst_length, unsigned char* pSrc, int src_length, GSetName gl, GSetName gr, void *param);
int JIStoSJIS(unsigned char* pShiftJis, int iSjisLen, unsigned char* pJis, int iJisLen, GSetName gl, GSetName gr, void *param);
int JIStoUTF16(unsigned short* pOut, int outlen, unsigned char* pInput, int inlen, GSetName gl, GSetName gr, void *param);

/*
internal control code, start with 30000
*/
typedef enum {
	I_CS = (1<<20),
	I_APR,
	I_SP,
	I_DEL,
	I_BKF,
	I_RDF,
	I_GRF,
	I_YLF,
	I_BLF,
	I_MGF,
	I_CNF,
	I_WHF,
	I_DRCS, //combo code ==> width, height, data address
	I_FLC_ON,
	I_FLC_OFF, 
	I_TIM, //combo code ==> time
	I_MSZ,
	I_NSZ
} InternalControlCode;


#endif //_DMGL_JIS_H_


