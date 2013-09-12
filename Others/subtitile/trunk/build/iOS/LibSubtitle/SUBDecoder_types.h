#include <windows.h>
#include "SUBDecoder.h"
#include "SUBSync.h"
#include "dsub_segments.h"

#define HORIZONTAL_PIXEL 720
#define VERTICAL_PIXEL 576

typedef struct _display_element  
{
	LPVOID display_set;
	DWORD pts;
}display_element;

typedef struct _display_node 
{
	display_element dis_element;
	_display_node* pNext;
}display_node;

typedef struct _display_info 
{
	unsigned char* buffer_yuv;

	/* Horizontal region composition position in page 0..719 */
	unsigned int region_horizontal_address;
	/* Vertical region composition position in page 0..575 */
	unsigned int region_vertical_address;
	unsigned int region_width;
	unsigned int region_height;
 
	ePageState page_state;
	unsigned int object_horizontal_address;  //just in use when page_state is PAGE_STATE_NORMAL_CASE
	unsigned int object_vertical_address;	 //just in use when page_state is PAGE_STATE_NORMAL_CASE

	DWORD startshow_time;
	unsigned int duration_time;
}display_info;

//typedef enum _SubDecState
//{
//	SUBDEC_STOPPED = 0,
//	SUBDEC_RUNNING,
//	SUBDEC_PAUSED
//}SubDecState;

typedef struct _DvbSub 
{
	CRITICAL_SECTION csDisplayNode;
	display_node* pHead_display;
	HANDLE hSubDecoder;
	PFN_DisplayDrawCB pRanderPage_CB;
	LPVOID		      pRanderPage_CB_para;
	UINT uColorMap[256];
	CSubSync cSubSync;
	/*walk around code begin*/
	//this member varies when every region of any display_set is rendering.
	display_info sDisplayInfo; 
	/*walk around code end*/
	BOOL bSourcePourIn;

	display_info* pPreDisInfo;    //just care the region part of display_info
	int			  iPreDisInfoNum; //Number of PreDisInfo
}DvbSub;
