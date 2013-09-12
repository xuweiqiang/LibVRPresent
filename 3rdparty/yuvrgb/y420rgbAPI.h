#ifndef __Y420RGBAPI_H__
#define __Y420RGBAPI_H__
#include <stdint.h>
typedef struct _tagRECT
{ 
	uint32_t left;
	uint32_t top;
	uint32_t right;
	uint32_t bottom;
} CC_RECT;


#ifdef __cplusplus
extern "C" {
#endif

int I420_RGB_Init(void ** pHandle, CC_RECT * src_rect, int * src_pitch, CC_RECT * dst_rect, CC_RECT * dst_dis_rect, int BPP);
void I420_RGB_unInit(void * pHandle);

void I420_R5G5B5_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint16_t *p_d);
void I420_R5G6B5_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint16_t *p_d);
void I420_A8R8G8B8_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d);
void I420_R8G8B8A8_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d);
void I420_B8G8R8A8_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d);
void I420_A8B8G8R8_MMX(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d);

void I420_R5G5B5_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint16_t *p_d);
void I420_R5G6B5_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint16_t *p_d);
void I420_A8R8G8B8_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d);
void I420_R8G8B8A8_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d);
void I420_B8G8R8A8_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d);
void I420_A8B8G8R8_SSE2(void * pHandle, uint8_t *p_y, uint8_t *p_u, uint8_t *p_v, uint32_t *p_d);


#ifdef __cplusplus
}
#endif

#endif /* __Y420RGBAPI_H__ */
