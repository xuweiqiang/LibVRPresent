
#ifndef __STREAMINPUT_H__
#define __STREAMINPUT_H__


#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int StreamInitBuffer  (unsigned char **ppBsBuffer, unsigned char **ppBsCurByte, int *bsByteLen);
int StreamReloadBuffer(unsigned char *pBsBuffer, unsigned char **ppBsCurByte, int *bsByteLen, FILE *stream);
int CheckReloadBuffer (unsigned char *pBsBuffer, unsigned char **ppBsCurByte, int *bsByteLen, FILE *stream);
int StreamFreeBuffer  (unsigned char *pBsBuffer);


#ifdef __cplusplus
}
#endif

#endif // __STREAMINPUT_H__
