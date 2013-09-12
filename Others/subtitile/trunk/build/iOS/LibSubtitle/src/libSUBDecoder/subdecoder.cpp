#include "SUBDecoder_types.h"
#include "SUBDecoder.h"
#include "McCom.h"
#include "ALI_DTV_SUB_typedef.h"
#include "ALI_DTV_SUB_api.h"
#include <assert.h>
#include <stdio.h>
#include "dsub_renderer.h"  //walk round: to get "tDisplaySet" type

#define QUERY_DURATION 3000

class CSUBDecoder : 
	public ISUBDecoder,
	public CMcUnknown
{
public:
	DECLARE_IUNKNOWN
	CSUBDecoder();
	~CSUBDecoder();

	STDMETHODIMP Open(DWORD dwOption);
	STDMETHODIMP Close();
	STDMETHODIMP Reset();
	STDMETHODIMP Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned);
	STDMETHODIMP Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData);

	STDMETHODIMP SendPES(unsigned char *pBuffer, DWORD dwLength);
	STDMETHODIMP SendES(unsigned char *pBuffer, DWORD& dwLength, DWORD PTS);
	STDMETHODIMP SendPCR(UINT64 u64TimeStamp);

    
public:
	void NotifyRenderCB(unsigned long PTS, int Regions, int Number);
	LPVOID GetDvbSub();
	BOOL AddDisplayElement(display_node** ppNode, display_element &element);
	BOOL FetchDisplayElement(display_node** ppNode, display_element &element);
	BOOL IsRanderThreadGoing();
	BOOL IsVanishThreadGoing();
	void ClearPrevRegions();
    HANDLE m_loopevent;

private:
	DvbSub m_dvbSUB;
	HANDLE m_hRanderThread;
	HANDLE m_hVanishThread;
	BOOL m_bRenderThreadGoing;
	BOOL m_bVanishThreadGoing;
    BOOL m_bOpen;
    
};


static void RecvDisplaySet_CB(void *priv, void *display_set,UINT64 pts)
{
	if (priv)
	{
		CSUBDecoder* pThis = (CSUBDecoder*)priv;
		DvbSub* pDvbSUB = (DvbSub*)(pThis->GetDvbSub());
		display_element elem;
		elem.display_set = display_set;
		elem.pts = (DWORD)(pts/90);			//unit: ms
		if(!pThis->AddDisplayElement(&pDvbSUB->pHead_display, elem))
			ALI_DTV_SUB_Display_Set_Destroy(pDvbSUB->hSubDecoder, display_set);
	}
}

static void SubColorSet_CB(void *priv, unsigned char pseudo_color, unsigned char Y, unsigned char Cb, unsigned char Cr, unsigned char T)
{
	if (priv)
	{
		CSUBDecoder* pThis = (CSUBDecoder*)priv;
		DvbSub* pDvbSUB = (DvbSub*)(pThis->GetDvbSub());
		pDvbSUB->uColorMap[pseudo_color] = (T<<24)|(Y<<16)|(Cb<<8)|Cr;
	}
	return;
}

