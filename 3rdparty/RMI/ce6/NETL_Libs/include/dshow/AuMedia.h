#ifndef __AUMEDIA_H
#define __AUMEDIA_H

// {0ba13ea1-70e5-11db-9690-00e08161165f}
DEFINE_GUID(CLSID_VREND, 
0xba13ea1, 0x70e5, 0x11db, 0x96, 0x90, 0x0, 0xe0, 0x81, 0x61, 0x16, 0x5f );

// {3145504D-0000-0010-8000-00AA00389B71}
DEFINE_GUID(MEDIASUBTYPE_MPE1, 
0x3145504D, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);

// {7A46365B-568E-40CD-93B5-5F9B2DC1CAE7
DEFINE_GUID(IID_AuMediaSample, 
0x7A46365B, 0x568E, 0x40CD, 0x93, 0xB5, 0x5F, 0x9B, 0x2D, 0xC1, 0xCA, 0xE7);

// {5CB603A4-6348-4A87-ABF2-62CDF335309D}
DEFINE_GUID(IID_IAuDecoder,
0x5CB603A4, 0x6348, 0x4A87, 0xAB, 0xF2, 0x62, 0xCD, 0xF3, 0x35, 0x30, 0x9D);


DECLARE_INTERFACE_(IAuMediaSample,IUnknown)
{
public:
	// IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID *ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IAuMediaSample
	STDMETHOD_(HRESULT,GetPhysPointer)(BYTE** ppBuffer) PURE;
	STDMETHOD_(HRESULT,GetVirtPointer)(BYTE** ppBuffer) PURE;
};

DECLARE_INTERFACE_(IAuDecoder,IUnknown)
{
public:
	// IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID *ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IAuMediaSample
	STDMETHOD_(HRESULT,ToggleVideo)(BOOL bTurnVideoOn) PURE;
	STDMETHOD_(HRESULT,DecodeEnabled)(BOOL *bEnabled) PURE;
};

// {603DCEAC-A54F-47E3-B66D-7E5B8F5C81ED}
DEFINE_GUID(IID_DivXInterface, 
0x603DCEAC, 0xA54F, 0x47E3, 0xB6, 0x6D, 0x7E, 0x5B, 0x8F, 0x5C, 0x81, 0xED);

DECLARE_INTERFACE_(IDivXInterface,IUnknown)
{
public:
	// IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID *ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IAuDivXDrm
	STDMETHOD_(HRESULT,PrepareDivXPlayback)(BYTE *pRentalMessageFlag, BYTE *pUseLimit, BYTE *pUseCount) PURE;
	STDMETHOD_(HRESULT,CommitDivXPlayback)() PURE;
};


// {730C8DB2-905E-42FD-BE51-3E49033B0384}
DEFINE_GUID(IID_SecureConnection, 
0x730C8DB2, 0x905E, 0x42FD, 0xBE, 0x51, 0x3E, 0x49, 0x03, 0x3B, 0x03, 0x84);

DECLARE_INTERFACE_(ISecureConnection,IUnknown)
{
public:
	// IUnknown
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, LPVOID *ppvObj) PURE;
    STDMETHOD_(ULONG,AddRef)(THIS) PURE;
    STDMETHOD_(ULONG,Release)(THIS) PURE;

	// IAuDivXDrm
	STDMETHOD_(BOOL,GetProtectionRequired)() PURE;
	STDMETHOD_(HRESULT,CompleteSecureConnection)(PBYTE pbMessage, DWORD dwMessageLength) PURE;
	STDMETHOD_(HRESULT,BreakSecureConnection)() PURE;
	STDMETHOD_(HRESULT,ExchangeSeeds)(PBYTE pbMySeed, DWORD dwMySeedLength, PBYTE pbTheirSeed, DWORD dwTheirSeedLength) PURE;
};

// Custom Error Codes
#define EC_UNSUPPORTED_MEDIA					(EC_USER + 1)

#endif __AUMEDIA_H