//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module:

	Microsoft Pocket IME (pIME) Ver 2.0

File Name:

	arrdata.h

Abstract:

	Array data manager.

Notes: 


--*/

#ifndef __ARRDATA_H__
#define __ARRDATA_H__

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif /* __cplusplus */

//
// Allocation type
//
#define	ARRDATA_ALLOCTYPE_GLOBAL		(123)
#define ARRDATA_ALLOCTYPE_HEAP			(456)
#define	ARRDATA_ALLOCTYPE_USEDEFAULT	(789)

#define ARR_INCSIZE 32

	#define	ARDBG
	#define ARAST(a)
	#define ARAST_EX(a)
	#define ARSetDebug(a)

class ARRData {

private:
	BYTE* m_pbData;
	UINT m_uiCount;
	DWORD m_dwAllocSize;
	DWORD m_dwUnitSize;
	DWORD m_dwIncSize;		// increment size
	static BOOL m_fUseHeap;		// HeapAlloc or GlobalHeap?
	static DWORD m_dwHeapCommit;		// HeapAlloc or GlobalHeap?
	static HANDLE m_hHeap;		// HeapAlloc or GlobalHeap?

public:

	//
	// statics
	//
	static BOOL InitializeMemoryMethod( BOOL fHeap, DWORD dwCommit );
	static BOOL TerminateMemoryMethod(VOID);
	static VOID* AllocPtr( DWORD dwSize, UINT uiAllocType = ARRDATA_ALLOCTYPE_USEDEFAULT);
	static VOID* ReAllocPtr( VOID* pvOrg, DWORD dwSize, UINT uiAllocType = ARRDATA_ALLOCTYPE_USEDEFAULT );
	static VOID FreePtr( VOID* pvOrg );
	static VOID* ARRData::Stamp( VOID* pv, UINT uiStamp, DWORD dwSize )
	{
		UINT_PTR* puiStamp = (UINT_PTR*)pv;
		UINT_PTR uiValue = MAKELONG( LOWORD(uiStamp), LOWORD(dwSize) );
		if( puiStamp ) {
			*puiStamp = uiValue;
			puiStamp++;
			return puiStamp;
		}
		return NULL;
	}

	static UINT_PTR ARRData::GetStamp( VOID* pv )
	{
		UINT_PTR* puiStamp = (UINT_PTR*)pv;
		if( puiStamp ) {
			puiStamp --;	// real stamp is in pv[-1]
			return LOWORD(*puiStamp);
		}
		return 0;
	}

	static UINT_PTR ARRData::GetStamp( VOID* pv, VOID** ppvReal )
	{
		UINT_PTR* puiStamp = (UINT_PTR*)pv;
		if( puiStamp ) {
			puiStamp --;	// real stamp is in pv[-1]
			*ppvReal = (VOID*)puiStamp;
			return LOWORD(*puiStamp);
		}
		return 0;
	}

	////////////////////////////////////////////////////////////////////////////
	// constractor, distractor
	////////////////////////////////////////////////////////////////////////////
	ARRData( DWORD dwUnitSize = 0, DWORD dwIncSize = 16);
	~ARRData();