static void SubDrawHLine_CB(void *priv, int x, int y, int length, int thickness, unsigned char pseudo_color)
{
	if (priv)
	{
		unsigned char Y, Cb, Cr;
		unsigned char *Y_frame, *Cb_frame, *Cr_frame;
		CSUBDecoder* pThis = (CSUBDecoder*)priv;
		DvbSub* pDvbSUB = (DvbSub*)(pThis->GetDvbSub());

		if (pDvbSUB->sDisplayInfo.page_state == PAGE_STATE_NORMAL_CASE)
		{
			//because in one region, we don't have to modify "y" value
			x -= pDvbSUB->sDisplayInfo.object_horizontal_address;	//buffer_yuv starts from object_horizontal_address;
		}
		Y = ((pDvbSUB->uColorMap[pseudo_color])&(0xff0000))>>16;
		Cb = ((pDvbSUB->uColorMap[pseudo_color])&(0xff00))>>8;
		Cr = (pDvbSUB->uColorMap[pseudo_color])&(0xff);

		if(Y==0) //full transparency
			return;
		Y_frame = pDvbSUB->sDisplayInfo.buffer_yuv + y*(pDvbSUB->sDisplayInfo.region_width) + x;
		memset(Y_frame, Y, length);
		if (!(y%2))
		{       
			if (x%2)
			{
				x++;
				length--;
			}
			Cb_frame = pDvbSUB->sDisplayInfo.buffer_yuv + (pDvbSUB->sDisplayInfo.region_height)*(pDvbSUB->sDisplayInfo.region_width) + y*(pDvbSUB->sDisplayInfo.region_width)/4 + x/2;	
			for(int i=0;i<(length+1)/2;i++)
			{
				*(Cb_frame+i) = Cb;
			}
		}
		else
		{
			if (x%2)
			{
				x++;
				length--;
			}
			Cr_frame = pDvbSUB->sDisplayInfo.buffer_yuv + (pDvbSUB->sDisplayInfo.region_height)*(pDvbSUB->sDisplayInfo.region_width)*5/4 + (y-1)*(pDvbSUB->sDisplayInfo.region_width)/4 + x/2;		
			for(int i=0;i<(length+1)/2;i++)
			{
				*(Cr_frame+i) = Cr;
			}
		}
	}
	return;
}

static void SubDrawString_CB(void *priv, int x, int y, int width, int height, const unsigned short *str, int length, unsigned char fgcolor, unsigned char bgcolor)
{
#if 0
	// TODO:In this function call, we need use the text string to draw a region
	DvbSub* pDvbSUB = (DvbSub*)priv;
	if (pDvbSUB->pRanderPage_CB)
	{
		CI_SUB_Rect sub_rect;
		sub_rect.lBottom = y + height;
		sub_rect.lLeft = x;
		sub_rect.lRight = x + width;
		sub_rect.lTop = y;
		//(*(pDvbSUB->pRanderPage_CB))((unsigned char*)str, sub_rect, pDvbSUB->pRanderPage_CB_para, 1, -1);
	}
#endif
	return;
}

