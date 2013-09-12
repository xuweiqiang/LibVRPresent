/*************************************************************************
**
** STGLIB, Accelerated 2D Graphics Library for Cartesio Plus SGA Engine.
**
** Copyright (C) 2009-2010 STMicroelectronics.  All rights reserved.
**
** This software is the confidential and/or proprietary information of
** STMicroelectronics. You shall not disclose such information and shall
** use it only in accordance with the terms of the license agreement you
** entered into with STMicroelectronics.
**
*************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stglib.h>
#include <sys/time.h>

#include "data/fontMap.h"
#include "data/fontmap11.h"
#include "data/fontmapComicSS18.h"

#define SGA_FREQ		208000000ul
#define ROTATION_STEP	(M_PI / 50.0f)
#define RX				220.0f
#define RY				80.0f
#define T1				(M_PI * 0.25f)

unsigned int width,height;
unsigned int fps = 0;

struct image;

enum {
	NEWSPAPER, YELLOWPAGES, RADIO, TRAFFIC
};

typedef struct {
	STHBITMAP  bitmap;
	int zCoord, yCoord, xCoord;
} t_menuItem;

//static struct image *picture0;
static struct image *picture1;
static struct image *picture2;
static struct image *picture3;

static STHBITMAP hbitmapnews;
static STHBITMAP hbitmapradio;
static STHBITMAP hbitmapyellowpages;
static STHBITMAP hbitmaptraffic;
static t_sgaFont font;


static STHDC     hdc         = NULL;
static STHFONT   hfont       = NULL;
static STHBITMAP hfontbitmap = NULL;

static t_menuItem traffic, radio, newspaper, yellowpages;
static float angle = 0.0f;
static int drawOrder[4];
static int dir = 1;

static int screen_width  = 0;
static int screen_height = 0;
static bool triple;

static void init_application(int *argc, char **argv[]);
static void exit_application(int status);
static void init_stglib(void);
static void exit_stglib(void);
static void update();

/*************************************************************************
*
* Load picture from TGA file.
*
*************************************************************************/

enum image_format {
	EMPTY,
	RGB,
	RGBA,
	GREYSCALE,
	YUV420,
	YUV422,
	UNDEFINED
};

struct image {
	int width;
	int height;
	int	pixel_depth;
	int alpha_depth;
	enum image_format format;
	int size;
	int length;
	unsigned char *data;
};

int fd;
struct image *load_image_from_tga(const char *fileName, int w, int h, enum image_format fmt)
{
	struct image *image;
	int size;

	fd = open(fileName, O_RDWR);

	if (fd < 0) {
		perror("Unable to load file\n");
		return NULL;
	}

	switch (fmt) {
	case YUV420:
		size = w * h * 3 / 2;
		break;
	case YUV422:
		size = w * h * 2;
		break;
	default:
		close(fd);
		return false;
	}

	printf("opening file %s\n",fileName);
	image = (struct image *)calloc(1, sizeof(struct image));
	image->width = w;
	image->height = h;
	image->size = size;
	image->pixel_depth = 2;

	image->length = lseek(fd, 0UL, SEEK_END);
	lseek(fd, 0UL, SEEK_SET);

	image->data = (unsigned char *)mmap(0, image->length, PROT_READ, MAP_SHARED, fd, 0);
	if(image->data == (void *)MAP_FAILED) {
		printf("map file failed\n");
		close(fd);
		return false;
	}
	printf("File length %d\n",image->length);

	// swap BGR(A) to RGB(A)

	return image;
}

void release_image(struct image *image)
{
	free(image);
	close(fd);
}

/*************************************************************************
*
* Main application.
*
*************************************************************************/

int	main(int argc, char *argv[])
{
	int run = 300;

	width = atoi(argv[1]);
	height = atoi(argv[2]);
	fps = atoi(argv[3]);

	printf("width:%d,%d,fps:%d\n",width,height,fps);
	init_stglib();

	/* Main loop */
		update();

//		events->WaitForEventWithTimeout(events, 0, 100);

	exit_stglib();

	exit_application(0);

	return 0;
}

static void	exit_application(int status)
{
	/* Terminate application */
	exit(status);
}

/*************************************************************************
*
* STGLIB initialization.
*
*************************************************************************/

