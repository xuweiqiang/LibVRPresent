//********************************************************************
//Renesas Video Output
//		by Benson Chuang
//			benson.chuang.tw@gmail.com  benson.chuang@renesas.com
//********************************************************************
//Decoder : libvo
//						Renesas Technology Taiwan/Engineerning Center
//********************************************************************
//I didn't implement error returning mechanism, please carefully use this library.

#ifdef RENESAS

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

#define DIRECT_SHOW

//**************************************
// Global variables
//**************************************
static HANDLE	_hddVEU1=NULL;
static RB		_video_rb;
#ifndef DIRECT_SHOW
static VO_RB	_frame_rb;
#endif
static S_UGE_DRAW_EX	_veu_srcsetting;
static S_UGE_DRAW_EX	_veu_dstsetting;

//**************************************
// Function   : VO_init_hw
// Description
//	The entry point of video outout unit
//**************************************
void VO_init_hw(VIDEO_CONTEXT *video_info)
{
	//Init VEU
	_hddVEU1=CreateFile(TEXT("VEU1:"), GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);
#ifndef DIRECT_SHOW	
	memset( &_frame_rb.frame_info , 0 , sizeof(VO_FRAMEINFO) * VO_BUFFER_FRAME );

RETAILMSG(0, (L"VO_read _hddVEU1=0x%x\r\n",_hddVEU1));	
	VO_sub_rb_init_hw();
#endif	
	return;
}   

//**************************************
// Function   :VO_deinit
// Description
//	
//**************************************
void VO_deinit_hw(void)
{
#ifndef DIRECT_SHOW
	VO_sub_rb_deinit_hw();
#endif
	if(_hddVEU1)
	{
		DeviceIoControl(_hddVEU1,IOCTL_VEU_FREE_ADDRESS,NULL,0,NULL,0,NULL,NULL);
		CloseHandle(_hddVEU1);
		_hddVEU1=NULL;
	}
////	for( i = 0 ; i < _frame_rb.frame_rb_num ; i ++)
////		FreePhysMem(_frame_rb.frame_info[i].address);
	return;
}
//**************************************
// Function   :VO_set_video_startpos
// Description
//	
//**************************************
void VO_set_video_startpos_hw(unsigned long x , unsigned long y)
{
	_veu_dstsetting.xoffset = x;//VO_NORMAL_POS_X;	
	_veu_dstsetting.yoffset = y;//VO_NORMAL_POS_Y;	
}

//**************************************
// Function   :VO_set_video_mode_change
// Description
//	
//**************************************
void VO_set_video_mode_change_hw(unsigned long in_w, unsigned long in_h , unsigned long in_mem_w , unsigned long out_w, unsigned long out_h , unsigned long out_mem_w , unsigned long buff_num)
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
	_veu_dstsetting.format = VIO_RGB_06_OUT;//VIO_RGB_06_OUT;
	
#if (WIN32_WCE == 0x500)
	_veu_dstsetting.buff1 = (unsigned char*)TO_CACHE(0xAF000000);
#else
    _veu_dstsetting.buff1 = (unsigned char*)TO_CACHE(0xB6000000);

#endif
	DeviceIoControl(_hddVEU1,IOCTL_VEU_SWAPVALUE, (LPVOID)&nswap, sizeof(nswap), NULL, 0, NULL, NULL);
	RETAILMSG(0, (L"VO_set_video_mode_change\r\n"));
}

