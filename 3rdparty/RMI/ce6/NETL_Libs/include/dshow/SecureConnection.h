#ifndef __SECURE_CONNECTION_H
#define __SECURE_CONNECTION_H

#include <wincrypt.h>
#include <Dshow.h>
#ifdef WINCE
#include "AuMedia.h"
#include "DivXInterface.h"

#define MAX_FILTER_NAME_LEN 256
#define NUM_KEYS			4
#define KEY_LENGTH			140
#define MAX_MESSAGE_LENGTH	256
#define RANDOM_SEED_LENGTH	32

class CSecureConnection : public ISecureConnection
{
private:
	TCHAR   m_pFilterName[MAX_FILTER_NAME_LEN];
	DWORD	m_dwOutgoingSeedLength;
	DWORD	m_dwIncomingSeedLength;
	BOOL	m_bRequireSecureConnection;
	BOOL	m_bConnected;
	LONG	m_cRef;
	BYTE	m_pbMySeed[RANDOM_SEED_LENGTH];
	BYTE	m_pbOtherSeed[RANDOM_SEED_LENGTH];

	ISecureConnection *m_pSecureConnection;

	HINSTANCE	m_hInstDivXLibrary;
	fnEncryptMessage_t m_fnEncryptMessage;
	fnDecryptMessage_t m_fnDecryptMessage;
	fnGetRandomBytes_t m_fnGetRandomBytes;

	int UnloadDivXInterface();
	int LoadDivXInterface();

	int CalculateIndex(PBYTE pbData, DWORD dwDataLen);
	int PrepareMessage(PBYTE pbData1, DWORD dwData1Len, PBYTE pbData2, DWORD dwData2Len, PBYTE pbData3, DWORD dwData3Len, PBYTE pbMessage, DWORD *pdwMessageLen);
	int CompareBytes(PBYTE pbData1, DWORD dwData1Len, PBYTE pbData2, DWORD dwData2Len);
	int ValidateMessage(PBYTE pbData1, DWORD dwData1Len, PBYTE pbData2, DWORD dwData2Len, PBYTE pbData3, DWORD dwData3Len, PBYTE pbMessage, DWORD dwMessageLen);
protected:


public:
	ULONG AddRef();
	ULONG Release();
	HRESULT QueryInterface(const IID &riid, LPVOID *ppvObj);
	CSecureConnection();
	~CSecureConnection();
	HRESULT MakeSecureConnection(IPin *pOtherPin);
	BOOL GetProtectionRequired() { return m_bRequireSecureConnection; };
	BOOL GetConnectionStatus() { return m_bConnected; };
	HRESULT CompleteSecureConnection(PBYTE pbMessage, DWORD dwMessageLength);
	HRESULT BreakSecureConnection();
	BOOL ConnectionOk();
	HRESULT ExchangeSeeds(PBYTE pbMySeed, DWORD dwMySeedLength, PBYTE pbTheirSeed, DWORD dwTheirSeedLength);

	void SetProtectionRequired(BOOL bProtectionRequired) { m_bRequireSecureConnection = bProtectionRequired; };
};
#endif
#endif // __SECURE_CONNECTION_H