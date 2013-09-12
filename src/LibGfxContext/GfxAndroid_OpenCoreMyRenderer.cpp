
#ifdef GFX_ANDROID_OPENCORE

#include "GfxAndroid_OpenCoreMyRenderer.h"
#include <pvmi_kvp.h>
#include <windows.h>
#include "../../inc/dp.h"


#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "My Renderer Class"

static CRITICAL_SECTION g_csDebugOutput;

void CMyPVPlayer::sendEvent(int msg, int ext1, int ext2)
{
	LOGI("CMyPVPlayer::sendEvent - msg 0x%x", msg);
	MediaPlayerBase::sendEvent(msg, ext1, ext2);
}

CGfxAndroid_MyRenderer::CGfxAndroid_MyRenderer(void) : m_pSurfaceOutput(NULL)
{
}

CGfxAndroid_MyRenderer::~CGfxAndroid_MyRenderer(void)
{
	EnterCriticalSection(&g_csDebugOutput);
	if (m_pSurfaceOutput)
	{
		LOGI("CGfxAndroid_MyRenderer::closeFrameBuf");
		m_pSurfaceOutput->closeFrameBuf();
		LOGI("CGfxAndroid_MyRenderer::delete m_pSurfaceOutput");
		delete m_pSurfaceOutput;
		m_pSurfaceOutput = NULL;
		LOGI("CGfxAndroid_Myrender:: delete m_pSurfaceOutput Quit");
	}
	LeaveCriticalSection(&g_csDebugOutput);

	DeleteCriticalSection (&g_csDebugOutput);
}