//**************************************
// Function   :VO_set_video_parameter
// Description
//	
//**************************************
void VO_set_video_parameter_hw(unsigned long in_w, unsigned long in_h , 
  unsigned long in_mem_w , unsigned long out_w, unsigned long out_h , 
  unsigned long out_mem_w , unsigned long buff_num)
{
	DWORD	nswap = 0x67;
	unsigned char * Address=NULL;
	unsigned char * PAddress=NULL;

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
#if (_WIN32_WCE == 0x500)
  _veu_dstsetting.buff1 = (unsigned char*)TO_CACHE(0xAF000000);
#else
   _veu_dstsetting.buff1 = (unsigned char*)TO_CACHE(0xB6000000);
#endif

	DeviceIoControl(_hddVEU1,IOCTL_VEU_SWAPVALUE, (LPVOID)&nswap, sizeof(nswap), NULL, 0, NULL, NULL);
#ifndef DIRECT_SHOW
	DeviceIoControl(_hddVEU1,IOCTL_VEU_ALLOC_ADDRESS,&PAddress,sizeof(DWORD),&Address,sizeof(DWORD),&BytesWritten,NULL);
	RETAILMSG(0, (L"Address:0x%x, PAddress=0x%x src:width_mem(0x%x)(%dx%d) dst:width_mem(0x%x)(%dx%d) \r\n",
		Address,PAddress, _veu_srcsetting.width_mem, _veu_srcsetting.width, _veu_srcsetting.height, \
		_veu_dstsetting.width_mem, _veu_dstsetting.width, _veu_dstsetting.height));
	//attach the frame buffer

	for( i = 0 ; i < VO_BUFFER_FRAME ; i ++)
	{
//		_frame_rb.frame_info[i].address = (unsigned char*)AllocPhysMem( in_w * in_h * VO_FRMAE_SIZE_PIXEL , PAGE_EXECUTE_READWRITE , 0 , 0 , (unsigned long*)&_frame_rb.frame_info[i].phy_address);

		_frame_rb.frame_info[i].address = (unsigned char*)(Address+ i*(in_w * in_h *VO_FRMAE_SIZE_PIXEL));
		_frame_rb.frame_info[i].phy_address = (unsigned char*)(PAddress+ i*(in_w * in_h *VO_FRMAE_SIZE_PIXEL));   
		RETAILMSG(0, (L"frame %d,_frame_rb.frame_info[i].address1=0x%x, frame=0x%x \r\n",i,_frame_rb.frame_info[i].address,_frame_rb.frame_info[i].phy_address));

		if( _frame_rb.frame_info[i].address == NULL ){
			//init the ring buffer
			VO_sub_rb_set_buffernum_hw(i-1);
			break;
		}
		VO_sub_rb_set_buffernum_hw( VO_BUFFER_FRAME );
	}
#endif
	return;

}

//**************************************
// Function   :VO_write
// Description
//	Store the image in ring buffer
//**************************************
bool VO_write_hw(unsigned char *y , unsigned char *c ,double dts)
{
	int i;
       DWORD	StartTickTime=0,NowTime=0;
#ifndef DIRECT_SHOW
#if VO_RINGBUFFERLOCK_MODE == 0

	rc = WaitForSingleObject(_frame_rb.handle , INFINITE );
	if( rc == WAIT_TIMEOUT)
		return false;
#elif VO_RINGBUFFERLOCK_MODE == 1
	EnterCriticalSection(&_frame_rb.handle);
#endif

    RETAILMSG(0, (L"VO_write\r\n"));
	while( VO_sub_rb_isfull_hw() )
	{
		Sleep(0);
	}
		    
	i = VO_sub_rb_enqueue_hw();
       //StartTickTime=GetTickCount();
#endif
#ifdef DIRECT_SHOW
	
	_veu_srcsetting.buff1=(unsigned char *)TO_CACHE(y);
	_veu_srcsetting.buff2=(unsigned char *)TO_CACHE(c);
 //	RETAILMSG(0, (L"VO_write DIRECT_SHOW: buf1:0x%x(0x%x) buf2: 0x%x(0x%x)\r\n", _veu_srcsetting.buff1,OALVAtoPA(y), _veu_srcsetting.buff2, OALVAtoPA(c)));
#else
	memcpy(_frame_rb.frame_info[i].address , y , _veu_srcsetting.height * _veu_srcsetting.width);
	memcpy(_frame_rb.frame_info[i].address + _veu_srcsetting.height * _veu_srcsetting.width , c , _veu_srcsetting.height * _veu_srcsetting.width /2);
#endif	
       //NowTime=GetTickCount();
       RETAILMSG(0, (L"VO_write address(y:0x%x c:0x%x)%dx%d:(%d)frame %d ms\r\n", y, c, _veu_srcsetting.height,  _veu_srcsetting.width,i, NowTime-StartTickTime));
	//Save dts
#ifndef DIRECT_SHOW
	_frame_rb.frame_info[i].dts = dts;
#if VO_RINGBUFFERLOCK_MODE == 0
	ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);
#elif VO_RINGBUFFERLOCK_MODE == 1
	LeaveCriticalSection(&_frame_rb.handle);
#endif
#endif
	return true;
}

