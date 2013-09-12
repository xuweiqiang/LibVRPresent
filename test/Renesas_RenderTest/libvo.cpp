//********************************************************************
//Renesas Video Output
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Decoder : libvo
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
//I didn't implement error returning mechanism, please carefully use this library.
#include "stdafx.h"
#include <windows.h> /* for WinCE */
#include <pkfuncs.h> /* for WinCE */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "sys.h"
#include "librb.h"
#include "libvo.h"
#include "player.h"
#include "veu.h"
#include <ceddk.h>
#include "VEU2H_ioctl.h"
#include "pre_define.h"
#include "shmdr_vio_drv.h"

//**************************************
// Global variables
//**************************************
static HANDLE	_hddVEU1=NULL;
static RB		_video_rb;
static VO_RB	_frame_rb;
static S_UGE_DRAW_EX	_veu_srcsetting;
static S_UGE_DRAW_EX	_veu_dstsetting;

//**************************************
// Function   : VO_init
// Description
//	The entry point of video outout unit
//**************************************
void VO_init(VIDEO_CONTEXT *video_info)
{
	//Init VEU
	_hddVEU1=CreateFile(TEXT("VEU1:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
	memset( &_frame_rb.frame_info , 0 , sizeof(VO_FRAMEINFO) * VO_BUFFER_FRAME );
	
	VO_sub_rb_init();
	return;
}

//**************************************
// Function   :VO_deinit
// Description
//	
//**************************************
void VO_deinit(void)
{
	unsigned long i;

	VO_sub_rb_deinit();
	if(_hddVEU1)
	{
		CloseHandle(_hddVEU1);
		_hddVEU1=NULL;
	}
	for( i = 0 ; i < _frame_rb.frame_rb_num ; i ++)
		FreePhysMem(_frame_rb.frame_info[i].address);
	return;
}
//**************************************
// Function   :VO_set_video_startpos
// Description
//	
//**************************************
void VO_set_video_startpos(unsigned long x , unsigned long y)
{
	_veu_dstsetting.xoffset = x;//VO_NORMAL_POS_X;	
	_veu_dstsetting.yoffset = y;//VO_NORMAL_POS_Y;	
}

//**************************************
// Function   :VO_set_video_mode_change
// Description
//	
//**************************************
void VO_set_video_mode_change(unsigned long in_w, unsigned long in_h , unsigned long in_mem_w , unsigned long out_w, unsigned long out_h , unsigned long out_mem_w , unsigned long buff_num)
{
	DWORD	nswap = 0x67;

	_veu_srcsetting.x = 0;	// not used
	_veu_srcsetting.y = 0;	// not used
	_veu_srcsetting.xoffset = 0;
	_veu_srcsetting.yoffset = 0;
	_veu_srcsetting.width_mem = in_mem_w;
	_veu_srcsetting.width = in_w;
	_veu_srcsetting.height = in_h;
	_veu_srcsetting.format = VIO_YUV420;		//VIO_YUV420

	_veu_dstsetting.x = 0;	// not used
	_veu_dstsetting.y = 0;	// not used
	//_veu_dstsetting.xoffset = 0;//VO_NORMAL_POS_X;	
	//_veu_dstsetting.yoffset = 0;//VO_NORMAL_POS_Y;	
	_veu_dstsetting.width_mem = out_mem_w;
	_veu_dstsetting.width = out_w;
	_veu_dstsetting.height = out_h;
	_veu_dstsetting.format = VIO_RGB_06_OUT;
	_veu_dstsetting.buff1 = (unsigned char*)TO_CACHE(0xAF000000);
	DeviceIoControl(_hddVEU1,IOCTL_VEU_SWAPVALUE, (LPVOID)&nswap, sizeof(nswap), NULL, 0, NULL, NULL);
}

//**************************************
// Function   :VO_set_video_parameter
// Description
//	
//**************************************
void VO_set_video_parameter(unsigned long in_w, unsigned long in_h , unsigned long in_mem_w , unsigned long out_w, unsigned long out_h , unsigned long out_mem_w , unsigned long buff_num)
{
	int i;
	DWORD	nswap = 0x67;

	_veu_srcsetting.x = 0;	// not used
	_veu_srcsetting.y = 0;	// not used
	_veu_srcsetting.xoffset = 0;
	_veu_srcsetting.yoffset = 0;
	_veu_srcsetting.width_mem = in_mem_w;
	_veu_srcsetting.width = in_w;
	_veu_srcsetting.height = in_h;
	_veu_srcsetting.format = VIO_YUV420;		//VIO_YUV420

	_veu_dstsetting.x = 0;	// not used
	_veu_dstsetting.y = 0;	// not used
	//_veu_dstsetting.xoffset = 0;//VO_NORMAL_POS_X;	
	//_veu_dstsetting.yoffset = 0;//VO_NORMAL_POS_Y;	
	_veu_dstsetting.width_mem = out_mem_w;
	_veu_dstsetting.width = out_w;
	_veu_dstsetting.height = out_h;
	_veu_dstsetting.format = VIO_RGB_06_OUT;
	_veu_dstsetting.buff1 = (unsigned char*)TO_CACHE(0xAF000000);
	DeviceIoControl(_hddVEU1,IOCTL_VEU_SWAPVALUE, (LPVOID)&nswap, sizeof(nswap), NULL, 0, NULL, NULL);

	//attach the frame buffer
	for( i = 0 ; i < VO_BUFFER_FRAME ; i ++)
	{
		_frame_rb.frame_info[i].address = (unsigned char*)AllocPhysMem( in_w * in_h * VO_FRMAE_SIZE_PIXEL , PAGE_EXECUTE_READWRITE , 0 , 0 , (unsigned long*)&_frame_rb.frame_info[i].phy_address);
		if( _frame_rb.frame_info[i].address == NULL ){
			//init the ring buffer
			VO_sub_rb_set_buffernum(i-1);
			break;
		}
		VO_sub_rb_set_buffernum( VO_BUFFER_FRAME );
	}

	return;
}

//**************************************
// Function   :VO_write
// Description
//	Store the image in ring buffer
//**************************************
bool VO_write(unsigned char *y , unsigned char *c ,double dts)
{
	int i;
	DWORD rc;
#if VO_RINGBUFFERLOCK_MODE == 0
	rc = WaitForSingleObject(_frame_rb.handle , INFINITE );
	if( rc == WAIT_TIMEOUT)
		return false;
#elif VO_RINGBUFFERLOCK_MODE == 1
	EnterCriticalSection(&_frame_rb.handle);
#endif

	while( VO_sub_rb_isfull() )
	{
		//Sleep(0);
	}
		
	i = VO_sub_rb_enqueue();

	memcpy(_frame_rb.frame_info[i].address , y , _veu_srcsetting.height * _veu_srcsetting.width);
	memcpy(_frame_rb.frame_info[i].address + _veu_srcsetting.height * _veu_srcsetting.width , c , _veu_srcsetting.height * _veu_srcsetting.width /2);

	//Save dts
	_frame_rb.frame_info[i].dts = dts;

#if VO_RINGBUFFERLOCK_MODE == 0
	ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);
#elif VO_RINGBUFFERLOCK_MODE == 1
	LeaveCriticalSection(&_frame_rb.handle);
#endif
	return true;
}

//**************************************
// Function   :VO_read
// Description
//	Output the frame to the panel
//**************************************
bool VO_read(void)
{
	unsigned long i;
	DWORD rc;
	//static unsigned long last_frame_num;
#if VO_RINGBUFFERLOCK_MODE == 0
	rc = WaitForSingleObject(_frame_rb.handle , 1 );
	if( rc == WAIT_TIMEOUT)
		return false;
#elif VO_RINGBUFFERLOCK_MODE == 1
	EnterCriticalSection(&_frame_rb.handle);
#endif

#if 1
	if( VO_sub_rb_isempty() ){
#if VO_RINGBUFFERLOCK_MODE == 0
		ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);
#elif VO_RINGBUFFERLOCK_MODE == 1
		LeaveCriticalSection(&_frame_rb.handle);
#endif
		return false;
	}
#else
	while( VO_sub_rb_isempty() );
#endif
	i = VO_sub_rb_dequeue();
	_veu_srcsetting.buff1=_frame_rb.frame_info[i].phy_address;
	_veu_srcsetting.buff2=_frame_rb.frame_info[i].phy_address + _veu_srcsetting.height * _veu_srcsetting.width;

	DeviceIoControl(_hddVEU1, IOCTL_VEU_CONVERTIMG, (LPVOID)&_veu_srcsetting, sizeof(_veu_srcsetting), (LPVOID)&_veu_dstsetting, sizeof(_veu_dstsetting), NULL, NULL);
#if VO_RINGBUFFERLOCK_MODE == 0
	ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);	//For better performance
