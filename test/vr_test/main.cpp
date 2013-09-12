#include <stdio.h>
#include <dlfcn.h>
#include "windows.h"
#include "../../inc/GfxPresent.h"
#include <time.h>

#define SRC_WIDTH 320
#define SRC_HEIGHT 240

#define DST_WIDTH 320
#define DST_HEIGHT 240

#define LOOP_CNT	20
typedef IMcVideoPresenter* (*FP_VPCREATOR)();


GUID inputGuid;

int initVR(IMcVideoPresenter* pPresenter)
{
	HRESULT hr = S_OK; 
	if(pPresenter == NULL)
		return -1;
	
	VP_OpenOptions sInit;
	memset(&sInit,0, sizeof(sInit));
	sInit.dwWidth = SRC_WIDTH ;
	sInit.dwHeight= SRC_HEIGHT;
	sInit.dwFourCC = MAKE_FOURCC('Y','V','1','2');

	hr = pPresenter->Open(&sInit,sizeof(sInit));
	if(FAILED(hr))
	{
		printf("Open failed \n");
		return -1;
	}

	RECT Dstrect;
	memset(&Dstrect ,0 ,sizeof(Dstrect));
	Dstrect.left = Dstrect.top =0;
	Dstrect.right = DST_WIDTH;
	Dstrect.bottom = DST_HEIGHT;
	hr = pPresenter->Set(inputGuid,MC_PROPID_VP_RECTDST,0,0,&Dstrect,0);
	if(FAILED(hr))
	{
		printf("Set Dst failed \n");
		return -1;
	}


	LONG m_lRotationMode = VP_ROTATION_0;
	hr = pPresenter->Set(inputGuid, MC_PROPID_VP_ROTATION,0,0,&m_lRotationMode,0);
	if(FAILED(hr))
	{
		printf("Set rotation failed \n");
		return -1;
	}

	return 0;
}

int DestroyVR(IMcVideoPresenter* pPresenter)
{
	if(pPresenter == NULL)
		return -1;
	
	pPresenter->Close();
	pPresenter->Release();

	return 0;
}

int main() 
{
	HINSTANCE m_hVRPresentDLL;
	IMcVideoPresenter* m_pPresent=NULL;
	

	#define VPLIBNAME "./LibVRPresent.dll"


	char *err;
	void *libHandle = NULL;
	libHandle = dlopen(VPLIBNAME, RTLD_LAZY);
	if(!libHandle)
	{
		err = dlerror();
		printf("dlopen error :%s\n", err);
		return -1;
	}
	IMcVideoPresenter* (*createvp)();

	createvp = (IMcVideoPresenter* (*)())dlsym(libHandle, "CreateVideoPresenter");
	if(createvp)
		m_pPresent = (IMcVideoPresenter*)createvp();

	if (m_pPresent == NULL)
	{
		printf("[PlaybackEngine][%4d] Create video presenter FAILED!!\n", __LINE__);
		return -1;
	}

	if(initVR(m_pPresent)!=0)
		return -1;
	
	
	FILE* m_file = NULL;
	m_file  = fopen(("./1.yuv"),("rb"));
	if(m_file == NULL)
	{
		printf("file open failed \n");
		return -1;
	}

	fseek(m_file,0,SEEK_END);
	LONG m_FileLength = ftell(m_file);
	fseek(m_file, 0 , SEEK_SET);

	LPBYTE m_pYbuf = NULL;
	LPBYTE m_pUbuf = NULL;
	LPBYTE m_pVbuf = NULL;
	
	m_pYbuf = (LPBYTE)malloc(SRC_WIDTH*SRC_HEIGHT);
	m_pUbuf = (LPBYTE)malloc(SRC_WIDTH*SRC_HEIGHT>>2);
	m_pVbuf = (LPBYTE)malloc(SRC_WIDTH*SRC_HEIGHT>>2);

	if((m_pYbuf == NULL) || (m_pUbuf  == NULL) || (m_pVbuf  == NULL))
	{
		if(m_pYbuf)
			free(m_pYbuf );
		if(m_pUbuf)
			free(m_pUbuf );
		if(m_pVbuf)
			free(m_pVbuf );
	
		return -1;
	}

	DWORD y_stride = SRC_WIDTH;
	DWORD u_stride = SRC_WIDTH>>1;
	DWORD v_stride = SRC_WIDTH>>1;;

	LPBYTE curr[3];
	curr[0] = &m_pYbuf [0];
	curr[1] = &m_pUbuf[0];
	curr[2] = &m_pVbuf[0];


	DWORD m_dwStrides[3];
	m_dwStrides[0] = SRC_WIDTH;
	m_dwStrides[1] = m_dwStrides[2] = SRC_WIDTH>>1;

	DWORD FrameSize = (SRC_WIDTH*SRC_HEIGHT*3)>>1;

	time_t timep;
	time(&timep);	
	printf("Start Time: %s \n",ctime(&timep));
	int dwFmcnt = 0;

	int iLoopCnt = 0;
	again:
	while(((m_FileLength - ftell(m_file)) >= FrameSize)&&(!feof(m_file)))
	{
		fread(m_pYbuf ,sizeof(BYTE),y_stride*SRC_HEIGHT,m_file);
		fread(m_pUbuf,sizeof(BYTE),u_stride*SRC_HEIGHT>>1,m_file);
		fread(m_pVbuf,sizeof(BYTE),v_stride*SRC_HEIGHT>>1,m_file);

		m_pPresent->Present(curr, m_dwStrides, 0);
		dwFmcnt++;
	}

	iLoopCnt++;
	if(iLoopCnt <LOOP_CNT)
	{
		fseek(m_file, 0 , SEEK_SET);
		goto again;
	}
	
	time_t time_end;
	time (&time_end);	
	printf("End Time: %s FrameCount:%d \n",ctime(&time_end), dwFmcnt);

	if(time_end - timep)
		printf("fps:%f \n",(float)dwFmcnt/(float)((time_end - timep) ) );
	else
		printf("Please loop for more times");
	DestroyVR(m_pPresent);

	if(m_pYbuf)
		free(m_pYbuf );
	if(m_pUbuf)
		free(m_pUbuf );
	if(m_pVbuf)
		free(m_pVbuf );

	return 0;
}
