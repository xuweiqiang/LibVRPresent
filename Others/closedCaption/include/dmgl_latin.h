#ifndef _DMGL_LATIN_H_
#define _DMGL_LATIN_H_

#include "isdbt_cc.h"

/*
typedef enum {	
	ALPHANUMERIC_LATIN,
	LATIN_EXTENSION,
	DRCS_FONT_LATIN, //user defined, get from stream
	SPECIAL_CHARACTERS_SET
} GSetName_Latin;
*/

int CharacterDecode_Latin(unsigned int *pDst, int dst_length, unsigned char* pSrc, int src_length, GSetName_Latin gl, GSetName_Latin gr, void *param);
int PORtoUTF16(unsigned short* unicode, int dst_length, unsigned char* pSrc, int src_length, GSetName_Latin gl, GSetName_Latin gr, void *param);


#endif //_DMGL_LATIN_H_