static DWORD WINAPI SubRanderThread(LPVOID pArg)
{
	CSUBDecoder* pThis = (CSUBDecoder*)pArg;
	DvbSub* pDvbSUB = (DvbSub*)(pThis->GetDvbSub());
	display_element elem;
	while (pThis->IsRanderThreadGoing())
	{	
		//here we may wait for an event 
		if (!pThis->FetchDisplayElement(&pDvbSUB->pHead_display, elem))
		{
			Sleep(200);  //200 is not sure
		}
		else 
		{
			int region_num = ALI_DTV_SUB_Get_Region_Count_Of_DS(pDvbSUB->hSubDecoder, elem.display_set);
			if (region_num)
			{
				//Sync PCR with PTS. 
				if (pDvbSUB->cSubSync.Synchronize(elem.pts, 10000))  //unit: ms
				{
					/*Destroy previous regions.*/
					// "(tDisplaySet*)(elem.display_set)" is a walk round to get "tDisplaySet" pointer.
					if (((tDisplaySet*)(elem.display_set))->page_composition_seg->page_state != PAGE_STATE_NORMAL_CASE)
					{
						pThis->ClearPrevRegions();
					}

					/*Create previous region arrays. The code below cannot implement our intention*/
					/*Special for PAGE_STATE_NORMAL_CASE. In this case, we hope the maximum-region_num array will cover the entire place.
					  But we cannot ensure that through the code below.
					  As for other two case, there is no special meaning.*/
					if (region_num > pDvbSUB->iPreDisInfoNum)  //work around. Just remember the maximum region_num arrays.
					{										   
						if (pDvbSUB->pPreDisInfo)
							delete[] pDvbSUB->pPreDisInfo;
						pDvbSUB->pPreDisInfo = new display_info[region_num];
						pDvbSUB->iPreDisInfoNum = region_num;
					}
					
					for (int i=0;i<region_num;i++)
					{
						region_info rgInfo;
						if(ALI_DTV_SUB_Get_Region_Info(pDvbSUB->hSubDecoder, elem.display_set, &rgInfo, i) == -1)
							continue;

						void* ctx = ALI_DTV_SUB_Region_Render_Context_Create(pDvbSUB->hSubDecoder, elem.display_set,
							rgInfo.region_id, SubDrawHLine_CB, SubDrawString_CB, SubColorSet_CB, (void*)pThis);
						if (!ctx)
							continue;

						//"sDisplayInfo" represents the region we are rendering.
						pDvbSUB->sDisplayInfo.page_state = ((tDisplaySet*)(elem.display_set))->page_composition_seg->page_state;
						pDvbSUB->sDisplayInfo.region_horizontal_address = rgInfo.region_horizontal_address;
						pDvbSUB->sDisplayInfo.region_vertical_address = rgInfo.region_vertical_address;
						if (pDvbSUB->sDisplayInfo.page_state == PAGE_STATE_NORMAL_CASE)
						{
							/*walk round begin*/
							/*the way of getting "object_horizontal_position" is bad, maybe we can use ALI_DTV_SUB_Get_Region_Info instead*/
							pDvbSUB->sDisplayInfo.object_horizontal_address = ((tRenderingContext*)ctx)->region_composition->regions[0].object_horizontal_position;
							pDvbSUB->sDisplayInfo.object_vertical_address = ((tRenderingContext*)ctx)->region_composition->regions[0].object_vertical_position;
							/*walk round end*/

							//Generally the object size differs every time, so realloc it every time.
							if (pDvbSUB->sDisplayInfo.buffer_yuv)
								free(pDvbSUB->sDisplayInfo.buffer_yuv);
							//work around: wid is inaccurate.
							//int wid = HORIZONTAL_PIXEL - pDvbSUB->sDisplayInfo.object_horizontal_address - pDvbSUB->sDisplayInfo.region_horizontal_address;
							int wid = rgInfo.region_width - pDvbSUB->sDisplayInfo.object_horizontal_address;
							pDvbSUB->sDisplayInfo.buffer_yuv = (unsigned char*)malloc((rgInfo.region_height)*wid*3/2);
							memset(pDvbSUB->sDisplayInfo.buffer_yuv, 0x80, (rgInfo.region_height)*wid*3/2);
							pDvbSUB->sDisplayInfo.region_height = rgInfo.region_height;
							pDvbSUB->sDisplayInfo.region_width = wid;
						}
						else
						{
							pDvbSUB->sDisplayInfo.object_horizontal_address = 0;	//means this variable is not in use
							pDvbSUB->sDisplayInfo.object_vertical_address = 0;		//means this variable is not in use
							if (pDvbSUB->sDisplayInfo.region_height != rgInfo.region_height || pDvbSUB->sDisplayInfo.region_width != rgInfo.region_width)
							{
								if (pDvbSUB->sDisplayInfo.buffer_yuv)
									free(pDvbSUB->sDisplayInfo.buffer_yuv);
								pDvbSUB->sDisplayInfo.buffer_yuv = (unsigned char*)malloc((rgInfo.region_height)*(rgInfo.region_width)*3/2);
								memset(pDvbSUB->sDisplayInfo.buffer_yuv, 0x80, (rgInfo.region_height)*(rgInfo.region_width)*3/2);
								pDvbSUB->sDisplayInfo.region_height = rgInfo.region_height;
								pDvbSUB->sDisplayInfo.region_width = rgInfo.region_width;
							}
						}

						/*work around: time_out value does not belong to region, it belongs to the whole display_set.
						Because we cannot hold display_set out of here, so paste time_out value to every regions of the display_set*/
						pDvbSUB->sDisplayInfo.startshow_time = pDvbSUB->cSubSync.GetTimeReference();
						pDvbSUB->sDisplayInfo.duration_time = ALI_DTV_SUB_Get_Time_Out_Of_DS(pDvbSUB->hSubDecoder, elem.display_set) * 1000;


						/*fill previous region arrays*/
						/*[inferior code]the array number in one complete region set may differs, so set the array every time*/
						pDvbSUB->pPreDisInfo[i].buffer_yuv = NULL;
						pDvbSUB->pPreDisInfo[i].region_height = pDvbSUB->sDisplayInfo.region_height;
						pDvbSUB->pPreDisInfo[i].region_width = pDvbSUB->sDisplayInfo.region_width;	//actually this variable is not in use, see Line 165.
						pDvbSUB->pPreDisInfo[i].region_horizontal_address = pDvbSUB->sDisplayInfo.region_horizontal_address;
						pDvbSUB->pPreDisInfo[i].region_vertical_address = pDvbSUB->sDisplayInfo.region_vertical_address;
		
						
						ALI_DTV_SUB_Region_Actual_Rendering(pDvbSUB->hSubDecoder, ctx);
						pThis->NotifyRenderCB(elem.pts, region_num, i+1);
						ALI_DTV_SUB_Region_Render_Context_Destroy(pDvbSUB->hSubDecoder, ctx);
					}
				}
			}
			ALI_DTV_SUB_Display_Set_Destroy(pDvbSUB->hSubDecoder, elem.display_set);
		}
	}

	pThis->ClearPrevRegions();
	return 1;
}

