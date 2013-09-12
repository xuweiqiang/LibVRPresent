#include <stdlib.h>
#include "isdbt_cc.h"
#include "cc_decode.h"

HRESULT ISDBTCC_Create(ISDBTCC_HANDLE *handle)
{
	if(handle==NULL)
		return E_FAIL;
	*handle = new IsdbtCCDecode;	
	return S_OK;
}

HRESULT ISDBTCC_Decode(ISDBTCC_HANDLE handle, ISDBTCC_DecodeOptions* option, PBYTE pData, DWORD* pLen)
{
	if(handle==NULL)
		return E_FAIL;

	return reinterpret_cast<IsdbtCCDecode *>(handle)->Decode(pData, pLen, option);
}

HRESULT ISDBTCC_Release(ISDBTCC_HANDLE handle)
{
	if(handle)
    {
        IsdbtCCDecode* pObj =  (IsdbtCCDecode*)handle;
        delete pObj;
    }

	return S_OK;
}

HRESULT ISDBTCC_Open(ISDBTCC_HANDLE handle, ISDBTCC_OpenOptions *option)
{
	if(handle==NULL || option==NULL)
		return E_FAIL;

	return reinterpret_cast<IsdbtCCDecode *>(handle)->Open(option);
}

HRESULT ISDBTCC_Close(ISDBTCC_HANDLE handle)
{
	if(handle)
		return reinterpret_cast<IsdbtCCDecode *>(handle)->Close();

	return E_FAIL;
}

HRESULT ISDBTCC_GetOutput(ISDBTCC_HANDLE handle, ISDBTCC_Output* pOutput)
{
	if(handle)
	{
		return reinterpret_cast<IsdbtCCDecode *>(handle)->GetOutput(pOutput);
	}
	return E_FAIL;
}

HRESULT ISDBTCC_Reset(ISDBTCC_HANDLE handle)
{
	if(handle)
	{
		return reinterpret_cast<IsdbtCCDecode *>(handle)->Reset();
	}
	return E_FAIL;
}

HRESULT ISDBTCC_Set(ISDBTCC_HANDLE handle, DWORD dwPropID, LPVOID pPropData, DWORD cbPropData)
{
	if(handle)
	{
		return reinterpret_cast<IsdbtCCDecode *>(handle)->Set(dwPropID, pPropData, cbPropData);
	}
	return E_FAIL;
}

HRESULT ISDBTCC_Get(ISDBTCC_HANDLE handle, DWORD dwPropID, LPVOID pPropData, DWORD cbPropData)
{
	if(handle)
	{
		return reinterpret_cast<IsdbtCCDecode *>(handle)->Get(dwPropID, pPropData, cbPropData);
	}
	return E_FAIL;
}
