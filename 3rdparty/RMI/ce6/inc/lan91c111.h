/*++
Copyright (c) 2004,2005  BSQUARE Corporation.  All rights reserved.

Module Name:

    lan91c111.h

Module Description:

    Definitions for the SMSC LAN91C111 used on the Pb1200
    
Author:
    Ian Rae - 13-Dec-2004

--*/

#ifndef LAN91C111_H
#define LAN91C111_H

#include <pshpack1.h>

typedef volatile struct
{
	union {
		struct {
			USHORT TCR;			
			USHORT EPHSR;		
			USHORT RCR;			
			USHORT ECR;			
			USHORT MIR;			
			USHORT RPCR;		
			USHORT Reserved0;	
		} Bank0;

		struct {
			USHORT CR;			
			USHORT BAR;			
			UCHAR  IAR[6];		
			USHORT GPR;			
			USHORT CTR;			
		} Bank1;

		struct {
			USHORT MMUCR;
			UCHAR  PNR;
			UCHAR  ARR;
			UCHAR  FIFO_TX;
			UCHAR  FIFO_RX;
			USHORT PTR;
			union {
				UCHAR  DATA8[4];
				USHORT DATA16[2];
				ULONG  DATA32;
			};
			union {
				UCHAR  IST;
				UCHAR  ACK;
			};
			UCHAR MSK;
		} Bank2;

		struct {
			UCHAR  MT[8];
			USHORT MGMT;
			USHORT REV;
			USHORT ERCV;
		} Bank3;
	};

	USHORT BANK;

} LAN91C111;

#include <poppack.h>


// Bank 0 - TCR - Transmit Control Register
#define TCR_TXENA		(1<<0)
#define TCR_LOOP		(1<<1)
#define TCR_FORCOL		(1<<2)
#define TCR_PAD_EN		(1<<7)
#define TCR_NOCRC		(1<<8)
#define TCR_MON_CSN		(1<<10)
#define TCR_FDUPLX		(1<<11)
#define TCR_STP_SQET	(1<<12)
#define TCR_EPH_LOOP	(1<<13)
#define TCR_SWFDUP		(1<<15)

// Bank 0 - EPHSR - EPH Status Register
#define EPHSR_TX_SUC	(1<<0)
#define EPHSR_SNGLCOL	(1<<1)
#define EPHSR_MULCOL	(1<<2)
#define EPHSR_LTX_MULT	(1<<3)
#define EPHSR_16COL		(1<<4)
#define EPHSR_SQET		(1<<5)
#define EPHSR_LTX_BRD	(1<<6)
#define EPHSR_TX_DEFR	(1<<7)
#define EPHSR_LATCOL	(1<<9)
#define EPHSR_LOST_CARR	(1<<10)
#define EPHSR_EXC_DEF	(1<<11)
#define EPHSR_CTR_ROL	(1<<12)
#define EPHSR_LINK_OK	(1<<14)
#define EPHSR_TXUNRN	(1<<15)

// Bank 0 - RCR - Receive Control Register
#define RCR_RX_ABORT	(1<<0)
#define RCR_PRMS		(1<<1)
#define RCR_ALMUL		(1<<2)
#define RCR_RXEN		(1<<8)
#define RCR_STRIP_CRC	(1<<9)
#define RCR_ABORT_ENB	(1<<13)
#define RCR_FILT_CAR	(1<<14)
#define RCR_SOFT_RST	(1<<15)

// Bank 0 - ECR - Counter Register
#define ECR_SNGLCOL_S		0
#define ECR_SNGLCOL_MASK	(0xF<<ECR_SNGLCOL_S)
#define ECR_MULCOL_S		4
#define ECR_MULCOL_MASK		(0xF<<ECR_MULCOL_S)
#define ECR_DEF_TX_S		8
#define ECR_DEF_TX_MASK		(0xF<<ECR_DEF_TX_S)
#define ECR_EXC_DEF_TX_S	12
#define ECR_EXC_DEF_TX_MASK	(0xF<<ECR_EXC_DEF_TX_S)

// Bank 0 - MIR - Memory Information Register
#define MIR_SIZE_S			0
#define MIR_SIZE_MASK		(0xFF<<MIR_SIZE_S)
#define MIR_AVAILABLE_S		8
#define MIR_AVAILABLE_MASK	(0xFF<<MIR_AVAILABLE_S)

// Bank 0 - RPCR - Receive/PHY Control Register
#define RPCR_LEDA_N(led)		((led)<<5)
#define RPCR_LEDB_N(led)		((led)<<2)
#define RPCR_LED_10M_OR_100M	0
#define RPCR_LED_10M			2
#define RPCR_LED_FDUPLX			3
#define RPCR_LED_ACTIVITY		4
#define RPCR_LED_100M			5
#define RPCR_LED_RX_ACTIVITY	6
#define RPCR_LED_TX_ACTIVITY	7
#define RPCR_ANEG				(1<<11)
#define RPCR_DPLX				(1<<12)
#define RPCR_SPEED				(1<<13)