static DWORD WINAPI SubVanishThread(LPVOID pArg)
{
	CSUBDecoder* pThis = (CSUBDecoder*)pArg;
	DvbSub* pDvbSUB = (DvbSub*)(pThis->GetDvbSub());
	while (pThis->IsVanishThreadGoing())
	{

         while (WaitForSingleObject(pThis->m_loopevent, QUERY_DURATION) == WAIT_TIMEOUT)
         {
             DWORD cur_time = pDvbSUB->cSubSync.GetTimeReference();   //unit: ms 
             if (pDvbSUB->iPreDisInfoNum > 0 
                 && cur_time > pDvbSUB->sDisplayInfo.startshow_time 
                 && (DWORD)(cur_time - pDvbSUB->sDisplayInfo.startshow_time) >= pDvbSUB->sDisplayInfo.duration_time)	//If pause, the PCR is not supposed to update.
             {
                 /*need review*/
                 
                 CI_SUB_Rect rect;
                 for (int i=0;i<pDvbSUB->iPreDisInfoNum;i++)
                 {
                     if (pDvbSUB->pPreDisInfo)
                     {
                         CI_SUB_Draw_Info Info;
                         Info.pRegionBuffer = NULL;
                         Info.bShow = 1;
                         Info.PTS = pDvbSUB->sDisplayInfo.startshow_time + pDvbSUB->sDisplayInfo.duration_time;
                         Info.TotalRegions = pDvbSUB->iPreDisInfoNum;
                         Info.RegionIndex = i;
                         Info.DispRect.lTop = pDvbSUB->pPreDisInfo[i].region_vertical_address;
                         Info.DispRect.lBottom = rect.lTop + pDvbSUB->pPreDisInfo[i].region_height;
                         Info.DispRect.lLeft = pDvbSUB->pPreDisInfo[i].region_horizontal_address;
                         Info.DispRect.lRight = rect.lLeft + pDvbSUB->pPreDisInfo[i].region_width;
                         (*(pDvbSUB->pRanderPage_CB))(&Info, pDvbSUB->pRanderPage_CB_para);
                     }
                 }
                 pDvbSUB->sDisplayInfo.startshow_time = 0;
                 pDvbSUB->sDisplayInfo.duration_time = 0;
                 
                 /*need review*/
             }
             //		Sleep(QUERY_DURATION);
         }
        
	}
	return 1;
}

