#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if defined(WIN32) || defined(WINCE)
#include <windows.h>
#else
#include <sys/time.h>
#endif

#include "y420rgbAPI.h"


#define INPUT_FILE     "h300.yuv"
#define OUTPUT_FILE    "test.yuv"

#ifdef OUTPUT_FILE
#define TEST_LOOP 1
#else
#define TEST_LOOP 10000
#endif

#ifdef INPUT_FILE
#define SRC_WIDTH      320
#define SRC_HEIGHT     240
#define SRC_PITCH	   32
#else
#define SRC_WIDTH      640
#define SRC_HEIGHT     480
#endif

#define DST_DIS_WIDTH  800
#define DST_DIS_HEIGHT 480
#define DST_WIDTH      1024
#define DST_HEIGHT     600


#if !defined(WIN32) && !defined(WINCE)
long GetTickCount(void)
{
	struct timeval my_timeval;
	if (gettimeofday(&my_timeval, NULL))
		return 0;
	return (long)(my_timeval.tv_sec * 1000) + (my_timeval.tv_usec / 1000);

}
#endif


int main(int argc, char *argv[])
{
    int i;
    unsigned char * src[3];
    int src_pitch[3];
    unsigned char * dst;
    CC_RECT src_rect, dst_rect, dst_dis_rect;
    FILE * in = 0;
    FILE * out = 0;
    void * pHandle = 0;
    
	unsigned long start_time = 0;
	unsigned long end_time = 0;
	unsigned long total_time = 0;
    double fps = 0;
    double mspf = 0;
    
    src_rect.left = 0;
    src_rect.top = 0;
    src_rect.right = SRC_WIDTH;
    src_rect.bottom = SRC_HEIGHT;
    src_pitch[0] = SRC_WIDTH + SRC_PITCH;
    src_pitch[1] = SRC_WIDTH / 2 + SRC_PITCH;
    src_pitch[2] = SRC_WIDTH / 2 + SRC_PITCH;
    
    dst_rect.left = dst_rect.top = 0;
    dst_rect.right = DST_WIDTH;
    dst_rect.bottom = DST_HEIGHT;
	
	dst_dis_rect.left = (DST_WIDTH - DST_DIS_WIDTH) / 2;
	dst_dis_rect.right = dst_dis_rect.left + DST_DIS_WIDTH;
	dst_dis_rect.top = (DST_HEIGHT - DST_DIS_HEIGHT) / 2;
	dst_dis_rect.bottom = dst_dis_rect.top + DST_DIS_HEIGHT;
	
    //dst_pitch = DST_WIDTH * 2;
    
#ifdef INPUT_FILE    
    in = fopen(INPUT_FILE,"rb");
    if (!in)
    {
            fprintf(stderr,"input file not open!!!\n");
            goto end;
    }
#endif
    src[0] = (unsigned char *)malloc(SRC_WIDTH * SRC_HEIGHT + SRC_PITCH * SRC_HEIGHT);
    src[1] = (unsigned char *)malloc(SRC_WIDTH * SRC_HEIGHT / 4 + SRC_PITCH * SRC_HEIGHT);
    src[2] = (unsigned char *)malloc(SRC_WIDTH * SRC_HEIGHT / 4 + SRC_PITCH * SRC_HEIGHT);
#ifdef INPUT_FILE    
    fseek(in, SRC_WIDTH * SRC_HEIGHT * 3 / 2 * 240, SEEK_SET);
	for (int i = 0; i < SRC_HEIGHT; i++)
	{
		fread(src[0] + i * (SRC_PITCH + SRC_WIDTH),1,SRC_WIDTH,in);
	}
	for (int i = 0; i < SRC_HEIGHT / 2; i++)
	{
		fread(src[1] + i * (SRC_PITCH + SRC_WIDTH / 2),1,SRC_WIDTH / 2,in);
	}
	for (int i = 0; i < SRC_HEIGHT / 2; i++)
	{
		fread(src[2] + i * (SRC_PITCH + SRC_WIDTH / 2),1,SRC_WIDTH / 2,in);
	}
#else    
    memset(src[0], 0, SRC_WIDTH * SRC_HEIGHT / 2);
    memset(src[0] + (SRC_WIDTH * SRC_HEIGHT / 2),255, SRC_WIDTH * SRC_HEIGHT / 2);
    memset(src[1], 128, SRC_WIDTH * SRC_HEIGHT / 4);
    memset(src[2], 128, SRC_WIDTH * SRC_HEIGHT / 4);
#endif

    dst = (unsigned char *)malloc(DST_WIDTH * DST_HEIGHT * 4 + 64);
    memset(dst,0,DST_WIDTH * DST_HEIGHT * 4);
    total_time = 0;
    
    if (I420_RGB_Init(&pHandle,&src_rect,src_pitch,&dst_rect,&dst_dis_rect, 2))
    {
       goto end;
    }
    
    for (i = 0; i < TEST_LOOP; i++)
    {
        start_time = GetTickCount();
        //I420_R5G5B5_MMX(pHandle,src[0],src[1],src[2],(uint16_t *)dst);
        //I420_R5G6B5_MMX(pHandle,src[0],src[1],src[2],(uint16_t *)dst);
        //I420_A8R8G8B8_MMX(pHandle,src[0],src[1],src[2],(uint32_t *)dst);        
        //I420_R8G8B8A8_MMX(pHandle,src[0],src[1],src[2],(uint32_t *)dst);
        //I420_B8G8R8A8_MMX(pHandle,src[0],src[1],src[2],(uint32_t *)dst);
        //I420_A8B8G8R8_MMX(pHandle,src[0],src[1],src[2],(uint32_t *)dst);
        //I420_R5G5B5_SSE2(pHandle,src[0],src[1],src[2],(uint16_t *)dst);
        I420_R5G6B5_SSE2(pHandle,src[0],src[1],src[2],(uint16_t *)dst);
        //I420_A8R8G8B8_SSE2(pHandle,src[0],src[1],src[2],(uint32_t *)dst);
        //I420_R8G8B8A8_SSE2(pHandle,src[0],src[1],src[2],(uint32_t *)dst);
        //I420_B8G8R8A8_SSE2(pHandle,src[0],src[1],src[2],(uint32_t *)dst);
        //I420_A8B8G8R8_SSE2(pHandle,src[0],src[1],src[2],(uint32_t *)dst);
       
        end_time = GetTickCount();
        total_time += end_time - start_time;
   }
    
#ifdef OUTPUT_FILE    
    out = fopen(OUTPUT_FILE,"wb");
    if (out)
    {
            fwrite(dst,1,DST_WIDTH * DST_HEIGHT * 4,out);
            fclose(out);
    }
#endif
	if (total_time > 0)
	{
		fps = TEST_LOOP * 1000.0 / total_time;
		mspf = (double)total_time / TEST_LOOP;
	}
	printf("Conv %d frames used time %lums FPS=%04f ms/frame=%04f\n", TEST_LOOP, total_time, fps, mspf);
 	I420_RGB_unInit(pHandle);
  

end:
    if (src[0]) free(src[0]);
    if (src[1]) free(src[1]);
    if (src[2]) free(src[2]);
    if (dst) free(dst);
#ifdef INPUT_FILE
    if (in)
    {
       fclose(in);
    } 
#endif        
	printf("Input any key to quit\n");
	getchar();
     return 0;
}
