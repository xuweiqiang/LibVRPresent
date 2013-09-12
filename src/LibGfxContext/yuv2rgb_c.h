#ifndef __YUV2RGB_C_H__
#define __YUV2RGB_C_H__
#ifdef __cplusplus
extern "C" {
#endif

	int yuv2rgb_c_init(int isRgb, int bpp, int fullRange, int brightness, int contrast, int saturation);
	int yuv2rgb_c_uninit();
	int yuv2rgb_c_32(unsigned char * src[], int srcStride[], int srcSliceY, int srcSliceH, unsigned char * dst[], int dstStride[], int dst_w);
	int yuv2rgb_c_24_rgb(unsigned char * src[], int srcStride[], int srcSliceY, int srcSliceH, unsigned char * dst[], int dstStride[], int dst_w);
	int yuv2rgb_c_24_bgr(unsigned char* src[], int srcStride[], int srcSliceY, int srcSliceH, unsigned char * dst[], int dstStride[], int dst_w);
	int yuv2rgb_c_16(unsigned char* src[], int srcStride[], int srcSliceY, int srcSliceH, unsigned char* dst[], int dstStride[], int dst_w);
	
#ifdef __cplusplus
}
#endif

#endif /*  __YUV2RGB_C_H__  */
