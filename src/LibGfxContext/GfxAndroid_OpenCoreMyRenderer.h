#pragma once

#include <PVPlayer.h>
#include <android_surface_output.h>


class CMyPVPlayer : public android::PVPlayer
{
public:
	CMyPVPlayer() {}
	~CMyPVPlayer() {}

	void sendEvent(int msg, int ext1=0, int ext2=0);
};

#ifdef ANDROID22
#define PIXEL_FORMAT_YCbCr_420_I HAL_PIXEL_FORMAT_YCbCr_420_I
#endif

class CGfxAndroid_MyRenderer
{
public:
	CGfxAndroid_MyRenderer(void);
	~CGfxAndroid_MyRenderer(void);

	void Init(const sp<ISurface>& surface, AndroidSurfaceOutput * surfaceOutput = NULL);

	bool SetVideoFormat(int videoWidth, int videoHeight, int dispWidth, int dispHeight, PixelFormat format = PIXEL_FORMAT_YCbCr_420_I);
	void Start();
	void Stop();
	void SendData(unsigned char * data, unsigned int dataBytes);

protected:
	PVMFStatus SetMIOParameterInt32(PvmiKeyType aKey, int32 aValue);
	PVMFStatus SetMIOParameterUint32(PvmiKeyType aKey, uint32 aValue);
	PVMFStatus SetMIOParameterPchar(PvmiKeyType aKey, char* aValue);
	PVMFStatus SetMIOParameterFormat(PvmiKeyType aKey, PVMFFormatType aFormatType);

private:
	CMyPVPlayer m_player;
	AndroidSurfaceOutput * m_pSurfaceOutput;
};