CSUBDecoder::CSUBDecoder():CMcUnknown(0,0)
{
	m_dvbSUB.pHead_display = NULL;
	m_dvbSUB.hSubDecoder = NULL;
	m_dvbSUB.pRanderPage_CB = NULL;
	InitializeCriticalSection(&m_dvbSUB.csDisplayNode);
	memset(m_dvbSUB.uColorMap, 0, 256*sizeof(UINT));
	memset(&m_dvbSUB.sDisplayInfo, 0, sizeof(display_info));
	m_dvbSUB.bSourcePourIn = FALSE;
	m_dvbSUB.iPreDisInfoNum = 0;
	m_dvbSUB.pPreDisInfo = NULL;
	m_bRenderThreadGoing = FALSE;
	m_bVanishThreadGoing = FALSE;
    m_loopevent = CreateEvent(NULL, FALSE, FALSE, NULL);
    m_bOpen = false;
}

CSUBDecoder::~CSUBDecoder()
{
//	Close();
    CloseHandle(m_loopevent);
	DeleteCriticalSection(&m_dvbSUB.csDisplayNode);
}

STDMETHODIMP CSUBDecoder::Open(DWORD dwOption) 
{
    if (m_bOpen == true) {
        return  E_FAIL;
    }
	CI_SUB_Open_Option* open_option = (CI_SUB_Open_Option*)dwOption;
	/*TODO: add some protection maybe*/
	
	m_dvbSUB.hSubDecoder = ALI_DTV_SUB_Create(open_option->usCompositionPageID, open_option->usAncillaryPageID,
											  RecvDisplaySet_CB, (void*)this);
	if (!m_dvbSUB.hSubDecoder)
	{
		printf("ALI_DTV_SUB_Create call fail!\n");
		return E_FAIL;
	}

	m_bRenderThreadGoing = TRUE;
	m_hRanderThread = CreateThread(NULL, 0, SubRanderThread, (void*)this, 0, NULL);
	m_bVanishThreadGoing = TRUE;
	m_hVanishThread = CreateThread(NULL, 0, SubVanishThread, (void*)this, 0, NULL);

    m_bOpen = true;
	return S_OK;
}

DWORD    timeGetTime(void) {
    struct timeval my_timeval;
    if (gettimeofday(&my_timeval, NULL))
        return 0;
    return (DWORD)(my_timeval.tv_sec * 1000) + (my_timeval.tv_usec / 1000);
}


STDMETHODIMP CSUBDecoder::Close()
{
    if (m_bOpen == false) {
        return E_FAIL;
    }
	m_dvbSUB.cSubSync.BreakSync(); //In case of pause state.
	//First close thread, then release hSubDecoder.
	m_bRenderThreadGoing = FALSE;
	if (WaitForSingleObject(m_hRanderThread, INFINITE) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hRanderThread, 0);
	}
    CloseHandle(m_hRanderThread);

	
    display_element elem;
	while (FetchDisplayElement(&m_dvbSUB.pHead_display, elem))	//Clear the rest display_set.
		ALI_DTV_SUB_Display_Set_Destroy(m_dvbSUB.hSubDecoder, elem.display_set);


	m_bVanishThreadGoing = FALSE;
    SetEvent(m_loopevent);
    if (WaitForSingleObject(m_hVanishThread, INFINITE) == WAIT_TIMEOUT)
        TerminateThread(m_hVanishThread, (DWORD)-1);
    
    CloseHandle(m_hVanishThread);
    
    //	if (WaitForSingleObject(m_hVanishThread, INFINITE) == WAIT_TIMEOUT)
//	{
//		TerminateThread(m_hVanishThread, 0);
//		CloseHandle(m_hVanishThread);
//	}
	
	if (m_dvbSUB.hSubDecoder)
	{
		ALI_DTV_SUB_Release(m_dvbSUB.hSubDecoder);
		m_dvbSUB.hSubDecoder = NULL;
	}

	if (m_dvbSUB.sDisplayInfo.buffer_yuv)
	{
		free(m_dvbSUB.sDisplayInfo.buffer_yuv);
		m_dvbSUB.sDisplayInfo.buffer_yuv = NULL;
	}
	memset(&m_dvbSUB.sDisplayInfo, 0, sizeof(display_info));
	if (m_dvbSUB.pPreDisInfo)
	{
		delete[] m_dvbSUB.pPreDisInfo;
		m_dvbSUB.pPreDisInfo = NULL;
	}
	m_dvbSUB.iPreDisInfoNum = 0;

    m_bOpen = false;
	return S_OK;
}

