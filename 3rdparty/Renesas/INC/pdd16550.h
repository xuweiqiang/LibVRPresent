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

Module Name:  

Abstract:

    Platform dependent Serial definitions for 16550  controller.

Notes: 
--*/
#ifndef __PDD16550_H_
#define __PDD16550_H_
#include <cserpdd.h>
class CReg16550 {
public:
    CReg16550(PBYTE pRegAddr, DWORD dwStride);
    virtual ~CReg16550() { ; };
    virtual BOOL    Init() { return (m_pReg!=NULL); } ;
    // We do not virtual Read & Write data because of Performance Concern.
    virtual void    Write_DATA(UINT8 uData) { WRITE_PORT_UCHAR( m_pData, uData); };
    virtual UINT8   Read_Data() { return READ_PORT_UCHAR(m_pData); } ;
    virtual void    Write_IER(UINT8 uData) { WRITE_PORT_UCHAR(m_pIER, uData); };
    virtual UINT8   Read_IER() { return READ_PORT_UCHAR(m_pIER); };
    virtual void    Write_FCR(UINT8 uData) { WRITE_PORT_UCHAR( m_pIIR_FCR, uData);m_FCR = uData; };
    virtual UINT8   Read_FCR() { return m_FCR; };
    virtual UINT8   Read_IIR() { return READ_PORT_UCHAR( m_pIIR_FCR) ;};
    virtual void    Write_LCR(UINT8 uData) { WRITE_PORT_UCHAR(m_pLCR, uData);};
    virtual UINT8   Read_LCR() { return READ_PORT_UCHAR(m_pLCR);};
    virtual void    Write_MCR(UINT8 uData) { WRITE_PORT_UCHAR( m_pMCR, uData);};
    virtual UINT8   Read_MCR() { return READ_PORT_UCHAR(m_pMCR);};
    virtual void    Write_LSR(UINT8 uData) { WRITE_PORT_UCHAR( m_pLSR, uData);};
    virtual UINT8   Read_LSR() { return READ_PORT_UCHAR(m_pLSR);};
    virtual void    Write_MSR(UINT8 uData) { WRITE_PORT_UCHAR( m_pMSR, uData) ; };
    virtual UINT8   Read_MSR() { return READ_PORT_UCHAR( m_pMSR); };
    virtual void    Write_SCR(UINT8 uData) { WRITE_PORT_UCHAR( m_pSRC, uData );};
    virtual UINT8   Read_SCR() { return READ_PORT_UCHAR(m_pSRC); };

    virtual BOOL    Write_BaudRate(UINT16 uData);

    virtual void    Backup();
    virtual void    Restore();
#ifdef DEBUG
    virtual void    DumpRegister();
#endif
protected:
    PBYTE  const  m_pReg;
    DWORD  const  m_dwStride;
    BYTE    m_FCR; // Because it is writeonly register.
    BOOL    m_fIsBackedUp;
    PBYTE   m_pData;    
    PBYTE   m_pIER;
    PBYTE   m_pIIR_FCR;
    PBYTE   m_pLCR;
    PBYTE   m_pMCR;
    PBYTE   m_pLSR;
    PBYTE   m_pMSR;
    PBYTE   m_pSRC;

private:
    BYTE    m_IERBackup;
    BYTE    m_LCRBackup;
    BYTE    m_MCRBackup;
    UINT16  m_BaudRate;
    
};

// Memory Mapped IO version.
class CMemReg16550: public CReg16550 {
public:
    CMemReg16550(PBYTE pRegAddr, DWORD dwStride) 
        :CReg16550(pRegAddr,dwStride) 
    {;}
    // We do not virtual Read & Write data because of Performance Concern.
    virtual void    Write_DATA(UINT8 uData) { WRITE_REGISTER_UCHAR( m_pData, uData); };
    virtual UINT8   Read_Data() { return READ_REGISTER_UCHAR(m_pData); } ;
    virtual void    Write_IER(UINT8 uData) { WRITE_REGISTER_UCHAR(m_pIER, uData); };
    virtual UINT8   Read_IER() { return READ_REGISTER_UCHAR(m_pIER); };
    virtual void    Write_FCR(UINT8 uData) { WRITE_REGISTER_UCHAR( m_pIIR_FCR, uData);m_FCR = uData; };
    virtual UINT8   Read_IIR() { return READ_REGISTER_UCHAR( m_pIIR_FCR) ;};
    virtual void    Write_LCR(UINT8 uData) { WRITE_REGISTER_UCHAR(m_pLCR, uData);};
    virtual UINT8   Read_LCR() { return READ_REGISTER_UCHAR(m_pLCR);};
    virtual void    Write_MCR(UINT8 uData) { WRITE_REGISTER_UCHAR( m_pMCR, uData);};
    virtual UINT8   Read_MCR() { return READ_REGISTER_UCHAR(m_pMCR);};
    virtual void    Write_LSR(UINT8 uData) { WRITE_REGISTER_UCHAR( m_pLSR, uData);};
    virtual UINT8   Read_LSR() { return READ_REGISTER_UCHAR(m_pLSR);};
    virtual void    Write_MSR(UINT8 uData) { WRITE_REGISTER_UCHAR( m_pMSR, uData) ; };
    virtual UINT8   Read_MSR() { return READ_REGISTER_UCHAR( m_pMSR); };
    virtual void    Write_SCR(UINT8 uData) { WRITE_REGISTER_UCHAR( m_pSRC, uData );};
    virtual UINT8   Read_SCR() { return READ_REGISTER_UCHAR(m_pSRC); };
};