	////////////////////////////////////////////////////////////////////////////
	// service methods
	////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////
	// BYTE version 
	////////////////////////////////////////////////////////////////////////////
	INT InsertData( UINT iIdx, BYTE b, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(BYTE) );
		return FillData( iIdx, (VOID*)&b, cuiUnits );
	}
	INT InsertData( UINT iIdx, BYTE* pv, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(BYTE) );
		return InsertData( iIdx, (VOID*)pv, cuiUnits );
	}
	INT AppendData( BYTE b ) {
		ARAST_EX( m_dwUnitSize == sizeof(BYTE) );
		if( m_uiCount + 1 <= m_dwAllocSize ) {
			((BYTE*)m_pbData)[m_uiCount++] = b;
			return m_uiCount;
		}
		return FillData( m_uiCount, (VOID*)&b, 1 );
	}
	INT AppendData( BYTE b, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(BYTE) );
		return FillData( m_uiCount, (VOID*)&b, cuiUnits );
	}
	INT AppendData( BYTE* pv )
	{
		ARAST_EX( m_dwUnitSize == sizeof(BYTE) );
		if( m_uiCount + 1 <= m_dwAllocSize ) {
			((BYTE*)m_pbData)[m_uiCount++] = *pv;
			return m_uiCount;
		}
		return InsertData( m_uiCount, (VOID*)pv, 1 );
	}
	INT AppendData( BYTE* pv, UINT cuiUnits )
	{
		return InsertData( m_uiCount, (VOID*)pv, cuiUnits );
	}
	INT SetData( BYTE* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(BYTE) );
		return SetData( (VOID*)pv, cuiUnits );
	}

	////////////////////////////////////////////////////////////////////////////
	// WORD version 
	////////////////////////////////////////////////////////////////////////////
	INT InsertData( UINT iIdx, WORD w, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(WORD) );
		return FillData( iIdx, (VOID*)&w, cuiUnits );
	}
	INT InsertData( UINT iIdx, WORD* pv, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(WORD) );
		return InsertData( iIdx, (VOID*)pv, cuiUnits );
	}
	INT AppendData( WORD w )
	{
		ARAST_EX( m_dwUnitSize == sizeof(WORD) );
		if( ((m_uiCount + 1)*sizeof(WORD)) <= m_dwAllocSize ) {
			((WORD*)m_pbData)[m_uiCount++] = w;
			return m_uiCount;
		}
		return FillData( m_uiCount, (VOID*)&w, 1 );
	}
	INT AppendData( WORD w, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(WORD) );
		return FillData( m_uiCount, (VOID*)&w, cuiUnits );
	}
	INT AppendData( WORD* pv )
	{
		ARAST_EX( m_dwUnitSize == sizeof(WORD) );
		if( ((m_uiCount + 1)*sizeof(WORD)) <= m_dwAllocSize ) {
			((WORD*)m_pbData)[m_uiCount++] = *pv;
			return m_uiCount;
		}
		return InsertData( m_uiCount, (VOID*)pv, 1 );
	}
	INT AppendData( WORD* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(WORD) );
		return InsertData( m_uiCount, (VOID*)pv, cuiUnits );
	}
	INT SetData( WORD* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(WORD) );
		return SetData( (VOID*)pv, cuiUnits );
	}

	////////////////////////////////////////////////////////////////////////////
	// INT version 
	////////////////////////////////////////////////////////////////////////////
	INT InsertData( UINT iIdx, INT i, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return FillData( iIdx, (VOID*)&i, cuiUnits );
	}
	INT InsertData( UINT iIdx, INT* pv, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return InsertData( iIdx, (VOID*)pv, cuiUnits );
	}
	INT AppendData( INT i )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		if( ((m_uiCount + 1)*sizeof(INT)) <= m_dwAllocSize ) {
			((INT*)m_pbData)[m_uiCount++] = i;
			return m_uiCount;
		}
		return FillData( m_uiCount, (VOID*)&i, 1 );
	}
	INT AppendData( INT i, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return FillData( m_uiCount, (VOID*)&i, cuiUnits );
	}
	INT AppendData( INT* pv )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		if( ((m_uiCount + 1)*sizeof(INT)) <= m_dwAllocSize ) {
			((INT*)m_pbData)[m_uiCount++] = *pv;
			return m_uiCount;
		}
		return InsertData( m_uiCount, (VOID*)pv, 1 );
	}
	INT AppendData( INT* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return InsertData( m_uiCount, (VOID*)pv, cuiUnits );
	}
	INT SetData( INT* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return SetData( (VOID*)pv, cuiUnits );
	}

	////////////////////////////////////////////////////////////////////////////
	// UINT version 
	////////////////////////////////////////////////////////////////////////////
	INT InsertData( UINT iIdx, UINT ui, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return FillData( iIdx, (VOID*)&ui, cuiUnits );
	}
	INT InsertData( UINT iIdx, UINT* pv, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return InsertData( iIdx, (VOID*)pv, cuiUnits );
	}
	INT AppendData( UINT ui )
	{
		ARAST_EX( m_dwUnitSize == sizeof(UINT) );
		if( ((m_uiCount + 1)*sizeof(UINT)) <= m_dwAllocSize ) {
			((UINT*)m_pbData)[m_uiCount++] = ui;
			return m_uiCount;
		}
		return FillData( m_uiCount, (VOID*)&ui, 1 );
	}
	INT AppendData( UINT ui, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return FillData( m_uiCount, (VOID*)&ui, cuiUnits );
	}
	INT AppendData( UINT* pv )
	{
		ARAST_EX( m_dwUnitSize == sizeof(UINT) );
		if( ((m_uiCount + 1)*sizeof(UINT)) <= m_dwAllocSize ) {
			((UINT*)m_pbData)[m_uiCount++] = *pv;
			return m_uiCount;
		}
		return InsertData( m_uiCount, (VOID*)pv, 1 );
	}
	INT AppendData( UINT* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return InsertData( m_uiCount, (VOID*)pv, cuiUnits );
	}
	INT SetData( UINT* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(INT) );
		return SetData( (VOID*)pv, cuiUnits );
	}

	////////////////////////////////////////////////////////////////////////////
	// LONG version 
	////////////////////////////////////////////////////////////////////////////
	INT InsertData( UINT iIdx, LONG l, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LONG) );
		return FillData( iIdx, (VOID*)&l, cuiUnits );
	}
	INT InsertData( UINT iIdx, LONG* pv, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LONG) );
		return InsertData( iIdx, (VOID*)pv, cuiUnits );
	}
	INT AppendData( LONG l )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LONG) );
		if( ((m_uiCount + 1)*sizeof(LONG)) <= m_dwAllocSize ) {
			((LONG*)m_pbData)[m_uiCount++] = l;
			return m_uiCount;
		}
		return FillData( m_uiCount, (VOID*)&l, 1 );
	}
	INT AppendData( LONG l, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LONG) );
		return FillData( m_uiCount, (VOID*)&l, cuiUnits );
	}
	INT AppendData( LONG* pv )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LONG) );
		if( ((m_uiCount + 1)*sizeof(LONG)) <= m_dwAllocSize ) {
			((LONG*)m_pbData)[m_uiCount++] = *pv;
			return m_uiCount;
		}
		return InsertData( m_uiCount, (VOID*)pv, 1 );
	}
	INT AppendData( LONG* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LONG) );
		return InsertData( m_uiCount, (VOID*)pv, cuiUnits );
	}
	INT SetData( LONG* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LONG) );
		return SetData( (VOID*)pv, cuiUnits );
	}

	////////////////////////////////////////////////////////////////////////////
	// DWORD version 
	////////////////////////////////////////////////////////////////////////////
	INT InsertData( UINT iIdx, DWORD dw, UINT cuiUnits = 1)
	{
		ARAST_EX( m_dwUnitSize == sizeof(DWORD) );
		return FillData( iIdx, (VOID*)&dw, cuiUnits );
	}
	INT InsertData( UINT iIdx, DWORD* pv, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(DWORD) );
		return InsertData( iIdx, (VOID*)pv, cuiUnits );
	}
	INT AppendData( DWORD dw )
	{
		ARAST_EX( m_dwUnitSize == sizeof(DWORD) );
		if( ((m_uiCount + 1)*sizeof(DWORD)) <= m_dwAllocSize ) {
			((DWORD*)m_pbData)[m_uiCount++] = dw;
			return m_uiCount;
		}
		BYTE* pb = (BYTE*)&dw;
		return FillData( m_uiCount, (VOID*)pb, 1 );
	}
	INT AppendData( DWORD dw, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(DWORD) );
		BYTE* pb = (BYTE*)&dw;
		return FillData( m_uiCount, (VOID*)pb, cuiUnits );
	}
	INT AppendData( DWORD* pv )
	{
		ARAST_EX( m_dwUnitSize == sizeof(DWORD) );
		if( ((m_uiCount + 1)*sizeof(DWORD)) <= m_dwAllocSize ) {
			((DWORD*)m_pbData)[m_uiCount++] = *pv;
			return m_uiCount;
		}
		return InsertData( m_uiCount, (VOID*)pv, 1 );
	}
	INT AppendData( DWORD* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(DWORD) );
		return InsertData( m_uiCount, (VOID*)pv, cuiUnits );
	}
	INT SetData( DWORD* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(DWORD) );
		return SetData( (VOID*)pv, cuiUnits );
	}