STDMETHODIMP CSUBDecoder::Reset()
{
	return S_OK;
}

STDMETHODIMP CSUBDecoder::Get(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData, DWORD *pcbReturned)
{
	return S_OK;
}

STDMETHODIMP CSUBDecoder::Set(REFGUID guidPropSet, DWORD dwPropID, LPVOID pInstanceData, DWORD cbInstanceData, LPVOID pPropData, DWORD cbPropData)
{
	switch(dwPropID)
	{
	case EMC_PROPID_SUBTITLE_SETDRAWCB:
		m_dvbSUB.pRanderPage_CB = (PFN_DisplayDrawCB)pPropData;
		m_dvbSUB.pRanderPage_CB_para = pInstanceData;
		break;
	case EMC_PROPID_SUBTITLE_DISP_ON:
		m_dvbSUB.bSourcePourIn = TRUE;
		break;
	case EMC_PROPID_SUBTITLE_DISP_OFF:
		m_dvbSUB.bSourcePourIn = FALSE;
		break;
	case EMC_PROPID_SUBTITLE_NOSYNC:
		m_dvbSUB.cSubSync.DisableSync((int)cbInstanceData);	
		break;
	case EMC_PROPID_SUBTITLE_SWITCH_LANGUAGE:
		if(S_OK != Close())
			return E_FAIL;
		//Assume pPropData is pointer to CI_SUB_Open_Option
		if(S_OK != Open((DWORD)pPropData))
			return E_FAIL;
		break;
	}
	return S_OK;
}

STDMETHODIMP CSUBDecoder::SendPES(unsigned char *pBuffer, DWORD dwLength)
{
	if (!dwLength || !pBuffer || !m_dvbSUB.hSubDecoder)
		return E_FAIL;
	if(m_dvbSUB.bSourcePourIn)
	{
		if (ALI_DTV_SUB_Push_PES_Pkt(m_dvbSUB.hSubDecoder, (char*)pBuffer, dwLength) == -1)
			return E_FAIL;
	}
	return S_OK;
}


STDMETHODIMP CSUBDecoder::SendES(unsigned char *pBuffer, DWORD& dwLength, DWORD PTS)
{
	if (!dwLength || !pBuffer || !m_dvbSUB.hSubDecoder)
		return E_FAIL;
	if(m_dvbSUB.bSourcePourIn)
	{
		int UsedLength = ALI_DTV_SUB_Push_ES_Pkt(m_dvbSUB.hSubDecoder, (char*)pBuffer, dwLength, PTS);
		if (UsedLength == -1)
			return E_FAIL;
		dwLength = UsedLength;
	}
	return S_OK;
}

STDMETHODIMP CSUBDecoder::SendPCR(UINT64 u64TimeStamp)
{
	m_dvbSUB.cSubSync.SetTimeReference((DWORD)u64TimeStamp);
	return S_OK;
}

ISUBDecoder* CreateSUBDecoder()
{
	return new CSUBDecoder();
}