//#define HIGH_WATER_SIZE     4
typedef struct  __PAIRS {
    ULONG   Key;
    ULONG   AssociatedValue;
} PAIRS, *PPAIRS;


class CPdd16550 : public CSerialPDD, public CMiniThread  {
public:
    CPdd16550 (LPTSTR lpActivePath, PVOID pMdd, PHWOBJ pHwObj);
    virtual ~CPdd16550();
    virtual BOOL Init();
    virtual void PostInit();
    virtual BOOL MapHardware();
    virtual BOOL CreateHardwareAccess();
//  Power Manager Required Function.
    virtual void    SerialRegisterBackup() { m_pReg16550->Backup(); };
    virtual void    SerialRegisterRestore() { m_pReg16550->Restore(); };

// Implement CPddSerial Function.
// Interrupt
    virtual BOOL    InitialEnableInterrupt(BOOL bEnable ) ; // Enable All the interrupt may include Xmit Interrupt.
private:
    virtual DWORD ThreadRun();   // IST
//  Tx Function.
public:
    virtual BOOL    InitXmit(BOOL bInit);
    virtual void    XmitInterruptHandler(PUCHAR pTxBuffer, ULONG *pBuffLen);
    virtual void    XmitComChar(UCHAR ComChar);
    virtual BOOL    EnableXmitInterrupt(BOOL bEnable);
    virtual BOOL    CancelXmit();
    virtual DWORD   GetWriteableSize();
protected:
    BOOL    m_XmitFifoEnable;
    HANDLE  m_XmitFlushDone;

//
//  Rx Function.
public:
    virtual BOOL    InitReceive(BOOL bInit);
    virtual ULONG   ReceiveInterruptHandler(PUCHAR pRxBuffer,ULONG *pBufflen);
    virtual ULONG   CancelReceive();
    virtual DWORD   GetWaterMark();
    virtual BYTE    GetWaterMarkBit();
protected:
    BOOL    m_bReceivedCanceled;
    DWORD   m_dwWaterMark;
//
//  Modem
public:
    virtual BOOL    InitModem(BOOL bInit);
    virtual void    ModemInterruptHandler() { GetModemStatus();};
    virtual ULONG   GetModemStatus();
    virtual void    SetDTR(BOOL bSet);
    virtual void    SetRTS(BOOL bSet);
//
// Line Function.
    virtual BOOL    InitLine(BOOL bInit) ;
    virtual void    LineInterruptHandler() { GetLineStatus();};
    virtual void    SetBreak(BOOL bSet) ;
    virtual BOOL    SetBaudRate(ULONG BaudRate,BOOL bIrModule) ;
    virtual BOOL    SetByteSize(ULONG ByteSize);
    virtual BOOL    SetParity(ULONG Parity);
    virtual BOOL    SetStopBits(ULONG StopBits);
//
// Line Internal Function
    BYTE            GetLineStatus();

protected:
    CReg16550 *  m_pReg16550;
    PVOID       m_pRegVirtualAddr;
    BOOL        m_bIsIo;
    DWORD       m_dwRegStride;
    
    CRegistryEdit m_ActiveReg;
//  Interrupt Handler
    DWORD       m_dwSysIntr;
    HANDLE      m_hISTEvent;
// Optional Parameter
    DWORD m_dwDevIndex;

};
// Constants
#define IER_NORMAL_INTS (SERIAL_IER_RDA | SERIAL_IER_RLS | SERIAL_IER_MS)

#endif