//**************************************
// Function   :VO_read
// Description
//	Output the frame to the panel
//**************************************
bool VO_read_hw(void)
{
	//static unsigned long last_frame_num;
#ifdef DIRECT_SHOW
	DeviceIoControl(_hddVEU1, IOCTL_VEU_CONVERTIMG, (LPVOID)&_veu_srcsetting, sizeof(_veu_srcsetting), (LPVOID)&_veu_dstsetting, sizeof(_veu_dstsetting), NULL, NULL);
#else
#if VO_RINGBUFFERLOCK_MODE == 0

	rc = WaitForSingleObject(_frame_rb.handle , 1 );
	if( rc == WAIT_TIMEOUT)
		return false;
#elif VO_RINGBUFFERLOCK_MODE == 1
	EnterCriticalSection(&_frame_rb.handle);
#endif

RETAILMSG(0, (L"VO_read\r\n"));

	if( VO_sub_rb_isempty_hw() ){
#if VO_RINGBUFFERLOCK_MODE == 0
		ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);
#elif VO_RINGBUFFERLOCK_MODE == 1
		LeaveCriticalSection(&_frame_rb.handle);
#endif
		RETAILMSG(0, (L"VO_read is empty\r\n"));
		return false;
	}

	while( VO_sub_rb_isempty_hw() );

	i = VO_sub_rb_dequeue_hw();

	_veu_srcsetting.buff1=_frame_rb.frame_info[i].phy_address;
	_veu_srcsetting.buff2=_frame_rb.frame_info[i].phy_address + _veu_srcsetting.height * _veu_srcsetting.width;

RETAILMSG(0, (L"VO_read _hddVEU1=0x%x _veu_srcsetting=0x%x _veu_dstsetting=0x%x\r\n",_hddVEU1,&_veu_srcsetting,&_veu_dstsetting));
	DeviceIoControl(_hddVEU1, IOCTL_VEU_CONVERTIMG, (LPVOID)&_veu_srcsetting, sizeof(_veu_srcsetting), (LPVOID)&_veu_dstsetting, sizeof(_veu_dstsetting), NULL, NULL);

#if VO_RINGBUFFERLOCK_MODE == 0
	ReleaseSemaphore( _frame_rb.handle, 1 ,NULL);	//For better performance
#elif VO_RINGBUFFERLOCK_MODE == 1
	LeaveCriticalSection(&_frame_rb.handle);
#endif
#endif
	//SleepTillTick();
	return true;
}

#ifndef DIRECT_SHOW
//**************************************
// Function   : VO_getdts
// Description
//	Output the frame to the panel
//**************************************
bool VO_getdts_hw(double *dts)
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

	if( VO_sub_rb_isempty_hw() ){
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
bool VO_skip_hw(void)
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
	if( VO_sub_rb_isempty_hw() ){
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
	i = VO_sub_rb_dequeue_hw();

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
bool VO_checkfull_hw(void)
{
	bool rtn;
	DWORD rc;
#if VO_RINGBUFFERLOCK_MODE == 0
	rc = WaitForSingleObject(_frame_rb.handle , 1 );
#elif VO_RINGBUFFERLOCK_MODE == 1
	EnterCriticalSection(&_frame_rb.handle);
#endif

	rtn = VO_sub_rb_isfull_hw();

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
static void VO_sub_rb_init_hw(void)
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
static void VO_sub_rb_deinit_hw(void)
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
static void VO_sub_rb_set_buffernum_hw(unsigned long num)
{
	_frame_rb.frame_rb_num = num;
}

//**************************************
// Function   :VO_sub_rb_enqueue
// Description
//	
//**************************************
inline static unsigned long VO_sub_rb_enqueue_hw(void)
{
	_frame_rb.frame_rb_rear =(_frame_rb.frame_rb_rear + 1) % (_frame_rb.frame_rb_num);
	
	return _frame_rb.frame_rb_rear;
}
//**************************************
// Function   :VO_sub_rb_dequeue
// Description
//	
//**************************************
inline static unsigned long VO_sub_rb_dequeue_hw(void)
{
	_frame_rb.frame_rb_front = (_frame_rb.frame_rb_front + 1 ) % _frame_rb.frame_rb_num;

	return _frame_rb.frame_rb_front;
}
//**************************************
// Function   :VO_sub_rb_isempty
// Description
//	
//**************************************
inline static bool VO_sub_rb_isempty_hw(void)
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
inline static bool VO_sub_rb_isfull_hw(void)
{
	bool rtn;
	if(  (_frame_rb.frame_rb_rear + 1) % (_frame_rb.frame_rb_num) == _frame_rb.frame_rb_front)
		rtn = true;
	else
		rtn = false;

	return rtn;
}

#else

bool VO_skip_hw(void)
{
  return true;
}

bool VO_getdts_hw(double *dts)
{
  return true;
}

bool VO_checkfull_hw(void)
{
  return true;
}

#endif
#endif