void CSUBDecoder::NotifyRenderCB(unsigned long PTS, int Regions, int Number)
{
	CI_SUB_Draw_Info Info;
	
	Info.DispRect.lTop = m_dvbSUB.sDisplayInfo.region_vertical_address;
	Info.DispRect.lBottom = m_dvbSUB.sDisplayInfo.region_height + Info.DispRect.lTop;
	Info.DispRect.lLeft = m_dvbSUB.sDisplayInfo.region_horizontal_address + m_dvbSUB.sDisplayInfo.object_horizontal_address;
	Info.DispRect.lRight = m_dvbSUB.sDisplayInfo.region_width + Info.DispRect.lLeft;

	Info.bShow = 1;
	Info.RegionIndex = Number;
	Info.TotalRegions = Regions;
	Info.PTS = PTS;
	Info.pRegionBuffer = m_dvbSUB.sDisplayInfo.buffer_yuv;

	(*(m_dvbSUB.pRanderPage_CB))(&Info, m_dvbSUB.pRanderPage_CB_para);
	memset(m_dvbSUB.sDisplayInfo.buffer_yuv, 0x80, (Info.DispRect.lBottom - Info.DispRect.lTop)*(Info.DispRect.lRight - Info.DispRect.lLeft)*3/2);
	printf("Render region: Top = %ld, Left = %ld, Bottom = %ld, Right = %ld\n", Info.DispRect.lTop, Info.DispRect.lLeft, Info.DispRect.lBottom, Info.DispRect.lRight);
}

LPVOID CSUBDecoder::GetDvbSub()
{
	return &m_dvbSUB;
}

BOOL CSUBDecoder::AddDisplayElement(display_node** ppNode, display_element &element)
{
	EnterCriticalSection(&m_dvbSUB.csDisplayNode);
	display_node* node = NULL;
	node = new display_node;
	if (!node)
	{
		LeaveCriticalSection(&m_dvbSUB.csDisplayNode);
		return FALSE;
	}
	node->dis_element= element;
	node->pNext = NULL;

	if (*ppNode)
	{
		display_node* pNode = *ppNode;
		while (pNode->pNext)
			pNode = pNode->pNext;
		pNode->pNext = node;
	}
	else
		*ppNode = node;

	LeaveCriticalSection(&m_dvbSUB.csDisplayNode);
	return TRUE;
}


BOOL CSUBDecoder::FetchDisplayElement(display_node** ppNode, display_element &element)
{
	EnterCriticalSection(&m_dvbSUB.csDisplayNode);
	if (*ppNode)
	{
		display_node* pTemp;
		element = (*ppNode)->dis_element;
		pTemp = *ppNode;
		*ppNode = pTemp->pNext;
		delete pTemp;
		LeaveCriticalSection(&m_dvbSUB.csDisplayNode);
		return TRUE;
	}
	LeaveCriticalSection(&m_dvbSUB.csDisplayNode);
	return FALSE;
}

BOOL CSUBDecoder::IsRanderThreadGoing()
{
	return m_bRenderThreadGoing;
}

BOOL CSUBDecoder::IsVanishThreadGoing()
{
	return m_bVanishThreadGoing;
}

void CSUBDecoder::ClearPrevRegions()
{
	if (m_dvbSUB.pPreDisInfo)
	{
		for (int i=0;i<m_dvbSUB.iPreDisInfoNum;i++)
		{
			CI_SUB_Draw_Info Info;
			Info.pRegionBuffer = NULL;
			Info.bShow = 0;
			Info.PTS = -1;
			Info.RegionIndex = i+1;
			Info.TotalRegions = m_dvbSUB.iPreDisInfoNum;
			Info.DispRect.lTop = m_dvbSUB.pPreDisInfo[i].region_vertical_address;
			Info.DispRect.lBottom = m_dvbSUB.pPreDisInfo[i].region_height + Info.DispRect.lTop;
			Info.DispRect.lLeft = m_dvbSUB.pPreDisInfo[i].region_horizontal_address;
			Info.DispRect.lRight = HORIZONTAL_PIXEL; //do not know the maximum value of right coordinates, so make it 720.
			(*(m_dvbSUB.pRanderPage_CB))(&Info, m_dvbSUB.pRanderPage_CB_para);
		}
		delete[] m_dvbSUB.pPreDisInfo;
		m_dvbSUB.pPreDisInfo = NULL;
		m_dvbSUB.iPreDisInfoNum = 0;
	}
}