// Bank 1 - CR - Configuration Register
#define CR_EXT_PHY			(1<<9)
#define CR_GPCNTRL			(1<<10)
#define CR_NO_WAIT			(1<<12)
#define CR_EPH_POWER_EN		(1<<15)

// Bank 1 - CTR - Control Register
#define CTR_STORE			(1<<0)
#define CTR_RELOAD			(1<<1)
#define CTR_EEPROM_SELECT	(1<<2)
#define CTR_TE_ENABLE		(1<<5)
#define CTR_CR_ENABLE		(1<<6)
#define CTR_LE_ENABLE		(1<<7)
#define CTR_AUTO_RELEASE	(1<<11)
#define CTR_RCV_BAD			(1<<14)

// Bank 2 - MMUCR - MMU Command Register
#define MMUCR_BUSY			(1<<0)
#define MMUCR_CMD_NOOP							(0<<5)
#define MMUCR_CMD_ALLOC_TX_MEM					(1<<5)
#define MMUCR_CMD_RESET_MMU						(2<<5)
#define MMUCR_CMD_REMOVE_RX_FRAME				(3<<5)
#define MMUCR_CMD_REMOVE_AND_RELEASE_RX_FRAME	(4<<5)
#define MMUCR_CMD_RELEASE_PACKET				(5<<5)
#define MMUCR_CMD_ENQUEUE_TX_PACKET				(6<<5)
#define MMUCR_CMD_RESET_TX_FIFOS				(7<<5)

// Bank 2 - ARR - Allocation Result Register
#define AAR_FAILED	(1<<7)

// Bank 2 - FIFO - FIFO Ports Register
#define FIFO_EMPTY	(1<<7)

// Bank 2 - PTR - Pointer Register
#define PTR_POINTER_MASK	(0x7F)
#define PTR_NOT_EMPTY		(1<<11)
#define PTR_ETEN			(1<<12)
#define PTR_READ			(1<<13)
#define PTR_AUTO_INCR		(1<<14)
#define PTR_RCV				(1<<15)

// Bank 2 - IST - Interrpt Status Register
// Bank 2 - ACK - Interrpt Acknowledge Register
// Bank 2 - MSK - Interrpt Mask Register
#define INT_RCV			(1<<0)
#define INT_TX			(1<<1)
#define INT_TX_EMPTY	(1<<2)
#define INT_ALLOC		(1<<3)
#define INT_RX_OVRN		(1<<4)
#define INT_EPH			(1<<5)
#define INT_ERCV		(1<<6)
#define INT_MD			(1<<7)

// Bank 3 - MGMT - Management Interface
#define MGMT_MDO		(1<<0)
#define MGMT_MDI		(1<<1)
#define MGMT_MCLK		(1<<2)
#define MGMT_MDOE		(1<<3)
#define MGMT_MSK_CRS100	(1<<14)

// Bank 3 - ERCV - Early Receive Register
#define ERCV_THRESHOLD_N(n)	((n&0x1F))
#define ERCV_RCV_DISCRD		(1<<7)

//
// PHY definition
//
//

#define PHY_CONTROL_REG			0
#define PHY_STATUS_REG			1
#define PHY_STATUS_OUTPUT_REG	18

// PHY Control Reg
#define PHY_CONTROL_RST			(1<<15)
#define PHY_CONTROL_ANEG_EN		(1<<12)

// PHY Status Reg
#define PHY_STATUS_LINK			(1<<2)
#define PHY_STATUS_REM_FLT		(1<<4)
#define PHY_STATUS_ANEG_ACK		(1<<5)

// PHY Status Output Reg
#define PHY_STATUS_OUTPUT_LINKFAIL	(1<<14)
#define PHY_STATUS_OUTPUT_SPDDET	(1<<7)
#define PHY_STATUS_OUTPUT_DPLDET	(1<<6)

// 
// Frame Format definitions
//
//

#define RFS_ALIGN_ERR		(1<<15)
#define RFS_BROADCAST		(1<<14)
#define RFS_BAD_CRC			(1<<13)
#define RFS_ODD_FRM			(1<<12)
#define RFS_TOO_LONG		(1<<11)
#define RFS_TOO_SHORT		(1<<10)
#define RFS_MULTICAST		(1<<0)

//
// Transmit Frame Control word
//
#define TC_ODD_FRAME		(1<<13)

extern BOOL Lan91c111_Init(UINT8 *pAddress, UINT32 offset, UINT16 mac[3]);
extern void Lan91c111_EnableInts(VOID);
extern void Lan91c111_DisableInts(VOID);
extern UINT16 Lan91c111_SendFrame(BYTE *pData, UINT32 dwLength);
extern UINT16 Lan91c111_GetFrame( BYTE *pData, UINT16 *pwLength);
extern void Lan91c111_ApplyPacketFilter(UINT32 dwFilter);
extern BOOL Lan91c111_ApplyMulticastList(UINT8 *pucMulticastAddresses, UINT32 dwNoOfAddresses);


#endif