#elif VO_RINGBUFFERLOCK_MODE == 1
	LeaveCriticalSection(&_frame_rb.handle);
#endif
	//SleepTillTick();
	return true;
}


//**************************************
// Function   : VO_getdts
// Description
//	Output the frame to the panel
//**************************************
bool VO_getdts(double *dts)
{
	DWORD rc;
	//static unsigned long last_frame_num;
#if VO_RINGBUFFERLOCK_MODE == 0
	rc = WaitForSingleObject(_frame_rb.handle , 1 );
	if( rc ==WAIT_TIMEOUT)
		return false;
#elif VO_RINGBUFFERLOCK_MODE == 1
	EnterCriticalSection(&_frame_rb.handle);
#endif

	if( VO_sub_rb_isempty() ){
#if VO_RINGBUFFERLOCK_MODE == 0
	ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);
#elif VO_RINGBUFFERLOCK_MODE == 1
	LeaveCriticalSection(&_frame_rb.handle);
#endif
		return false;
	}

	//i = VO_sub_rb_dequeue();
	*dts = _frame_rb.frame_info[ (_frame_rb.frame_rb_front + 1 ) % _frame_rb.frame_rb_num ].dts;
#if VO_RINGBUFFERLOCK_MODE == 0
	ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);
#elif VO_RINGBUFFERLOCK_MODE == 1
	LeaveCriticalSection(&_frame_rb.handle);