static void init_stglib(void)
{
	int offset;
	void *address;
	void *ptr;
	int pitch;

	hdc = STCreateDC((HWND)1, 2, &address);

	font.charNum			= 43;		// The number of chars in the font
	font.charStep			= 21 * 16;	// The distance in pixels between two characters (X axis)
	font.charHeight			= 21;		// The Height of one character
	font.charWidth			= 16;		// The Width of each character
	font.fontTransparency	= 1;
	font.color				= 0xFFFFFF;
	font.alphaMax			= 0xFF;
	font.alphaMin			= 0xFF;

	hfontbitmap = STCreateBitmap(hdc, 16, 1911, SGA_DIS_FMT_MONO_8BPP, &fontMap);
	hfont = STCreateFont(hdc, &font, hfontbitmap);

//	picture0 = load_image_from_tga("/mnt/cmmbvideo_qvga_ref_320x240.yuv", width, height, YUV420);
//	if (!picture0) {
//		printf("Unable to load file %s\n", "/mnt/cmmbvideo_qvga_ref_320x240.yuv");
//		exit(-1);
//	}

}

static void exit_stglib(void)
{
//	release_image(picture0);

	STDestroyBitmap(hdc, hfontbitmap);
	STDestroyFont(hdc, hfont);

	STDestroyVideoBuffers(hdc);
	STReleaseDC(hdc);
}

/*************************************************************************
*
* Draw and update screen.
*
*************************************************************************/
#define	ASSERT(x)	{ int ret;\
			ret = x;\
			if(ret != 1) { \
				printf("Function at line %d failed\n",__LINE__); \
				return; \
			} \
		}
			
				
static void	update()
{
	t_sgaStatistics stats; 
	int count;
	unsigned long clk;
	float frame_rate;
	t_sgaFont font;
	void *buffer_out_Address;
	int bufferSize;
	int gYUV_BufferCount = 1;
	unsigned long read = 0;
	struct timespec ts,ts1;
	int i;

	ASSERT(STSetScreenOffset(hdc, 0, 0));

	ASSERT(STCreateVideoBuffers(hdc, width, height, SGA_DIS_FMT_YUV420, &gYUV_BufferCount, &hbitmapnews));
	// Now compute new positions

	ASSERT(STGetVideoBufferAddress(hdc, hbitmapnews, &buffer_out_Address));
        ASSERT(STGetVideoBufferSize(hdc, hbitmapnews, &bufferSize));

	//STTextOut(hdc, 10, 10, "Cartesio Plus Carousel Demo", hfont);

	struct timespec startTick ;
	struct timespec currTick ;
	unsigned long gframes = 0;
	clock_gettime(CLOCK_MONOTONIC, &startTick);
	
	i = 0;
	while(1){//(read < picture0->length) {
	//	{
	//		unsigned long tick = (currTick.tv_sec-startTick.tv_sec)*1000 + (currTick.tv_nsec-startTick.tv_nsec)/1000000;
	//		if(tick <(1000/fps)*gframes)
	//			usleep((1000/fps)*gframes - tick);
	//	}
		unsigned long tick = (currTick.tv_sec-startTick.tv_sec)*1000+ (currTick.tv_nsec-startTick.tv_nsec)/1000000;
		if(tick <gframes*1000/fps)
			usleep(((gframes*1000/fps) - tick)*1000);

		int scalex = 1.5;
		int scaley = 1.1;
		//const int w0 = (int)(scalex * picture0->width);
		//const int h0 = (int)(scaley * picture0->height);
		//const int x0 = (480-w0) >> 1;
		//const int y0 = (272-h0) >> 1;
		const int x0 = 0;
		const int y0 = 0;

		clock_gettime(CLOCK_MONOTONIC, &ts);
		memset(buffer_out_Address, rand(), width*height*3/2);
		clock_gettime(CLOCK_MONOTONIC, &ts1);
		printf("After memcpy %lu  secs : %lu nanosecs\n",ts1.tv_sec-ts.tv_sec,ts1.tv_nsec-ts.tv_nsec);

		clock_gettime(CLOCK_MONOTONIC, &ts);
		ASSERT(STSetVideoBufferWindow(hdc, hbitmapnews, 0, 0, 800, 480));

		ASSERT(STRenderVideoBuffer(hdc, hbitmapnews, 0, width, height, 0, width*height, width*height*5/4, width));
		clock_gettime(CLOCK_MONOTONIC, &ts1);
		printf("After Render %lu secs : %lu nanosecs\n",ts1.tv_sec-ts.tv_sec,ts1.tv_nsec-ts.tv_nsec);

	//	read+=picture0->size;
		i++;
		if(i==4)
			i=0;

		gframes++;
		clock_gettime(CLOCK_MONOTONIC, &currTick);
		tick = (currTick.tv_sec-startTick.tv_sec)*1000+ (currTick.tv_nsec-startTick.tv_nsec)/1000000;
		printf("fps:%ld \n", gframes*1000/tick) ;
	}
}

/* End */