PVMFStatus CGfxAndroid_MyRenderer::SetMIOParameterInt32(PvmiKeyType aKey, int32 aValue)
//to set parameters to the MIO component through its config interface.
{
    OsclMemAllocator alloc;
    PvmiKvp kvp;
    PvmiKvp* retKvp = NULL; // for return value

    kvp.key = NULL;
    kvp.length = oscl_strlen(aKey) + 1; // +1 for \0
    kvp.capacity = kvp.length;

    kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
    if (!kvp.key)
        return PVMFErrNoMemory;

    oscl_strncpy(kvp.key, aKey, kvp.length);
    kvp.value.int32_value = aValue;

    int32 err;
    OSCL_TRY(err, m_pSurfaceOutput->setParametersSync(0, &kvp, 1, retKvp););
    alloc.deallocate(kvp.key);

    if (err != OsclErrNone || retKvp)
        return PVMFFailure;

    return PVMFSuccess;
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus CGfxAndroid_MyRenderer::SetMIOParameterUint32(PvmiKeyType aKey, uint32 aValue)
//to set parameters to the MIO component through its config interface.
{
    OsclMemAllocator alloc;
    PvmiKvp kvp;
    PvmiKvp* retKvp = NULL; // for return value

    kvp.key = NULL;
    kvp.length = oscl_strlen(aKey) + 1; // +1 for \0
    kvp.capacity = kvp.length;

    kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
    if (!kvp.key)
        return PVMFErrNoMemory;

    oscl_strncpy(kvp.key, aKey, kvp.length);
    kvp.value.uint32_value = aValue;

    int32 err;
    OSCL_TRY(err, m_pSurfaceOutput->setParametersSync(0, &kvp, 1, retKvp););
    alloc.deallocate(kvp.key);

    if (err != OsclErrNone || retKvp)
        return PVMFFailure;

    return PVMFSuccess;
}


////////////////////////////////////////////////////////////////////////////
PVMFStatus CGfxAndroid_MyRenderer::SetMIOParameterPchar(PvmiKeyType aKey, char* aValue)
//to set parameters to the MIO component through its config interface.
{
    OsclMemAllocator alloc;
    PvmiKvp kvp;
    PvmiKvp* retKvp = NULL; // for return value

    kvp.key = NULL;
    kvp.length = oscl_strlen(aKey) + 1; // +1 for \0
    kvp.capacity = kvp.length;

    kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
    if (!kvp.key)
        return PVMFErrNoMemory;

    oscl_strncpy(kvp.key, aKey, kvp.length);
    kvp.value.pChar_value = aValue;

    int32 err;
    OSCL_TRY(err, m_pSurfaceOutput->setParametersSync(0, &kvp, 1, retKvp););
    alloc.deallocate(kvp.key);

    if (err != OsclErrNone || retKvp)
        return PVMFFailure;

    return PVMFSuccess;
}

////////////////////////////////////////////////////////////////////////////
#if (ANDROID_VER != 15)
PVMFStatus CGfxAndroid_MyRenderer::SetMIOParameterFormat(PvmiKeyType aKey, PVMFFormatType aFormatType)
//to set parameters to the MIO component through its config interface.
{
    OsclMemAllocator alloc;
    PvmiKvp kvp;
    PvmiKvp* retKvp = NULL; // for return value

    kvp.key = NULL;
    kvp.length = oscl_strlen(aKey) + 1; // +1 for \0
    kvp.capacity = kvp.length;

    kvp.key = (PvmiKeyType)alloc.ALLOCATE(kvp.length);
    if (!kvp.key)
        return PVMFErrNoMemory;

    oscl_strncpy(kvp.key, aKey, kvp.length);
    kvp.value.pChar_value = (char*)aFormatType.getMIMEStrPtr();

    int32 err;
    OSCL_TRY(err, m_pSurfaceOutput->setParametersSync(0, &kvp, 1, retKvp););
    alloc.deallocate(kvp.key);

    if (err != OsclErrNone || retKvp)
        return PVMFFailure;

    return PVMFSuccess;
}
#endif

void CGfxAndroid_MyRenderer::Init(const sp<ISurface>& surface, AndroidSurfaceOutput * surfaceOutput)
{
	LOGI("CGfxAndroid_MyRenderer::Init - surface %p", surface.get());

	EnterCriticalSection(&g_csDebugOutput);
	if (m_pSurfaceOutput)
	{
		LOGI("CGfxAndroid_MyRenderer::closeFrameBuf");
		m_pSurfaceOutput->closeFrameBuf();
		LOGI("CGfxAndroid_MyRenderer::delete m_pSurfaceOutput");
		delete m_pSurfaceOutput;
		m_pSurfaceOutput = NULL;
	}

	if (surfaceOutput)
	{
		m_pSurfaceOutput = surfaceOutput;
	}
	else
	{
		m_pSurfaceOutput = new AndroidSurfaceOutput;
	}

	LOGI("CGfxAndroid_MyRenderer::Init check");
	m_pSurfaceOutput->initCheck();
	LOGI("CGfxAndroid_MyRenderer::Set");
	m_pSurfaceOutput->set(&m_player, surface, false);

	LeaveCriticalSection(&g_csDebugOutput);
	LOGI("CGfxAndroid_MyRenderer::Init - AndroidSurfaceOutput set OK\n");
}

bool CGfxAndroid_MyRenderer::SetVideoFormat(int videoWidth, int videoHeight, int dispWidth, int dispHeight, PixelFormat /*format*/)
{
	LOGI("CGfxAndroid_MyRenderer::SetVideoFormat - video %dx%d", videoWidth, videoHeight);

	EnterCriticalSection(&g_csDebugOutput);
	if (m_pSurfaceOutput)
	{
		PVMFStatus status = SetMIOParameterUint32((char*)MOUT_VIDEO_WIDTH_KEY,	videoWidth);
		LOGI("set videoWidth out");
		if (status == PVMFSuccess)
		{
			status = SetMIOParameterUint32((char*)MOUT_VIDEO_HEIGHT_KEY, videoHeight);
			LOGI("set videoHeight out");
		}
		if (status == PVMFSuccess)
		{
			status = SetMIOParameterUint32((char*)MOUT_VIDEO_DISPLAY_WIDTH_KEY, dispWidth);
			LOGI("set dispWidth out");
		}
		if (status == PVMFSuccess)
		{
			status = SetMIOParameterUint32((char*)MOUT_VIDEO_DISPLAY_HEIGHT_KEY, dispHeight);
			LOGI("set dispHeight out");
		}
		if (status == PVMFSuccess)
		{

			// ignore status here
#if (ANDROID_VER == 15)
			SetMIOParameterUint32((char*)MOUT_VIDEO_SUBFORMAT_KEY, PVMF_YUV420_PLANAR);
#else
			SetMIOParameterFormat((char*)MOUT_VIDEO_SUBFORMAT_KEY, PVMF_MIME_YUV420);
#endif
			LOGI("set PVMF_MIME_YUV420 out");
		}

		LeaveCriticalSection(&g_csDebugOutput);
		return true;
	}

	LeaveCriticalSection(&g_csDebugOutput);
	return false;
}

void CGfxAndroid_MyRenderer::Start()
{
}

void CGfxAndroid_MyRenderer::Stop()
{
}

void CGfxAndroid_MyRenderer::SendData(unsigned char * data, unsigned int dataBytes)
{
	EnterCriticalSection(&g_csDebugOutput);
	if (m_pSurfaceOutput)
	{
		PvmiMediaXferHeader data_header_info;
		memset(&data_header_info, 0, sizeof(data_header_info));
		m_pSurfaceOutput->writeFrameBuf(data, dataBytes, data_header_info);
	}
	LeaveCriticalSection(&g_csDebugOutput);

}

#endif