#endif
	return true;
}
//**************************************
// Function   :VO_skip
// Description
//	skip one frame
//**************************************
bool VO_skip(void)
{
	unsigned long i;
	DWORD rc;
	//static unsigned long last_frame_num;
#if VO_RINGBUFFERLOCK_MODE == 0
	rc = WaitForSingleObject(_frame_rb.handle , 1 );
	if( rc == WAIT_TIMEOUT)
		return false;
#elif VO_RINGBUFFERLOCK_MODE == 1
	EnterCriticalSection(&_frame_rb.handle);
#endif

#if 1
	if( VO_sub_rb_isempty() ){
#if VO_RINGBUFFERLOCK_MODE == 0
		ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);
#elif VO_RINGBUFFERLOCK_MODE == 1
		LeaveCriticalSection(&_frame_rb.handle);
#endif
		return false;
	}
#else
	while( VO_sub_rb_isempty() );
#endif
	i = VO_sub_rb_dequeue();

	//DeviceIoControl(_hddVEU1, IOCTL_VEU_CONVERTIMG, (LPVOID)&_veu_srcsetting, sizeof(_veu_srcsetting), (LPVOID)&_veu_dstsetting, sizeof(_veu_dstsetting), NULL, NULL);
#if VO_RINGBUFFERLOCK_MODE == 0
	ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);	//For better performance
#elif VO_RINGBUFFERLOCK_MODE == 1
	LeaveCriticalSection(&_frame_rb.handle);
#endif
	return true;
}

//**************************************
// Function   :VO_checkfull
// Description
//	skip one frame
//**************************************
bool VO_checkfull(void)
{
	bool rtn;
	DWORD rc;
#if VO_RINGBUFFERLOCK_MODE == 0
	rc = WaitForSingleObject(_frame_rb.handle , 1 );
#elif VO_RINGBUFFERLOCK_MODE == 1
	EnterCriticalSection(&_frame_rb.handle);
#endif

	rtn = VO_sub_rb_isfull();

#if VO_RINGBUFFERLOCK_MODE == 0
	ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);	//For better performance
#elif VO_RINGBUFFERLOCK_MODE == 1
	LeaveCriticalSection(&_frame_rb.handle);
#endif
	return rtn;
}

//**************************************
// Function   :VO_sub_rb_init
// Description
//	
//**************************************
static void VO_sub_rb_init(void)
{
	memset( &_frame_rb , 0 , sizeof(VO_RB) );
#if VO_RINGBUFFERLOCK_MODE == 0	
	_frame_rb.handle = CreateSemaphore( NULL , 1, 1, NULL);
#elif VO_RINGBUFFERLOCK_MODE == 1
	InitializeCriticalSection(&_frame_rb.handle);
#endif	
}

//**************************************
// Function   :VO_sub_rb_deinit
// Description
//	
//**************************************
static void VO_sub_rb_deinit(void)
{
#if VO_RINGBUFFERLOCK_MODE == 0	
	CloseHandle( _frame_rb.handle );
#elif VO_RINGBUFFERLOCK_MODE == 1
	DeleteCriticalSection(&_frame_rb.handle);
#endif	
}

//**************************************
// Function   :VO_sub_rb_set_buffernum
// Description
//	
//**************************************
static void VO_sub_rb_set_buffernum(unsigned long num)
{
	_frame_rb.frame_rb_num = num;
}

//**************************************
// Function   :VO_sub_rb_enqueue
// Description
//	
//**************************************
inline static unsigned long VO_sub_rb_enqueue(void)
{
	_frame_rb.frame_rb_rear =(_frame_rb.frame_rb_rear + 1) % (_frame_rb.frame_rb_num);
	
	return _frame_rb.frame_rb_rear;
}
//**************************************
// Function   :VO_sub_rb_dequeue
// Description
//	
//**************************************
inline static unsigned long VO_sub_rb_dequeue(void)
{
	_frame_rb.frame_rb_front = (_frame_rb.frame_rb_front + 1 ) % _frame_rb.frame_rb_num;

	return _frame_rb.frame_rb_front;
}
//**************************************
// Function   :VO_sub_rb_isempty
// Description
//	
//**************************************
inline static bool VO_sub_rb_isempty(void)
{
	bool rtn;
	if(_frame_rb.frame_rb_front == _frame_rb.frame_rb_rear)
		rtn = true;
	else
		rtn = false;

	return rtn;

}
//**************************************
// Function   :VO_sub_rb_isfull
// Description
//	
//**************************************
inline static bool VO_sub_rb_isfull(void)
{
	bool rtn;
	if(  (_frame_rb.frame_rb_rear + 1) % (_frame_rb.frame_rb_num) == _frame_rb.frame_rb_front)
		rtn = true;
	else
		rtn = false;

	return rtn;
}