#ifdef _WIN64
	////////////////////////////////////////////////////////////////////////////
	// LPARAM version 
	////////////////////////////////////////////////////////////////////////////
	INT InsertData( UINT iIdx, LPARAM lpara, UINT cuiUnits = 1)
	{
		ARAST_EX( m_dwUnitSize == sizeof(LPARAM) );
		return FillData( iIdx, (VOID*)&lpara, cuiUnits );
	}
	INT InsertData( UINT iIdx, LPARAM* plpara, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LPARAM) );
		return InsertData( iIdx, (VOID*)plpara, cuiUnits );
	}
	INT AppendData( LPARAM lpara, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LPARAM) );
		BYTE* pb = (BYTE*)&lpara;
		return FillData( m_uiCount, (VOID*)pb, cuiUnits );
	}
	INT AppendData( LPARAM* plpara, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LPARAM) );
		return InsertData( m_uiCount, (VOID*)plpara, cuiUnits );
	}
	INT SetData( LPARAM* pv, UINT cuiUnits )
	{
		ARAST_EX( m_dwUnitSize == sizeof(LPARAM) );
		return SetData( (VOID*)pv, cuiUnits );
	}
#endif

	////////////////////////////////////////////////////////////////////////////
	// VOID version 
	////////////////////////////////////////////////////////////////////////////
	INT AppendData( VOID* pv, UINT cuiUnits = 1 )
	{
		ARAST_EX( m_dwUnitSize == sizeof(DWORD) );
		return InsertData( m_uiCount, (VOID*)pv, cuiUnits );
	}

	////////////////////////////////////////////////////////////////////////////
	// Basic methods
	////////////////////////////////////////////////////////////////////////////
	VOID Init(DWORD dwUnitSize, DWORD dwIncSize = 16 );
	VOID* ReAlloc( DWORD dwSize );
	INT InsertData( UINT iIdx, VOID* pv, UINT cuiUnits = 1);
	INT FillData( UINT iIdx, VOID* pv, UINT cuiUnits = 1);
	INT DeleteData( UINT iIdx, UINT cuiUnits = 1 );
	INT SetData( VOID* pv, UINT cuiUnits );
	VOID* GetData( UINT iIdx );

	DWORD GetMaxBufferSize(VOID)
	{
		ARAST_EX( m_dwUnitSize != 0 );
		return m_dwAllocSize;
	}
	VOID* GetBuffer(VOID)
	{
		ARAST_EX( m_dwUnitSize != 0 );
		return (VOID*)m_pbData;
	}
	UINT GetCount(VOID)
	{
		ARAST_EX( m_dwUnitSize != 0 );
		return m_uiCount;
	}
	VOID Reset(VOID) {
		ARAST_EX( m_dwUnitSize != 0 );
		m_uiCount = 0;
	}

};

#ifdef __cplusplus
} /* end of 'extern "C" {' */
#endif	// __cplusplus

#endif //__ARRDATA_H__
