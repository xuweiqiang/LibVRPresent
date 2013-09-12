// $Change: 395037 $
// mpif.cpp: implementation of the mpif class.
//
//////////////////////////////////////////////////////////////////////

#ifdef GFX_MSTAR
#include "stdafx.h"
#include "mpif.h"

#include "columbus_typedefs.h"
#include "columbus_mpif.h"
//#include "ceddkex.h"
#include "columbus_base_regs.h"
#include "columbus_chiptop.h"
#include "ceddk.h"
#include "cmmb_dll.h"

#pragma comment(lib, "mpif.lib")
#pragma comment(lib, "coredll.lib")
#pragma comment(lib, "ceddk.lib")
 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define INREG8(x)           READ_REGISTER_UCHAR(x)
#define OUTREG8(x, y)       WRITE_REGISTER_UCHAR(x, (UCHAR)(y))
#define SETREG8(x, y)       OUTREG8(x, INREG8(x)|(y))
#define CLRREG8(x, y)       OUTREG8(x, INREG8(x)&~(y))

#define INREG16(x)          READ_REGISTER_USHORT(x)
#define OUTREG16(x, y)      WRITE_REGISTER_USHORT(x,(USHORT)(y))
#define SETREG16(x, y)      OUTREG16(x, INREG16(x)|(y))
#define CLRREG16(x, y)      OUTREG16(x, INREG16(x)&~(y))


#define GPIO_ARGO_POWER_ON    (1<<6)		/* GPIO G22 */
#define GPIO_ARGO_POWER_RESET    (1<<5)		/* GPIO G05 */


#define TWO_BIT_MODE    0
#define FOUR_BIT_MODE   1
#define MEMSIZE 512 * 1024 //4*1024*1024, slave memory size
#define MIU_MODE_MAX_DATA_BYTES_3A 128*1024	//64k bytes are verified
#define BUFLEN 	MIU_MODE_MAX_DATA_BYTES_3A


//=======================================================
//=======================================================
//=======================================================
#define MBOX_BANK 19
#define MBOX_BASE           0x9300
#define MBOX_R(x)           ((MBOX_BASE|(x<<1))*2)

// bank ID
#define AEON_G_BANK 0x0
#define AEON_V0_BANK 0x10//0x90
#define AEON_V1_BANK 0x12//0x92
#define AEON_A0_BANK 0x11//0x91

#define BANK_MAILBOX 0x13//0x93
#define BANK_HWSEM   0x14//0x94
#define BANK_AEONINT 0x15//0x95

#define AEON_PH_BANK 0x02//0x82

// AEON_G_BANK: offset
#define AEON_G_CPU_CLK0   0x28
#define AEON_G_CPU_CLK1   0x2A

#define AEON_ENABLE     0xF0 //0x78 //

#define AEON_IMEM_ADDR0 0xE4 //0x72 //
#define AEON_IMEM_ADDR1 0xE5 //0x72 //
#define AEON_IMEM_ADDR2 0xE3 //0x71 //
#define AEON_DMEM_ADDR0 0xF4 //0x7a //
#define AEON_DMEM_ADDR1 0xF5 //0x7a //
#define AEON_DMEM_ADDR2 0xF6 //0x7b //

#define AEON_SPI_SPACE  0xFE

#define AEON_ISB        0xE7

#define AEON_SWRESET    0xC6 //0x63 //
#define AEON_UARTSEL    0xC0 //0x60 //
#define AEON_UART_V0	3
#define AEON_UART_V1	4
#define AEON_UART_A0	5

#define AEON_QMEM_DMASK0 0xE8
#define AEON_QMEM_DMASK1 0xEA
#define AEON_QMEM_DADDR0 0xEC
#define AEON_QMEM_DADDR1 0xEE

#define AEON_HW_DMX_RESET 0x10

#define AEON_V0_ISB 4  // 1~4
#define AEON_V1_ISB 4
#define AEON_A0_ISB 4

#define AEON_V0_ID      0
#define AEON_V1_ID      2
#define AEON_A0_ID      1

#define AEON_V0_I_BASE  0
#define AEON_V0_D_BASE  AEON_V0_I_BASE
#define AEON_V0_SIZE    380*1024 //550*1024//380*1024 //(AEON_V0_CODE_SIZE + AEON_V0_HARDCODE_SIZE + AEON_V0_DATA_SIZE) //0x650000 //

#define AEON_V1_I_BASE  (AEON_V0_I_BASE + AEON_V0_SIZE)
#define AEON_V1_D_BASE  AEON_V1_I_BASE
#define AEON_V1_SIZE    1100*1024//1400*1024//1100*1024//1400*1024 //(AEON_V1_CODE_SIZE + AEON_V1_HARDCODE_SIZE + AEON_V1_DATA_SIZE) //0x650000 //

#define AEON_A0_I_BASE  (AEON_V1_I_BASE + AEON_V1_SIZE)
#define AEON_A0_D_BASE  AEON_A0_I_BASE
#define AEON_A0_SIZE    1580*1024//1050*1024//1580*1024//1350*1024//1350*1024//1050*1024 //700*1024 //(AEON_A0_CODE_SIZE + AEON_A0_HARDCODE_SIZE + AEON_A0_DATA_SIZE) //0x650000 //

#define AEON_END        (AEON_A0_I_BASE + AEON_A0_SIZE)


#define CVT_IMEM_ADDR(x) ((x)>>4)
#define CVT_DMEM_ADDR(x) ((x)>>3)

#define SUPPORT_AEON_V0 1
#define SUPPORT_AEON_V1 1
#define SUPPORT_AEON_A0 1


#ifdef _FPGA_
#define AEON_G_CPU_30_85M  0x0
#define AEON_G_CPU_32_4M   0x1
#define AEON_G_CPU_36M     0x2
#define AEON_G_CPU_13_5M   0x3

#define AEON_V0_CPU_CLK   AEON_G_CPU_30_85M
#define AEON_V1_CPU_CLK   AEON_G_CPU_36M
#define AEON_A0_CPU_CLK   AEON_G_CPU_32_4M

#define AEON_G_CONF_MIU   0x2A
#define AEON_G_MIU_MASK   0xC0
#define AEON_G_MIU_12_7M  0x0
#define AEON_G_MIU_13_5M  0x40
#define AEON_G_MIU_27_0M  0xC0
#else
//clock control register; [0]=1:gate clock, [1]=1:invert clock. [7:2]: clock rate sel.
//00xx00 for 166.4MHz.
//00xx01 for 138.667MHz
//00xx10 for 104MHz
//00xx11 for XTALI.
//0100xx for 83.2MHz.
//0101xx for 69.333MHz
//0110xx for 52MHz
//0111xx for XTALI.
//1xxxxx for DFT clk.
#define AEON_G_CPU_GATE_CLOCK       0
#define AEON_G_CPU_INVERT_CLOCK     0
#define AEON_G_CPU_166_4M       0x0000
#define AEON_G_CPU_138_667M     0x0001
#define AEON_G_CPU_104M         0x0002
#define AEON_G_CPU_XTALI        0x0003
#define AEON_G_CPU_83_2M        0x0010
#define AEON_G_CPU_69_333M      0x0014
#define AEON_G_CPU_52_0M        0x0018
#define AEON_G_CPU_XTALI2       0x001c
#define AEON_G_CPU_DFT          0x0020

#define AEON_V0_CPU_CLK   AEON_G_CPU_166_4M //AEON_G_CPU_138_667M //AEON_G_CPU_166_4M //AEON_G_CPU_XTALI //
#define AEON_V1_CPU_CLK   AEON_G_CPU_166_4M //AEON_G_CPU_138_667M //AEON_G_CPU_166_4M //AEON_G_CPU_XTALI //
#define AEON_A0_CPU_CLK   AEON_G_CPU_166_4M //AEON_G_CPU_138_667M //AEON_G_CPU_166_4M //AEON_G_CPU_XTALI //
#endif

typedef U32 demux_reg_t;
//=======================================================
//=======================================================
//=======================================================

#define CARINA_1A_TEST  0
#define CARINA_2A_TEST  0
#define CARINA_2A_TEST_XIU  0
#define CARINA_2A_TEST_0xF0F0 0
#define CARINA_3A_TEST  0
#define CARINA_3A_TEST_RIU    0     /* SPIF use RIU */
#define CARINA_3A_TEST_MIU      0 /* SPIF use RIU */
#define CARINA_3A_TIMER_MEASURMENT_TEST     0

//#define DMA_BY_USER   1

//#if (CARINA_3A_TEST_RIU || CARINA_3A_TEST_MIU)
//#define BUFFER_SIZE 				16
//#define BUFFER_SIZE_16 		16
//#else
#define BUFFER_SIZE_16 		16
#define BUFFER_SIZE  (1<<17)    //128KB
//#define BUFFER_SIZE  (1<<16)    //64KB
//#define BUFFER_SIZE  (1<<14)    //16KB
//#endif

UINT32 hydra_handler_pif;
volatile PREG_MPIFDMA_st g_pMPIFDMARegs = NULL;
static volatile PREG_CHIPTOP_st g_pCHIPTOPReg_s = NULL;
U8 buf[BUFLEN];

extern U8  bin8051[];
extern DWORD	Bin8051Size(void);

//=======================================================
#define ONOFF_DAR_PATH(enable) { \
    pmstar_multi_pif_put_st pif_put; \
    MPIF_ERROR_CODE_et err1 = MPIF_UNKNOWN_ERROR; \
    U32 wval2 = enable; \
    pif_put.handler = hydra_handler_pif; \
    pif_put.pput_data_buffer = & wval2; \
    pif_put.eChannel = LC1A; \
    pif_put.slave_address = 1; \
    err1 = mstar_multi_pif_put_data(&pif_put); \
    if (err1 != MPIF_SUCCESS) \
        _tprintf(_T("err = %d\r\n"),err1); \
}

#define wreg(bank, addr, val) { \
    pmstar_multi_pif_put_st pif_put; \
    MPIF_ERROR_CODE_et err1 = MPIF_UNKNOWN_ERROR; \
    int addr2; \
    /* MUST use U32 since driver uses U32 */\
    U32 wval2 = val; \
    pif_put.handler = hydra_handler_pif; \
    pif_put.eChannel = LC2A; \
    addr2 = ((1<<15) | ((bank)<<8) | ((addr)*2))>>1; \
    pif_put.pput_data_buffer = & wval2; \
    pif_put.slave_address = addr2; \
    err1 = mstar_multi_pif_put_data(&pif_put); \
    if (err1 != MPIF_SUCCESS) \
        _tprintf(_T("err = %d\r\n"),err1); \
}

#define rreg(bank, addr, val) { \
    pmstar_multi_pif_get_st pif_get; \
    MPIF_ERROR_CODE_et err1 = MPIF_UNKNOWN_ERROR; \
    int addr2; \
    pif_get.handler = hydra_handler_pif; \
    pif_get.eChannel = LC2A; \
    addr2 = ((1<<15) | ((bank)<<8) | ((addr)*2))>>1; \
    pif_get.pget_data_buffer = & val; \
    pif_get.slave_address = addr2; \
    err1 = mstar_multi_pif_get_data(&pif_get); \
    if (err1 != MPIF_SUCCESS) \
        _tprintf(_T("err = %d\r\n"),err1); \
}

#define SET_SPIF_REG(_addr_, _val_) { \
    pmstar_multi_pif_put_st pif_put_2A; \
    MPIF_ERROR_CODE_et err1 = MPIF_UNKNOWN_ERROR; \
    U32 wval2; \
    pif_put_2A.handler = hydra_handler_pif; \
    pif_put_2A.eChannel = LC2A; \
    pif_put_2A.pput_data_buffer = & wval2; \
    wval2 = _val_; \
    pif_put_2A.slave_address = _addr_; \
    err1 = mstar_multi_pif_put_data(&pif_put_2A); \
    if (err1 != MPIF_SUCCESS) \
        _tprintf(_T("err = %d\r\n"),err1); \
}

#define SET_SPIF_3A_ADDR_LEN(addr, _len_) { \
    pmstar_multi_pif_put_st pif_put_2A; \
    MPIF_ERROR_CODE_et err1 = MPIF_UNKNOWN_ERROR; \
    U32 wval2; \
	pif_put_2A.handler = hydra_handler_pif; \
	pif_put_2A.eChannel = LC2A; \
    pif_put_2A.pput_data_buffer = & wval2; \
    wval2 = ((addr)>>3) & 0xffff; \
	pif_put_2A.slave_address = 0x33; \
	err1 = mstar_multi_pif_put_data(&pif_put_2A); \
    \
    wval2 = ((addr)>>3) >> 16; \
	pif_put_2A.slave_address = 0x34;  \
	err1 = mstar_multi_pif_put_data(&pif_put_2A); \
    \
	wval2 = ((_len_)+15)>>4; \
	pif_put_2A.slave_address = 0x32; \
	err1 = mstar_multi_pif_put_data(&pif_put_2A); \
}

#define FIRE_MPIF_3A_WRITE(mode1, _len_) { \
    pmstar_multi_pif_put_st pif_put; \
    MPIF_ERROR_CODE_et err1 = MPIF_UNKNOWN_ERROR; \
	pif_put.handler = hydra_handler_pif; \
    pif_put.mode = mode1; \
    pif_put.put_length = ((_len_)+15)>>4; \
    pif_put.pput_data_buffer = NULL; \
	pif_put.eChannel = LC3A; \
    err1 = mstar_multi_pif_put_data(&pif_put); \
    if (err1 != MPIF_SUCCESS) \
		RETAILMSG(DBG_ARGO_INIT, (_T(" err = %d\n"), err1));\
}

#define FIRE_MPIF_3A_READ(mode1, _len_) { \
    pmstar_multi_pif_get_st pif_get; \
    MPIF_ERROR_CODE_et err1 = MPIF_UNKNOWN_ERROR; \
	pif_get.handler = hydra_handler_pif; \
    pif_get.mode = mode1; \
    pif_get.get_length = ((_len_)+15)>>4; \
    pif_get.pget_data_buffer = NULL; \
    pif_get.eChannel = LC3A; \
    err1 = mstar_multi_pif_get_data(&pif_get); \
    if (err1 != MPIF_SUCCESS) \
        RETAILMSG(DBG_ARGO_INIT, (_T(" err = %d\n"), err1));\
}

#define wreg_b(banknum, addr, data) { \
    demux_reg_t lregval, lregval2; \
    unsigned short addr_t; \
    addr_t = addr>>1; \
	rreg(banknum, addr_t, lregval); \
    if (addr&1) { \
        lregval2 = (lregval&0xff) | ((data&0xff)<<8); \
    } else { \
        lregval2 = (lregval&0xff00) | (data&0xff); \
    } \
	wreg(banknum, addr_t, lregval2); \
	rreg(banknum, addr_t, lregval); \
}

#define mmp_thread_delay(x) ;
//===================================================


#define	DEBUG_MPIF		1

#if DEBUG_MPIF
#define DBG_MPIF_INIT		1
#define DBG_MPIF_1A		1
#define DBG_MPIF_2A		1
#define DBG_MPIF_3A		1
#define DBG_ARGO_INIT	1
#define FUNCTION_ENTER_MSG() RETAILMSG(1, (_T("%s ++\r\n"), TEXT(__FUNCTION__)))
#define FUNCTION_LEAVE_MSG() RETAILMSG(1, (_T("%s --\r\n"), TEXT(__FUNCTION__)))
#else
#define DBG_MPIF_INIT		0
#define DBG_MPIF_1A		0
#define DBG_MPIF_2A		0
#define DBG_MPIF_3A		0
#define DBG_ARGO_INIT	0
#define FUNCTION_ENTER_MSG()
#define FUNCTION_LEAVE_MSG()
#endif

//=======================================================
//map physical addr for app
typedef struct {
	void*    pvDestMem;
	DWORD    dwPhysAddr;
	DWORD    dwSize;
} VIRTUAL_COPY_EX_DATA;


volatile LPVOID GetVirtualAddr(DWORD dwPhyBaseAddress, DWORD dwSize)
{
	volatile LPVOID pVirtual;
	VIRTUAL_COPY_EX_DATA vced;

	if(dwPhyBaseAddress&0xFFF)
	{
		return NULL;
	}
	vced.dwPhysAddr = dwPhyBaseAddress>>8;
	pVirtual = VirtualAlloc(0,dwSize,MEM_RESERVE,PAGE_NOACCESS);
	vced.pvDestMem = pVirtual;
	vced.dwSize = dwSize;

	if(KernelIoControl(IOCTL_VIRTUAL_COPY_EX,&vced, sizeof(vced), NULL, NULL, NULL))
		return pVirtual;
	else
		return NULL;
}
VOID UnmapVirtualAddr(VOID *pAddress, U32 size)
{
    VirtualFree(
        (VOID*)((U32)pAddress & ~(U32)(PAGE_SIZE - 1)), 0, MEM_RELEASE
        );
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Cmpif::Cmpif()
{
	m_tx = NULL;
	m_rx = NULL;
}

Cmpif::~Cmpif()
{

}

BOOL Cmpif::MpifInit()
{
	//PHYSICAL_ADDRESS MPIFDMAPhysicalBase = {COLUMBUS_BASE_DMA_MPIF_PA, 0};
	DWORD i,wval;
	MPIF_ERROR_CODE_et err = MPIF_UNKNOWN_ERROR;

	FUNCTION_ENTER_MSG();

	//MPIFDMAPhysicalBase.QuadPart= COLUMBUS_BASE_DMA_MPIF_PA;
	//m_pMPIFDMARegs = (PREG_MPIFDMA_st)MmMapIoSpace(MPIFDMAPhysicalBase, sizeof(REG_MPIFDMA_st), FALSE);
	m_pMPIFDMARegs = (PREG_MPIFDMA_st)GetVirtualAddr(COLUMBUS_BASE_DMA_MPIF_PA, sizeof(REG_MPIFDMA_st));

	//1. If space for mapping the range is insufficient, return directly
    if ( NULL == m_pMPIFDMARegs )
    {
		RETAILMSG(DBG_MPIF_INIT, (TEXT("MpifInit mpif mapping the range fail!\r\n")));
	}

	//1.init member var
	m_bFlagExitTest = 0;
	m_mpifClk = SET_MPIF_CLOCK_12M;//SET_MPIF_CLOCK_10M;
	m_dwMpifBitMode = SLAVE3_CHANGE_TO_1BIT_MODE;

	//1.malloc buffer
	m_tx = ( U8*) malloc(BUFFER_SIZE);
	m_rx = ( U8*) malloc(BUFFER_SIZE);

	for(i=0;i<BUFFER_SIZE;i++)
	{
		m_tx[i] = 0;
		m_rx[i] = 0;
	}

	m_hydra_put.handler = hydra_handler_pif;
	m_hydra_get.handler = hydra_handler_pif;
	m_hydra_put_2A.handler = hydra_handler_pif;

	DDK_MPIF_Init();
	mstar_multi_pif_init();

	//1.Board Pad Mux Setting
	mstar_multi_pif_reset();

	//mstar_multi_pif_IOControl(hydra_handler_pif, ARGO_POWER_INIT);

	//1.Fill register file
	m_hydra_register.device_id = MPIF1;
	m_hydra_register.slave_id = SLAVE_3;
	m_hydra_register.wait_type = POLLING;
	m_hydra_register.DMA_buffer_size = BUFFER_SIZE;    //TODO: use dynamic alloc

	//1.hydra register,
	err = mstar_multi_pif_register(&m_hydra_register);
	if(MPIF_SUCCESS != err){
		RETAILMSG(DBG_MPIF_INIT, (TEXT("MpifInit mstar_multi_pif_register fail!\r\n")));
		return FALSE;
		}

	 //1.DMA Address for Read/Write
    	hydra_handler_pif = m_hydra_register.handler;
	RETAILMSG(DBG_MPIF_INIT, (TEXT("\r\nMpifInit Register PIF handler NO. %d!\r\n"), hydra_handler_pif));


	mstar_multi_pif_IOControl(hydra_handler_pif, ARGO_POWER_INIT);


    //1.Clock Setting
    mstar_multi_pif_IOControl(hydra_handler_pif, SET_MPIF_CLOCK_12M);

    m_sOption.bCheckSum = 1;  // 1:Enable, 0:Disable the HW checksum.
    m_sOption.bReTX = 0;  // 1:Enable, 0:Disable the Retransmission.
    m_sOption.u8ReTXTimes = 0;  //If Enable Retransmission, the RX times is 0~3.
    m_sOption.u8TurnAroundCycle = 1;
    m_sOption.u8WaitCycle = 1;
    //sOption.eMode = MIU_MODE;   //RIU_MODE & MIU_MODE.
    //sOption.eFastMode = 0;  //0: FAST_MODE_DISABLE, 1: FAST_MODE_ENABLE.
    m_sOption.u8MaxWait = 3;  //0~15, Used for LC3A, LC3B, and LC4A.
    //sOption.u32MIUAddr = MPIF_DMA_MIU_BASE_OFFSET;  //Used for store the data from Hydra.
    //sOption.u32MIUAddr = MIU_4AWriteAddr;
    //sOption.u32MIUReadAddr = MIU_4AReadAddr;
    m_sOption.u8PageSize = 0;  //0~4, Used for LC4A.
    //sOption.u32DMAGuardingTime = ;  //Used for SPI, No use for PIF.

    ConfigPIF(hydra_handler_pif, m_sOption);

	for (i = 0; i < BUFFER_SIZE; i++)
		{
		m_tx[i] = (U8) (i & 0xFF);
		m_pMPIFDMARegs->u8Data[i]=0;
		}

	//1. turn off DAR path
	wval = 0;
	m_hydra_put.pput_data_buffer = &wval;
	m_hydra_put.eChannel = LC1A;
	m_hydra_put.slave_address = 1;
	err = mstar_multi_pif_put_data(&m_hydra_put);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_INIT, (TEXT("[1]MpifInit mstar_multi_pif_put_data err(%x)!\r\n"), err));

	 //1. turn on DAR path
    wval = 1;
    m_hydra_put.pput_data_buffer = & wval;
    m_hydra_put.eChannel = LC1A;
    m_hydra_put.slave_address = 1;
    err = mstar_multi_pif_put_data(&m_hydra_put);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_INIT, (TEXT("[2]MpifInit mstar_multi_pif_put_data err(%x)!\r\n"), err));

	MIUInit();

	//1. turn off DAR path
	wval = 0;
	m_hydra_put.pput_data_buffer = &wval;
	m_hydra_put.eChannel = LC1A;
	m_hydra_put.slave_address = 1;
	err = mstar_multi_pif_put_data(&m_hydra_put);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_INIT, (TEXT("[1]MpifInit mstar_multi_pif_put_data err(%x)!\r\n"), err));

	FUNCTION_LEAVE_MSG();
	return TRUE;
}

void Cmpif::MIUInit()
{
	RETAILMSG(DBG_MPIF_INIT, (TEXT("[1]MIUInit++!\r\n")));

	// dig_top reset
	wreg(2, 0x10, 0x0001);
	wreg(2, 0x10, 0x0001);
	wreg(2, 0x10, 0x0000);
	wreg(2, 0x10, 0x0000);

	// disable WDT
	wreg(2, 0x08, 0xaa55);

	// MPLL setting
	// reg_gc_mpllin_ictrl[14:12] Charge pump current control register
	// 000:2.6uA; 001:5.2uA; 010:7.7uA; 011:10.3uA; 100:13uA; 101:15.5uA; 110:20.6uA; 111:41.2uA
	// reg_gc_mpllin_adclksel[10:8] CLKRO_ADCCLK_HV selection
	// 000/DIV19; 001/DIV19; 010/DIV24; 011/DIV33; 1xx/XTAL
	wreg(1, 0x30, 0x6400);

	// MIU_PLL
	wreg(12, 0x1c, 0x0000);
	wreg(12, 0x10, 0x19d9);
	wreg(12, 0x12, 0x4000);

	// ------------------------------------------------
	// --- ADC clock on
	// ------------------------------------------------
	// reg_adc_sf_bypass[0]
	// reg_adc_ensfi[1], I-channel Level-shifter power down (0: power down)
	// reg_adc_ensfq[2], Q-channel Level-shifter power down (0: power down)
	// wreg 01 0x35 0x0007
	wreg(1, 0x35, 0x0001);

	// ------------------------------------------------
	// --- ADC on
	// ------------------------------------------------
	// reg_adc_eni[0], I-channel ADC power down (0: power down)
	// reg_adc_enq[1], Q-channel ADC power down (0: power down)
	// reg_adc_enref[3:2], ref & bgap power down (00: power down; 01: bandgap preset; 11: active)
	// reg_adc_ac
	// reg_adc_cal
	// 1.2Vpp ADC input
	// wreg 01 0x20 0x303f
	// 2.5Vpp ADC input
	// wreg 01 0x20 0x003f
	wreg(1, 0x20, 0x301f);
	mmp_thread_delay(1);
	wreg(1, 0x20, 0x303f);

	// Turn on all clock
	wreg(0, 0x15, 0x0000);
	wreg(0, 0x0a, 0x0000);
	wreg(0, 0x0d, 0x0000);
	wreg(0, 0x0c, 0x0000);
	wreg(0, 0x10, 0x0000);
	wreg(0, 0x11, 0x0014);
	wreg(0, 0x12, 0x0000);
	wreg(0, 0x13, 0x0014);
	wreg(0, 0x14, 0x0000);
	// internal SRAM
	//wreg(0, 0x15, 0x0400);
	// external SDRAM
	wreg(0, 0x15, 0x0000);
	wreg(0, 0x16, 0x0000);
	wreg(0, 0x17, 0x0000);
	wreg(0, 0x18, 0x0000);
	wreg(0, 0x1c, 0x0000);
	wreg(0, 0x1d, 0x0000);
	wreg(0, 0x1e, 0x0000);
	wreg(0, 0x1f, 0x0000);

	// disable turn off pad
	// [0]Lock pads to input register control; 1: input
	// [1]Enable JTAG signal access through PAD_TS_xx.
	// [2]Enable I2S function
	// [3]Enable SPDIF function
	// [4]Enable CCIR 656 out  function
	// [5]Enable UART debug  function
	// [6]Enable I2S PAD GPIO function
	// [7]Trun off MIU pad
	// [8]Enable SDIO PAD function
	// [9]MPIF(share SDIO) PADfunction
	// [10]Enable BT656(share SDIO) PADfunction
	// [11]Enable SPIF_INT PAD GPIO function
	// [12]Enable SIM-CARD PADfunction
	// [13]Enable Uart debug on I2C pad
	wreg(0, 0x29, 0x0020);
	wreg(0, 0x2a, 0x0030);
	// wreg 00 0x2a 0x0001
	// [0]if agc pad enable
	// [2:1]if agc drive sel
	// [4]if agc_up pad enable
	// [5]if agc_down pad enable
	// disable tslice function
	// [0] clk gate enable
	// [1] rst control enable
	wreg(2, 0x70, 0x0000);
	// [1] aeon share sram enable
	wreg(2, 0x5b, 0x0102);
	wreg(2, 0x5b, 0x0102);
	wreg(2, 0x5b, 0x0102);
	// internal SRAM
	//wreg(2, 0x5b, 0x0002);
	// external SDRAM
	wreg(2, 0x5b, 0x0000);
	mmp_thread_delay(5);
	wreg(12, 0x10, 0x19d9);
	wreg(12, 0x12, 0x4000);
	mmp_thread_delay(5);
	wreg(12, 0x01, 0x0011);
	wreg(12, 0x02, 0x0350);
	wreg(12, 0x03, 0x0040);
	wreg(12, 0x04, 0x44c8);
	wreg(12, 0x05, 0x0263);
	wreg(12, 0x06, 0x4522);
	wreg(12, 0x07, 0x410e);
	wreg(12, 0x0e, 0x1800);
	wreg(12, 0x1b, 0x0260);
	wreg(12, 0x20, 0xc001);
	wreg(12, 0x30, 0xc001);
	wreg(12, 0x23, 0x0000);
	wreg(12, 0x33, 0x0000);
	wreg(12, 0x00, 0x0001);
	mmp_thread_delay(5);
	wreg(12, 0x72, 0x0100);
	wreg(12, 0x73, 0x0000);
	wreg(12, 0x74, 0x55aa);
	// wreg 12 0x70 0x0001
	mmp_thread_delay(5);
	// check 0x70 == 0x8001

	RETAILMSG(DBG_MPIF_INIT, (TEXT("[1]MIUInit--!\r\n")));

}
/*
void AndyMpifInit(void)
{
	UINT32 wval, rval = 0xAAAA;
    UINT32 poll = 0;
    UINT32 k = 0;
    UINT32 times = 0;
    UINT32 len = 0;
    UINT8 *tx;
    UINT8 *rx;

    UINT32 u32TimeElapsed, u32NewTime,u32OldTime;
    UINT32 i = 0;
    UINT32 j = 0;

    MPIF_OPTIONS_st sOption;
    pmstar_multi_pif_register_st hydra_register;
    MPIF_ERROR_CODE_et err = MPIF_UNKNOWN_ERROR;
    INT32 length_3A = 0;        //unit in 16 bytes
    UINT32 addr;

    PHYSICAL_ADDRESS MPIFDMAPhysicalBase = {PIONEER_BASE_DMA_MPIF_PA, 0};
    PHYSICAL_ADDRESS CHIPTOPPhysicalBase = {PIONEER_BASE_REG_CHIPTOP_PA, 0};


    pmstar_multi_pif_get_st    hydra_get;
    pmstar_multi_pif_put_st hydra_put,hydra_put_2A;

    _tprintf(_T("Hello World! Welcom to Carina MPIF Test!\n"));

   mmp_mutex_init(&reg_mutex);


    u32TimeElapsed = 0;
    u32NewTime = 0;
    u32OldTime = 0;


    //MPIFDMAPhysicalBase.QuadPart= COLUMBUS_BASE_REG_CHIPTOP_PA;
    MPIFDMAPhysicalBase.QuadPart= PIONEER_BASE_DMA_MPIF_PA;
    g_pMPIFDMARegs = (PREG_MPIFDMA_st)MmMapIoSpace(MPIFDMAPhysicalBase, sizeof(REG_MPIFDMA_st), FALSE);


      // If space for mapping the range is insufficient, return directly

    if ( NULL == g_pMPIFDMARegs )
    {
        _tprintf(_T("[Carina] Unable to map DMA space \r\n"));
    }

   	mstar_multi_pif_IOControl(hydra_handler_pif, ARGO_POWER_INIT);

    tx = ( UINT8 *) malloc(BUFFER_SIZE);
    rx = ( UINT8 *) malloc(BUFFER_SIZE);

    for(i=0;i<BUFFER_SIZE;i++)
    {
        tx[i] = 0;
        rx[i] = 0;
    }

    hydra_put.handler = hydra_handler_pif;
    hydra_get.handler = hydra_handler_pif;
    hydra_put_2A.handler = hydra_handler_pif;

    mstar_multi_pif_init();

    //Board Pad Mux Setting
    mstar_multi_pif_reset();

    //Fill register file
    hydra_register.device_id = MPIF1;
    hydra_register.slave_id = SLAVE_3;
    hydra_register.wait_type = POLLING;
    hydra_register.DMA_buffer_size = BUFFER_SIZE;    //TODO: use dynamic alloc
    //hydra register

    err = mstar_multi_pif_register(&hydra_register);

    if(MPIF_SUCCESS != err)
    {
        _tprintf(_T("Register hydra handler fail!!\n"));
        while(1);
    }



    //DMA Address for Read/Write
    hydra_handler_pif = hydra_register.handler;

    _tprintf(_T("Register PIF handler NO. %d\n"),hydra_handler_pif);

    //Clock Setting
    mstar_multi_pif_IOControl(hydra_handler_pif, SET_MPIF_CLOCK_27M);
    //mstar_multi_pif_IOControl(hydra_handler_pif, SET_MPIF_CLOCK_36M);
    //mstar_multi_pif_IOControl(hydra_handler_pif, SET_MPIF_CLOCK_54M);
    //mstar_multi_pif_IOControl(hydra_handler_pif, SET_MPIF_CLOCK_62M);

    //MPIFIOControl(ENHANCE_DRIVING_ALL);  //For debug.

    sOption.bCheckSum = 1;  // 1:Enable, 0:Disable the HW checksum.
    sOption.bReTX = 0;  // 1:Enable, 0:Disable the Retransmission.
    sOption.u8ReTXTimes = 0;  //If Enable Retransmission, the RX times is 0~3.
    sOption.u8TurnAroundCycle = 1;
    sOption.u8WaitCycle = 1;
    //sOption.eMode = MIU_MODE;   //RIU_MODE & MIU_MODE.
    //sOption.eFastMode = 0;  //0: FAST_MODE_DISABLE, 1: FAST_MODE_ENABLE.
    sOption.u8MaxWait = 3;  //0~15, Used for LC3A, LC3B, and LC4A.
    //sOption.u32MIUAddr = MPIF_DMA_MIU_BASE_OFFSET;  //Used for store the data from Hydra.
    //sOption.u32MIUAddr = MIU_4AWriteAddr;
    //sOption.u32MIUReadAddr = MIU_4AReadAddr;
    sOption.u8PageSize = 0;  //0~4, Used for LC4A.
    //sOption.u32DMAGuardingTime = ;  //Used for SPI, No use for PIF.

    ConfigPIF(hydra_handler_pif, sOption);

    _tprintf(_T("chun1\n"));
    len = BUFFER_SIZE;
    for (i = 0; i < len; i++)
    {
        tx[i] = (U8) (i & 0xFF);
        g_pMPIFDMARegs->u8Data[i]=0;
    }
    _tprintf(_T("chun2\n"));

        // turn off DAR path

        wval = 0;
        hydra_put.pput_data_buffer = &wval;
        hydra_put.eChannel = LC1A;
        hydra_put.slave_address = 1;
        err = mstar_multi_pif_put_data(&hydra_put);

        if (err != MPIF_SUCCESS)
            _tprintf(_T(" err = %d\n"),err);

#if 1 //Andy

        wval = 0x171F;
      //Andy wval = 0x1017;
                hydra_put.pput_data_buffer = &wval;
                hydra_put.eChannel = LC2A;
                hydra_put.slave_address = 0x72;
                err = mstar_multi_pif_put_data(&hydra_put);
                //Andy if (err != MPIF_SUCCESS)
             while (err != MPIF_SUCCESS) //Andy
                {
                    _tprintf(_T(" err = %d\n"),err);
                    _tprintf(_T(" !!\n!!\n!!\n!!\n!!\n!!\n1======== LC2A test fail!! Addr = 0x%x, (0x%x, 0x%x)========\r\n"), hydra_put.slave_address, wval, rval);
                    //Andy while(1);
                //Andy up
                hydra_put.pput_data_buffer = &wval;
                hydra_put.eChannel = LC2A;
                hydra_put.slave_address = 0x72;
                err = mstar_multi_pif_put_data(&hydra_put);
                //Andy down
                }

                mstar_multi_pif_IOControl(hydra_handler_pif, SET_MPIF_CLOCK_54M);
                rval = 0x5A5A;
                hydra_get.pget_data_buffer = &rval;
                hydra_get.eChannel = LC2A;
                hydra_get.slave_address = 0x72;
                err = mstar_multi_pif_get_data(&hydra_get);
                //Andy if (err != MPIF_SUCCESS)
             while (err != MPIF_SUCCESS) //Andy
                {
                    _tprintf(_T(" err = %d\n"),err);
                //Andy up
                rval = 0x5A5A;
                hydra_get.pget_data_buffer = &rval;
                hydra_get.eChannel = LC2A;
                hydra_get.slave_address = 0x72;
                err = mstar_multi_pif_get_data(&hydra_get);
                //Andy down
                    //Andy while(1);
                }

                _tprintf(_T(" 2======== LC2A  Addr = 0x%x, (0x%x, 0x%x)========\r\n"), hydra_put.slave_address, wval, rval);
#endif

//==============================================================================

    //MIU init

    // turn on DAR path
    wval = 1;
    hydra_put.pput_data_buffer = & wval;
    hydra_put.eChannel = LC1A;
    hydra_put.slave_address = 1;
    err = mstar_multi_pif_put_data(&hydra_put);


    //init Argo MIU
//    MIUInit();
    //wreg(0, 0x0c, 0x000c); //init miu

    // turn off DAR path
    wval = 0;
    hydra_put.pput_data_buffer = & wval;
    hydra_put.eChannel = LC1A;
    hydra_put.slave_address = 1;
    err = mstar_multi_pif_put_data(&hydra_put);

    if (err != MPIF_SUCCESS)
        _tprintf(_T(" err = %d\n"),err);


#if TWO_BIT_MODE
      _tprintf(_T("2 BTIS mode\r\n") );
    mstar_multi_pif_IOControl( hydra_handler_pif, SLAVE3_CHANGE_TO_2BIT_MODE);
#endif
#if FOUR_BIT_MODE
        _tprintf(_T("4 BTIS mode\r\n") );
        mstar_multi_pif_IOControl( hydra_handler_pif, SLAVE3_CHANGE_TO_4BIT_MODE);
#endif

    _tprintf(_T("chun3\n"));
    mstar_multi_pif_IOControl(hydra_handler_pif, SLAVE_CHANGE_TO_3A_MIU_MODE);     //For LC3A.
    //mstar_multi_pif_IOControl(hydra_handler_pif, SLAVE_CHANGE_TO_3B_MIU_MODE);    //For LC3B.

    _tprintf(_T("chun4\n"));

   //SET_SPIF_REG(0x72, 0x171F); //Andy
   ONOFF_DAR_PATH(1); //Andy
   printf("pDMAAddr Init..\n");
   pDMAAddrInit();
}
*/
BOOL Cmpif::MpifTestInit(void)
{
	DWORD i,wval;
	MPIF_ERROR_CODE_et err = MPIF_UNKNOWN_ERROR;

	FUNCTION_ENTER_MSG();

	mstar_multi_pif_IOControl(hydra_handler_pif, ARGO_POWER_INIT);

	//1.init member var
	m_bFlagExitTest = 0;

	for(i=0;i<BUFFER_SIZE;i++)
	{
		m_tx[i] = 0;
		m_rx[i] = 0;
	}

	m_hydra_put.handler = hydra_handler_pif;
	m_hydra_get.handler = hydra_handler_pif;
	m_hydra_put_2A.handler = hydra_handler_pif;

	mstar_multi_pif_init();

	//1.Board Pad Mux Setting
	mstar_multi_pif_reset();

	//1.Fill register file
	m_hydra_register.device_id = MPIF1;
	m_hydra_register.slave_id = SLAVE_3;
	m_hydra_register.wait_type = POLLING;
	m_hydra_register.DMA_buffer_size = BUFFER_SIZE;    //TODO: use dynamic alloc

	//1//1.hydra register,Unify mpif文件中只支持1个访问者，
	//1MpifInit中已经注册了，这里就不要再注册了。
	/*err = mstar_multi_pif_register(&m_hydra_register);
	if(MPIF_SUCCESS != err){
		RETAILMSG(DBG_MPIF_INIT, (TEXT("MpifInit mstar_multi_pif_register fail!\r\n")));
		return FALSE;
		}*/

	 //1.DMA Address for Read/Write
    hydra_handler_pif = m_hydra_register.handler;
	RETAILMSG(DBG_MPIF_INIT, (TEXT("\r\nMpifTestInit Register PIF handler NO. %d!\r\n"), hydra_handler_pif));

    //1.Clock Setting
    mstar_multi_pif_IOControl(hydra_handler_pif, m_mpifClk);

    m_sOption.bCheckSum = 1;  // 1:Enable, 0:Disable the HW checksum.
    m_sOption.bReTX = 0;  // 1:Enable, 0:Disable the Retransmission.
    m_sOption.u8ReTXTimes = 0;  //If Enable Retransmission, the RX times is 0~3.
    m_sOption.u8TurnAroundCycle = 1;
    m_sOption.u8WaitCycle = 1;
    //sOption.eMode = MIU_MODE;   //RIU_MODE & MIU_MODE.
    //sOption.eFastMode = 0;  //0: FAST_MODE_DISABLE, 1: FAST_MODE_ENABLE.
    m_sOption.u8MaxWait = 3;  //0~15, Used for LC3A, LC3B, and LC4A.
    //sOption.u32MIUAddr = MPIF_DMA_MIU_BASE_OFFSET;  //Used for store the data from Hydra.
    //sOption.u32MIUAddr = MIU_4AWriteAddr;
    //sOption.u32MIUReadAddr = MIU_4AReadAddr;
    m_sOption.u8PageSize = 0;  //0~4, Used for LC4A.
    //sOption.u32DMAGuardingTime = ;  //Used for SPI, No use for PIF.

    ConfigPIF(hydra_handler_pif, m_sOption);

	for (i = 0; i < BUFFER_SIZE; i++)
		{
		m_tx[i] = (U8) (i & 0xFF);
		m_pMPIFDMARegs->u8Data[i]=0;
		}

	//1. turn off DAR path
	wval = 0;
	m_hydra_put.pput_data_buffer = &wval;
	m_hydra_put.eChannel = LC1A;
	m_hydra_put.slave_address = 1;
	err = mstar_multi_pif_put_data(&m_hydra_put);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_INIT, (TEXT("[1]MpifTestInit mstar_multi_pif_put_data err(%x)!\r\n"), err));

	 //1. turn on DAR path
    wval = 1;
    m_hydra_put.pput_data_buffer = & wval;
    m_hydra_put.eChannel = LC1A;
    m_hydra_put.slave_address = 1;
    err = mstar_multi_pif_put_data(&m_hydra_put);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_INIT, (TEXT("[2]MpifTestInit mstar_multi_pif_put_data err(%x)!\r\n"), err));

	MIUInit();

	//1. turn off DAR path
	wval = 0;
	m_hydra_put.pput_data_buffer = &wval;
	m_hydra_put.eChannel = LC1A;
	m_hydra_put.slave_address = 1;
	err = mstar_multi_pif_put_data(&m_hydra_put);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_INIT, (TEXT("[1]MpifTestInit mstar_multi_pif_put_data err(%x)!\r\n"), err));

	//1.change to xbit mode
	if(SLAVE3_CHANGE_TO_2BIT_MODE == m_dwMpifBitMode)
		{
			RETAILMSG(DBG_MPIF_INIT, (TEXT("[1]2 BTIS mode!\r\n")));
			mstar_multi_pif_IOControl( hydra_handler_pif, m_dwMpifBitMode);
		}
		if(SLAVE3_CHANGE_TO_4BIT_MODE == m_dwMpifBitMode)
		{
			RETAILMSG(DBG_MPIF_INIT, (TEXT("[1]4 BTIS mode!\r\n")));
			mstar_multi_pif_IOControl( hydra_handler_pif, m_dwMpifBitMode);
		}

	//1.CHANGE_TO_3A_MIU_MODE
	RETAILMSG(DBG_MPIF_INIT, (TEXT("[1]4 CHANGE_TO_3A_MIU_MODE!\r\n")));
    mstar_multi_pif_IOControl(hydra_handler_pif, SLAVE_CHANGE_TO_3A_MIU_MODE);     //For LC3A.
    //mstar_multi_pif_IOControl(hydra_handler_pif, SLAVE_CHANGE_TO_3B_MIU_MODE);    //For LC3B.

	FUNCTION_LEAVE_MSG();

	return TRUE;
}

void Cmpif::MpifStop()
{
	RETAILMSG(DBG_MPIF_INIT, (TEXT("[*]MpifStop!\r\n")));

	if(NULL != m_tx){
		RETAILMSG(DBG_MPIF_INIT, (TEXT("[*]free( m_tx)!\r\n")));
		free( m_tx);
		}
	if(NULL != m_rx){
		RETAILMSG(DBG_MPIF_INIT, (TEXT("[*]free( m_rx)!\r\n")));
		free( m_rx);
		}
	if (NULL != m_pMPIFDMARegs)
		{
		//1TODO:
		//MmUnmapIoSpace((PVOID)m_pMPIFDMARegs, sizeof(PREG_MPIFDMA_st));
		UnmapVirtualAddr((PVOID)m_pMPIFDMARegs, sizeof(PREG_MPIFDMA_st));
		}
}

void Cmpif::Carina_1A_Test()
{
DWORD	rval, wval, i;
MPIF_ERROR_CODE_et err = MPIF_UNKNOWN_ERROR;

	MpifTestInit();

	RETAILMSG(DBG_MPIF_1A, (TEXT("[1]1A test!\r\n")));
	wval = 1;
	i=0;
	while(!m_bFlagExitTest)
	{
		if (i > 0xFFFF0)
		i = 0;
		else
		i++;

		//1. turn on DAR path
		wval = (wval + 1)%2;
		m_hydra_put.pput_data_buffer = &wval;
		m_hydra_put.eChannel = LC1A;
		m_hydra_put.slave_address = 1;
		err = mstar_multi_pif_put_data(&m_hydra_put);
		if (err != MPIF_SUCCESS)
			RETAILMSG(DBG_MPIF_1A, (TEXT("1 err = %d\r\n"),err));

		rval = 0xff;
		m_hydra_get.pget_data_buffer = &rval;
		m_hydra_get.eChannel = LC1A;
		m_hydra_get.slave_address = 1;
		err = mstar_multi_pif_get_data(&m_hydra_get);
		if (err != MPIF_SUCCESS)
			RETAILMSG(DBG_MPIF_1A, (TEXT("2 err = %d\r\n"),err));

		if (wval != rval)
			{
			RETAILMSG(DBG_MPIF_1A, (TEXT("[ERROR]wval=0x%x is rval=0x%x\r\n"),wval, rval));
			break;
			}

		if(0 == (i % 0x1000)){
			Sleep(5);
			RETAILMSG(DBG_MPIF_1A, (TEXT(" ======== LC1A test Pass 0x%x times!!========\r\n"), i));
			}
	}

}

void Cmpif::Carina_2A_Test()
{
DWORD	rval, wval, i;
U16	addr;
MPIF_ERROR_CODE_et err = MPIF_UNKNOWN_ERROR;

	MpifTestInit();

	RETAILMSG(DBG_MPIF_2A, (TEXT("[1]2A test!\r\n")));

	//1 turn on DAR path
	wval = 0;
	m_hydra_put.pput_data_buffer = & wval;
	m_hydra_put.eChannel = LC1A;
	m_hydra_put.slave_address = 1;
	err = mstar_multi_pif_put_data(&m_hydra_put);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_2A, (TEXT("1 err = %d\r\n"),err));

    rval = 0;
    m_hydra_get.pget_data_buffer = & rval;
    m_hydra_get.eChannel = LC1A;
    m_hydra_get.slave_address = 1;
    err = mstar_multi_pif_get_data(&m_hydra_get);
    if (err != MPIF_SUCCESS)
       RETAILMSG(DBG_MPIF_2A, (TEXT("2 err = %d\r\n"),err));

   RETAILMSG(DBG_MPIF_2A, (_T("DAR PATH is 0x%x now!\r\n"), rval));

    while(!m_bFlagExitTest)
	{
		for (addr = 0x10; addr < 0x18; addr+=1)
		{
			for (i = 0x0; i < 0x10000; i++)
			{
				//1-----------2A test
				wval = i;
				m_hydra_put.pput_data_buffer = & wval;
				m_hydra_put.eChannel = LC2A;
				m_hydra_put.slave_address = addr;
				err = mstar_multi_pif_put_data(&m_hydra_put);
				if (err != MPIF_SUCCESS){
					RETAILMSG(DBG_MPIF_2A, (_T(" err = %d\r\n"),err));
					RETAILMSG(DBG_MPIF_2A, (_T(" ======== LC2A test fail!! Addr = 0x%x, (0x%x, 0x%x)========\r\n"), addr, wval, rval));
					return;//while(1);
				}

				rval = 0x5A5A;
				m_hydra_get.pget_data_buffer = &rval;
				m_hydra_get.eChannel = LC2A;
				m_hydra_get.slave_address = addr;
				err = mstar_multi_pif_get_data(&m_hydra_get);
				if (err != MPIF_SUCCESS){
					RETAILMSG(DBG_MPIF_2A, (_T(" err = %d\r\n"),err));
					return;//while(1);
				}

				if (wval != rval){
					RETAILMSG(DBG_MPIF_2A, (_T(" ======== LC2A test fail!! Addr = 0x%x, (0x%x, 0x%x)========\r\n"), addr, wval, rval));
					return;//while(1);
				}

				//2.--------------2B test
				wval = i;
				m_hydra_put.pput_data_buffer = & wval;
				m_hydra_put.eChannel = LC2B;
				m_hydra_put.slave_address = addr + 0x10;
				err = mstar_multi_pif_put_data(&m_hydra_put);
				if (err != MPIF_SUCCESS){
					RETAILMSG(DBG_MPIF_2A, (_T(" err = %d\r\n"),err));
					RETAILMSG(DBG_MPIF_2A, (_T(" ======== LC2B test fail!! Addr = 0x%x, (0x%x, 0x%x)========\r\n"), addr, wval, rval));
					return;//while(1);
				}

				rval = 0x5A5A;
				m_hydra_get.pget_data_buffer = &rval;
				m_hydra_get.eChannel = LC2B;
				m_hydra_get.slave_address = addr + 0x10;
				err = mstar_multi_pif_get_data(&m_hydra_get);
				if (err != MPIF_SUCCESS){
					RETAILMSG(DBG_MPIF_2A, (_T(" err = %d\r\n"),err));
					return;//while(1);
				}

				if (wval != rval){
					RETAILMSG(DBG_MPIF_2A, (_T(" ======== LC2B test fail!! Addr = 0x%x, (0x%x, 0x%x)========\r\n"), addr, wval, rval));
					return;//while(1);
				}

				if(0 == i % 0x4000){
					RETAILMSG(DBG_MPIF_2A, (_T(" ======== LC2A/B test Pass %x times!!========\r\n"), i));
					Sleep(5);
				}

				if(m_bFlagExitTest)
					break;
			}

			if(m_bFlagExitTest)
				break;

		}//for (i = 0; i < 100000; i++)
	}
}

void Cmpif::Carina2ATestXIU()
{
DWORD	rval, wval, i, k,poll;
U16	addr;
MPIF_ERROR_CODE_et err = MPIF_UNKNOWN_ERROR;

	MpifTestInit();

	RETAILMSG(DBG_MPIF_2A, (TEXT("2A XIU test\r\n")));

	//1. turn on DAR path
	wval = 1;
	m_hydra_put.pput_data_buffer = & wval;
	m_hydra_put.eChannel = LC1A;
	m_hydra_put.slave_address = 1;
	err = mstar_multi_pif_put_data(&m_hydra_put);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_2A, (TEXT("1 err = %d\r\n"),err));

	rval = 0;
	m_hydra_get.pget_data_buffer = & rval;
	m_hydra_get.eChannel = LC1A;
	m_hydra_get.slave_address = 1;
	err = mstar_multi_pif_get_data(&m_hydra_get);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_2A, (TEXT("2 err = %d\r\n"),err));

	RETAILMSG(DBG_MPIF_2A, (TEXT("DAR PATH is 0x%x now!\r\n"),rval));

	//1. Enable Polling
	wval = 1;
	m_hydra_put.pput_data_buffer = & wval;
	m_hydra_put.eChannel = LC1A;
	m_hydra_put.slave_address = 6;
	err = mstar_multi_pif_put_data(&m_hydra_put);
	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_2A, (TEXT("3 err = %d\r\n"),err));

	//1. Enable Polling
	i = 0;
	while(!m_bFlagExitTest)
		{
		if (i > 0xfffffff0)
			i =0 ;
		else
			i++;

		for (addr = 0x4980; addr < 0x49B0; addr+=1)
			{
			wval = (i & 0xFFFF);
			m_hydra_put.pput_data_buffer = & wval;
			m_hydra_put.eChannel = LC2A;
			m_hydra_put.slave_address = addr;
			err = mstar_multi_pif_put_data(&m_hydra_put);
			if (err != MPIF_SUCCESS){
				RETAILMSG(DBG_MPIF_2A, (TEXT("4 err = %d\r\n"),err));
				break;
				}

			//1. Polling
			for(k=0;k<0x100;k++)
				{
				rval = 0;
				m_hydra_get.pget_data_buffer = & rval;
				m_hydra_get.eChannel = LC1A;
				m_hydra_get.slave_address = 7;
				err = mstar_multi_pif_get_data(&m_hydra_get);
				if (err != MPIF_SUCCESS)
					RETAILMSG(DBG_MPIF_2A, (TEXT("5 err = %d\r\n"),err));

				//1.Polling
				poll = (rval>>1)&0x1;
				if(0x1 == poll )
					{
					rval = 0;
					m_hydra_get.pget_data_buffer = &rval;
					m_hydra_get.eChannel = LC2A;
					m_hydra_get.slave_address = addr;
					err = mstar_multi_pif_get_data(&m_hydra_get);
					if (err != MPIF_SUCCESS){
						RETAILMSG(DBG_MPIF_2A, (TEXT("6 err = %d\r\n"),err));
						break;
						}
					break;
					}
				else
					RETAILMSG(DBG_MPIF_2A, (TEXT(" !!! Polling LC2A status is Busy(%x) !!!\r\n"),i));
				}
			if(0x100 == k){
				RETAILMSG(DBG_MPIF_2A, (TEXT("!!!!!!!!!!!!!!!!Polling err............\r\n")));
				return;
				}

			if (wval != rval) {
				RETAILMSG(DBG_MPIF_2A, (TEXT(" ======== LC2A test fail!! Addr = 0x%x, (0x%x, 0x%x)========\r\n"), addr, wval, rval));
				return;
				}
			}
		if(0 == i % 0x1000){
			RETAILMSG(DBG_MPIF_2A, (TEXT(" ======== LC2A  XIU test Pass %x times!!========\r\n"),i));
			Sleep(5);
			}
		}
	//m_bFlagExitTest = 0;
}

void Cmpif::Carina2ATestF0F0()
{

}

void Cmpif::Carina3ATest()
{
DWORD	rval, wval, i, k, len, times;
U32	addr;
MPIF_ERROR_CODE_et err = MPIF_UNKNOWN_ERROR;

	MpifTestInit();

	RETAILMSG(DBG_MPIF_3A, (TEXT("Carina3ATest\r\n")));
	RETAILMSG(DBG_MPIF_3A,(_T("3A init MIU test, BUFFER_SIZE=%d KB\r\n"), (BUFFER_SIZE>>10)));

	//1. turn off DAR path
	wval = 0;
	m_hydra_put.pput_data_buffer = & wval;
	m_hydra_put.eChannel = LC1A;
	m_hydra_put.slave_address = 1;
	err = mstar_multi_pif_put_data(&m_hydra_put);

	if (err != MPIF_SUCCESS)
	RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));

	rval = 9;
	m_hydra_get.pget_data_buffer = & rval;
	m_hydra_get.eChannel = LC1A;
	m_hydra_get.slave_address = 1;
	err = mstar_multi_pif_get_data(&m_hydra_get);

	if (err != MPIF_SUCCESS)
	RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));

	RETAILMSG(DBG_MPIF_3A,(_T("DAR PATH is 0x%x now!\r\n"), rval));

	//1 for 3A MIU mode
	//SWAP slave use MIU mode
	wval = 0x04;
	m_hydra_put_2A.pput_data_buffer = & wval;
	m_hydra_put_2A.eChannel = LC2A;
	m_hydra_put_2A.slave_address = 0x30;     //SPIF_R_SPIF_3A_MADR_L_ADDR
	err = mstar_multi_pif_put_data(&m_hydra_put_2A);

	rval = 0x5A5A;
	m_hydra_get.pget_data_buffer = &rval;
	m_hydra_get.eChannel = LC2A;
	m_hydra_get.slave_address = 0x30;
	err = mstar_multi_pif_get_data(&m_hydra_get);

	if (err != MPIF_SUCCESS){
		RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));
		return;
		//while(1);
	}
	RETAILMSG(DBG_MPIF_3A,(_T(" ======== LC2A value 0x30, (0x%x, )========\r\n"), rval));

	//1for 3B MIU mode
	//SWAP slave use MIU mode
	wval = 0x04;
	m_hydra_put_2A.pput_data_buffer = & wval;
	m_hydra_put_2A.eChannel = LC2A;
	m_hydra_put_2A.slave_address = 0x40;     //SPIF_R_SPIF_3A_MADR_L_ADDR
	err = mstar_multi_pif_put_data(&m_hydra_put_2A);

	rval = 0x5A5A;
	m_hydra_get.pget_data_buffer = &rval;
	m_hydra_get.eChannel = LC2A;
	m_hydra_get.slave_address = 0x40;
	err = mstar_multi_pif_get_data(&m_hydra_get);

	if (err != MPIF_SUCCESS){
		RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));
		return;
		//while(1);
	}
	RETAILMSG(DBG_MPIF_3A,(_T(" ======== LC2A value 0x40, (0x%x, )========\r\n"), rval));

	//1len = 16;  //16 Bytes.
	len = BUFFER_SIZE_16;//BUFFER_SIZE;
	k = 0;

	for(times = 0; times <0x10000000; times++ )
	{
		if(m_bFlagExitTest)
			break;

		for (addr = 0; addr < MEMSIZE; addr += len)
		{
			//1-------3B test
			//1 init data
		#ifdef DMA_BY_USER
			for (i = 0; i < len; i++)
				m_pMPIFDMARegs->u8Data[i]= (U8) ((addr + i) & 0xFF);
		#else
			for (i = 0; i < len; i++)
				m_tx[i] = (U8) ((addr + i) & 0xFF);
		#endif

			m_hydra_put_2A.eChannel = LC2A;
			m_hydra_put_2A.pput_data_buffer = & wval;

			//1Fill in Carina MIU Address. Config SPIF register, bank 9, 0x33 & 0x34.
			wval = ((addr)>>3) & 0xffff;
			m_hydra_put_2A.slave_address = 0x43;     //SPIF_R_SPIF_3B_MADR_L_ADDR
			err = mstar_multi_pif_put_data(&m_hydra_put_2A);

			wval = ((addr)>>3) >> 16;
			m_hydra_put_2A.slave_address = 0x44;     //SPIF_R_SPIF_3B_MADR_H_ADDR
			err = mstar_multi_pif_put_data(&m_hydra_put_2A);

			//1Fill in Carina the Packet length. Config SPIF register, bank 9, 0x32. In reg h32, 0x1 = 16 Bytes.
			wval = (len>>4);
			m_hydra_put_2A.slave_address = 0x42;     //SPIF_R_SPIF_3B_MADR_H_ADDR
			err = mstar_multi_pif_put_data(&m_hydra_put_2A);

			//1 Write data to Carina.
			m_hydra_put.mode = MIU_MODE;
			m_hydra_put.put_length = (len>>4);  // one packet is 16 byte
		#ifdef DMA_BY_USER
			m_hydra_put.pput_data_buffer = NULL;
		#else
			m_hydra_put.pput_data_buffer = m_tx;
		#endif
			m_hydra_put.eChannel = LC3B;
			err = mstar_multi_pif_put_data(&m_hydra_put);

#if 0          //clear data
#ifdef DMA_BY_USER
			for (i = 0; i < len; i++)
			{
			m_pMPIFDMARegs->u8Data[i]=0;
			}
#else
			for (i = 0; i < len; i++)
			{
			m_rx[i] = 0;
			RETAILMSG(DBG_MPIF_3A,(_T("rx[%d] =%u \r\n"), i, rx[i]));
			}
#endif
			for (i = 0; i < 0x10; i++)
			{
			RETAILMSG(DBG_MPIF_3A,(_T("DMA[%d] =%u \r\n"), i, g_pMPIFDMARegs->u8Data[i]));
			}
#endif

			//1 read back
			m_hydra_get.mode = MIU_MODE;
			m_hydra_get.get_length = (len>>4);  // one packet is 16 byte
		#ifdef DMA_BY_USER
			m_hydra_get.pget_data_buffer = NULL;
		#else
			m_hydra_get.pget_data_buffer = m_rx;
		#endif
			m_hydra_get.eChannel = LC3B;
			err = mstar_multi_pif_get_data(&m_hydra_get);

			for (i  = 0; i < len; i++)
			{

			#ifdef DMA_BY_USER
				if( m_pMPIFDMARegs->u8Data[i] != ( ( addr + i ) & 0xff ) )
			#else
				if ( m_rx[i] != (U8)( ( addr + i ) & 0xff ) )
			#endif
				{
				#ifdef DMA_BY_USER
					RETAILMSG(DBG_MPIF_3A,(_T("3B Read error: addr = 0x%x, correct value = 0x%02x, wrong value = 0x%02x\r\n"), addr+i, i, g_pMPIFDMARegs->u8Data[i]));
				#else
					RETAILMSG(DBG_MPIF_3A,(_T("3B Read error: addr = 0x%x, correct value = 0x%02x, wrong value = 0x%02x\r\n"), addr+i, m_tx[i], m_rx[i]));
				#endif
					RETAILMSG(DBG_MPIF_3A,(_T("3B Current Length = %d == Set SPIF ADDR 0x%x\r\n"),len, addr));
					k ++;
				}
			}
			//_tprintf(_T(" K is %x now.\n"), k);
			if (0 != k) /* error count is not zero, break */
			break;
		}

		RETAILMSG(DBG_MPIF_3A,(_T(" LC3A/3B_Test times are %x now.\r\n"), times));
		if (0 != k) /* error count is not zero, break */
			break;

		Sleep(5);
	}
}

void Cmpif::Carina3ATestRIU()
{
DWORD	rval, wval, i, k, len, times;
U32	addr;
MPIF_ERROR_CODE_et err = MPIF_UNKNOWN_ERROR;

	MpifTestInit();

	RETAILMSG(DBG_MPIF_3A, (TEXT("Carina 3A RIU test\r\n")));

	//1 turn off DAR path
	wval = 0;
	m_hydra_put.pput_data_buffer = & wval;
	m_hydra_put.eChannel = LC1A;
	m_hydra_put.slave_address = 1;
	err = mstar_multi_pif_put_data(&m_hydra_put);

	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));

	rval = 9;
	m_hydra_get.pget_data_buffer = & rval;
	m_hydra_get.eChannel = LC1A;
	m_hydra_get.slave_address = 1;
	err = mstar_multi_pif_get_data(&m_hydra_get);

	if (err != MPIF_SUCCESS)
		RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));

	RETAILMSG(DBG_MPIF_3A,(_T("DAR PATH is 0x%x now!\r\n"), rval));

	wval = 0;
	m_hydra_put.pput_data_buffer = &wval;
	m_hydra_put.eChannel = LC2A;
	m_hydra_put.slave_address = 0x30;
	err = mstar_multi_pif_put_data(&m_hydra_put);

	if (err != MPIF_SUCCESS){
		RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));
		return;//while(1);
	}

	rval = 0x5A5A;
	m_hydra_get.pget_data_buffer = &rval;
	m_hydra_get.eChannel = LC2A;
	m_hydra_get.slave_address = 0x30;
	err = mstar_multi_pif_get_data(&m_hydra_get);

	if (err != MPIF_SUCCESS){
		RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));
		return;//while(1);
	}

	RETAILMSG(DBG_MPIF_3A,(_T(" ======== LC2A value 0x30, (0x%x )========\r\n"), rval));

	len = 16;
	k = 0;
	addr = 0;
	for(times = 0; times <0x1000; times++ ){

		if(m_bFlagExitTest)
			break;

		for (i = 0; i < len; i++)
			m_tx[i] = (U8) ((addr + i) & 0xFF);

		//1 Write data to Carina.
		m_hydra_put.mode = RIU_MODE;
		m_hydra_put.put_length = (len>>4);  // one packet is 16 byte
		m_hydra_put.pput_data_buffer = m_tx;
		m_hydra_put.eChannel = LC3A;
		err = mstar_multi_pif_put_data(&m_hydra_put);

		for (i = 0; i < len; i++)
			m_rx[i] = 0xAA;

		//1 read back
		m_hydra_get.mode = RIU_MODE;
		m_hydra_get.get_length = (len>>4);  // one packet is 16 byte
		m_hydra_get.pget_data_buffer = m_rx;
		m_hydra_get.eChannel = LC3A;
		err = mstar_multi_pif_get_data(&m_hydra_get);

		for (i  = 0; i < len; i++)
		{
			if ( m_rx[i] != (U8)( ( addr + i ) & 0xff ) ){
				RETAILMSG(DBG_MPIF_3A,(_T("Read error: addr = 0x%x, correct value = 0x%02x, wrong value = 0x%02x\r\n"), addr+i, m_tx[i], m_rx[i]));
				RETAILMSG(DBG_MPIF_3A,(_T(" Current Length = %d == Set SPIF ADDR 0x%x\r\n"),len, addr));
				k ++;
			}
		}

		if(0 == times % 100)
			RETAILMSG(DBG_MPIF_3A,(_T(" =======pass,LC3A_Test times are %x now.=======\r\n"), times));

		if (0 != k)
			break;

		Sleep(5);
	}
}

void Cmpif::Carina3ATestMIU()
{
DWORD	rval, wval, i, k, len, times;
U32	addr;
MPIF_ERROR_CODE_et err = MPIF_UNKNOWN_ERROR;

	MpifTestInit();

    //1 turn off DAR path
    wval = 0;
    m_hydra_put.pput_data_buffer = & wval;
    m_hydra_put.eChannel = LC1A;
    m_hydra_put.slave_address = 1;
    err = mstar_multi_pif_put_data(&m_hydra_put);

    if (err != MPIF_SUCCESS)
        RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));

    rval = 9;
    m_hydra_get.pget_data_buffer = & rval;
    m_hydra_get.eChannel = LC1A;
    m_hydra_get.slave_address = 1;
    err = mstar_multi_pif_get_data(&m_hydra_get);

    if (err != MPIF_SUCCESS)
        RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));

    RETAILMSG(DBG_MPIF_3A,(_T("DAR PATH is 0x%x now!\r\n"), rval));

    wval = 4;   //set Slave to MIU mode
    m_hydra_put.pput_data_buffer = &wval;
    m_hydra_put.eChannel = LC2A;
    m_hydra_put.slave_address = 0x30;
    err = mstar_multi_pif_put_data(&m_hydra_put);

    if (err != MPIF_SUCCESS)
    {
        RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));
        return;//while(1);
    }

    rval = 0x5A5A;

    m_hydra_get.pget_data_buffer = &rval;
    m_hydra_get.eChannel = LC2A;
    m_hydra_get.slave_address = 0x30;
    err = mstar_multi_pif_get_data(&m_hydra_get);
    if (err != MPIF_SUCCESS)
    {
        RETAILMSG(DBG_MPIF_3A,(_T(" err = %d\r\n"),err));
        return;//while(1);
    }

    RETAILMSG(DBG_MPIF_3A,(_T(" ======== LC2A value 0x30, (0x%x)========\r\n"), rval));

    len = BUFFER_SIZE_16;//BUFFER_SIZE;
    k = 0;
    addr = 0;
    times = 0;

    while(!m_bFlagExitTest)
    {
        if(times > 0xFFFFF0)
            times = 0;
        else
            times++;

        for (i = 0; i < len; i++)
            m_tx[i] = (U8) ((addr + i) & 0xFF);

        // Write data to Carina.
        m_hydra_put.mode = MIU_MODE;
        m_hydra_put.put_length = (len>>4);  // one packet is 16 byte

        m_hydra_put.pput_data_buffer = m_tx;

        m_hydra_put.eChannel = LC3A;
        err = mstar_multi_pif_put_data(&m_hydra_put);

        for (i = 0; i < len; i++)
            m_rx[i] = 0xAA;

        // read back
        m_hydra_get.mode = MIU_MODE;
        m_hydra_get.get_length = (len>>4);  // one packet is 16 byte

        m_hydra_get.pget_data_buffer = m_rx;

        m_hydra_get.eChannel = LC3A;

        err = mstar_multi_pif_get_data(&m_hydra_get);

        for (i  = 0; i < len; i++)
        {
            if ( m_rx[i] != (U8)( ( addr + i ) & 0xff ) )
            {
                RETAILMSG(DBG_MPIF_3A,(_T("Read error: addr = 0x%x, correct value = 0x%02x, wrong value = 0x%02x\r\n"), addr+i, m_tx[i], m_rx[i]));
                RETAILMSG(DBG_MPIF_3A,(_T(" Current Length = %d == Set SPIF ADDR 0x%x\r\n"),len, addr));
                k ++;
            }
        }

	if(0 == times % 100)
		RETAILMSG(DBG_MPIF_3A,(_T(" LC3A_Test times are %d now.\r\n"), times));

        if (0 != k)
        {
            k = 0;
            break;
        }
	Sleep(5);
    }
}

BOOL Cmpif::DataFilePut(LPCTSTR lpFileName, DWORD addr)
{
	FILE* pFile;
	DWORD	dwfileLen;//, dwNumRead;
	int len = BUFLEN;
	int org_addr = addr;
	U8 *pDMAAddr;
	//U8 buf[BUFLEN];
	volatile U8 patch_write;

	//MpifTestInit();

	if ((pFile = _wfopen(lpFileName,_T("rb"))) != NULL) {
		if(fseek( pFile, 0, SEEK_END)){
			RETAILMSG(DBG_ARGO_INIT, (_T("[DataFilePut] fseek file[%s] fail!\r\n"),lpFileName));
			}
		else{
			dwfileLen = ftell(pFile);
			fseek( pFile, 0, SEEK_SET);

			ONOFF_DAR_PATH(0);
			pDMAAddr = (U8 *)m_pMPIFDMARegs;

			RETAILMSG(DBG_ARGO_INIT, (_T("[DataFilePut] start download fw\r\n")));

			while(1)
				{
				int readsize;

				readsize = fread(pDMAAddr, sizeof(char), len, pFile);
				if (readsize == 0)
					break;

				RETAILMSG(0/*DBG_ARGO_INIT*/, (_T("[DataFilePut] file data[%02x, %02x, %02x, %02x]\r\n"),pDMAAddr[0], \
					pDMAAddr[1], pDMAAddr[2], pDMAAddr[3]));

				SET_SPIF_3A_ADDR_LEN(addr, readsize);
				patch_write = pDMAAddr[readsize-1];
				FIRE_MPIF_3A_WRITE(MIU_MODE, readsize);
			#if 1//def _DEBUG
				memcpy(buf, pDMAAddr, readsize);
				memset(pDMAAddr, 0, readsize);

				FIRE_MPIF_3A_READ(MIU_MODE, readsize);
				patch_write = pDMAAddr[readsize-1];

				if (memcmp(pDMAAddr, buf, readsize) != 0)
					{
					int i,j;
					RETAILMSG(DBG_ARGO_INIT, (_T("\r\n")));
					j = 0;
					for (i=0; i < readsize; i++){
						if (pDMAAddr[i] != buf[i]){
							RETAILMSG(DBG_ARGO_INIT, (_T("(%04X) %02X %02X  "), i, pDMAAddr[i], buf[i]));
							j++;
							if(0 == j%5)
								RETAILMSG(DBG_ARGO_INIT, (_T("\r\n")));
							}
						}
					RETAILMSG(DBG_ARGO_INIT, (_T("\r\n\r\n")));
					return FALSE;
					}
			#endif
				addr += readsize;
				}

			RETAILMSG(DBG_ARGO_INIT, (_T("[DataFilePut] upload file success, filesize = %d bytes\r\n"),addr-org_addr));
			fclose(pFile);
			ONOFF_DAR_PATH(1);
			}
	 	}
	 else{
		RETAILMSG(DBG_ARGO_INIT, (_T("[DataFilePut]open file[%s] fail!\r\n"),lpFileName));
		return FALSE;
	 	}
	 return TRUE;
}

void Cmpif::load8051(LPCTSTR lpFileName)
{
	int addr = 0;
	int len;
	//unsigned short tmp;

	FILE* pFile;
	DWORD	dwfileLen, dwNumRead, decTime;
	U8 *pDMAAddr;

	pDMAAddr = (U8 *)m_pMPIFDMARegs;
	decTime = GetTickCount();

	if ((pFile = _wfopen(lpFileName,_T("rb"))) != NULL) {
		if(fseek( pFile, 0, SEEK_END)){
			RETAILMSG(DBG_ARGO_INIT, (_T("[load8051]fseek file[%s] fail!\r\n"),lpFileName));
			}
		else{
			dwfileLen = ftell(pFile);
			fseek( pFile, 0, SEEK_SET);
			dwNumRead = fread( pDMAAddr, sizeof( char ), dwfileLen, pFile );
			len = dwNumRead;
			RETAILMSG(DBG_ARGO_INIT, (_T("[load8051]fread file[0x%x] !\r\n"),dwNumRead));
			fclose(pFile);
			}
		}

    wreg(0, 0x0c, 0x000c); //init miu
    wreg(2, 0x5b, 0x0001); //set SPIF write to 51 sram (0 : MIU)
    wreg(2, 0x18, 0x0002);
    ONOFF_DAR_PATH(0);
    SET_SPIF_3A_ADDR_LEN(addr, len);
    FIRE_MPIF_3A_WRITE(MIU_MODE, len);

#if 1//def _DEBUG
	memcpy(buf, pDMAAddr, len);
	memset(pDMAAddr, 0, len);
	FIRE_MPIF_3A_READ(MIU_MODE, len);
	//{
	//	volatile U8 patch_write;
	//    patch_write = pDMAAddr[readsize-1];
	//}
	if (memcmp(pDMAAddr, buf, len) != 0){
		int i;
		for (i=0; i < len; i++)
			if (pDMAAddr[i] != buf[i])
				RETAILMSG(DBG_ARGO_INIT, (_T("[(%02X) %02X %02X] "), i, pDMAAddr[i], buf[i]));
				//diag_printf("(%02X) %02X %02X", i, pDMAAddr[i], buf[i]);
		}
	//diag_printf("(%02X) %02X %02X", readsize, pDMAAddr[0], buf[0]);
#endif

    ONOFF_DAR_PATH(1);
    // SPIF a 8051 load code step:
    wreg(2, 0x5b, 0x0000);
    wreg(2, 0x18, 0x0000);
	 // mcu soft reset
    wreg(2, 0x19, 0x0301);
    wreg(2, 0x19, 0x0300);
	RETAILMSG(DBG_ARGO_INIT, (_T("\r\n[load8051]upload 8051 finished!\r\n")));
/*
    //cyg_thread_delay(1000); // wait for 8051 stable..., or how to make sure 8051 runs??
    // check offset 0x48(high byte) of bank 0 to judge whether Carina is scanning or not
    rreg(0, 0x48, tmp);
    while (tmp != 0x0800) {
		RETAILMSG(DBG_ARGO_INIT, (_T("tmp = 0x%x\r\n"),tmp));
		Sleep(10);
		if(m_bFlagExitTest){
			RETAILMSG(DBG_ARGO_INIT, (_T("8051 download fail....\r\n")));
			return;
			}
		rreg(0, 0x48, tmp);
    }
*/
	RETAILMSG(DBG_ARGO_INIT, (_T("8051 starts scanning....\r\n")));
}

void Cmpif::MpifWriteBuffer(int addr, int len, char *buf)
{
    int remain=len, writesize;
	char *ptr = buf;
	U8 *pDMAAddr;

	pDMAAddr = (U8 *)m_pMPIFDMARegs;

	while (remain > 0) {
		if (remain > MIU_MODE_MAX_DATA_BYTES_3A) {
			writesize = MIU_MODE_MAX_DATA_BYTES_3A;
		} else {
			writesize = remain;
		}
		memcpy(pDMAAddr, ptr, writesize);
		SET_SPIF_3A_ADDR_LEN(addr, writesize);
        FIRE_MPIF_3A_WRITE(MIU_MODE, writesize);
		ptr += writesize;
		remain -= writesize;
		addr += writesize;
	}
}

void Cmpif::MpifReadBuffer(int addr, int len, char *buf)
{
    int remain=len, readsize;
	int bytealign;
    char *ptr = buf;
	int firstcheck = 1;
	U8 *pDMAAddr;

	pDMAAddr = (U8 *)m_pMPIFDMARegs;

	bytealign = 8 - (addr%8);
	if (bytealign == 8) bytealign = 0;
	remain += bytealign;

	addr -= bytealign;
	while (remain > 0) {
		if (remain > MIU_MODE_MAX_DATA_BYTES_3A) {
			readsize = MIU_MODE_MAX_DATA_BYTES_3A;
		} else {
			readsize = remain;
		}

		SET_SPIF_3A_ADDR_LEN(addr, readsize);
        FIRE_MPIF_3A_READ(MIU_MODE, readsize);

		if (firstcheck == 1) {
			firstcheck = 0;
			memcpy(ptr, (pDMAAddr+bytealign), readsize-bytealign);
			ptr += (readsize-bytealign);
		} else {
			memcpy(ptr, pDMAAddr, readsize);
			ptr += readsize;
		}

		remain -= readsize;
		addr += readsize;
	}
}

BOOL Cmpif::CarinaInit()
{
	short regval;
	U8	u8Count = 0;

	RETAILMSG(DBG_ARGO_INIT, (_T("PIF INIT~~~~\r\n")));
	//1pif_init();
	MpifTestInit();

	//diag_printf("8051 BOOT~~~~\n");
	RETAILMSG(DBG_ARGO_INIT, (_T("8051 BOOT~~~~\r\n")));
	load8051(TEXT("\\nandflash\\CMMB.bin"));//(MS_FN("\\SDMMC\\CMMB.bin"));

	//diag_printf("KAPONO INIT~~~~\n");
	//kapono_init(CMMB_freq,CMMB_ts2,CMMB_ts1,CMMB_ts0,1);
	//kapono_init(530,0,0,0x7801,1);

	//mmp_mutex_init(&get_data_mutex);
	//mmp_mutex_init(&cmd_mutex);

    //diag_printf("AEON BOOT~~~~\n");
	RETAILMSG(DBG_ARGO_INIT, (_T("AEON BOOT~~~~\r\n")));
	AeonBoot();

	do {
		rreg(MBOX_BANK, 0x2f, regval);
		Sleep(10);
		u8Count ++;
		if(m_bFlagExitTest || u8Count >250){
			RETAILMSG(DBG_ARGO_INIT, (_T("AeonBoot  fail....\r\n")));
			return FALSE;
			}
	} while (regval != 0x4321);

	RETAILMSG(DBG_ARGO_INIT, (_T("AEON BOOT SUCCESS\r\n")));
	return TRUE;
}

void Cmpif::AeonBoot()
{
	int len = BUFLEN;
    //int i;
    //int ret;
    int aeon_en_vector, first_aeon_vec;
    int uart_sel;
    //unsigned char buf[BUFLEN];
    U32 regval;
    //unsigned int pc;

//RELOAD:
    // spif reset
    rreg(2, 0x11, regval);
    regval &= ~1;
    wreg(2, 0x11, regval);

    // disable all aeons
    wreg_b(AEON_V0_BANK, AEON_ENABLE, 0x0);
    wreg_b(AEON_V1_BANK, AEON_ENABLE, 0x0);
    wreg_b(AEON_A0_BANK, AEON_ENABLE, 0x0);

    // aeon sw reset
    rreg(AEON_PH_BANK, AEON_SWRESET>>1, regval);
    regval |= 7;
    wreg_b(AEON_PH_BANK, AEON_SWRESET, regval);
    aeon_en_vector = 0;
    first_aeon_vec = 0;

#if SUPPORT_AEON_V0
	RETAILMSG(DBG_ARGO_INIT, (_T("upload v0 image\r\n")));
	DataFilePut(TEXT("\\nandflash\\aeon_v0.bin"), AEON_V0_I_BASE);
    aeon_en_vector |= 1<<AEON_V0_ID;
    // aeon v0 enable and memory mapping configuration
    wreg_b(AEON_V0_BANK, AEON_ENABLE, 0x3);
    wreg_b(AEON_V0_BANK, AEON_IMEM_ADDR0,  CVT_IMEM_ADDR(AEON_V0_I_BASE)&0xff);
    wreg_b(AEON_V0_BANK, AEON_IMEM_ADDR1, (CVT_IMEM_ADDR(AEON_V0_I_BASE)>>8)&0xff);
    wreg_b(AEON_V0_BANK, AEON_IMEM_ADDR2, (CVT_IMEM_ADDR(AEON_V0_I_BASE)>>16)&0xff);
    wreg_b(AEON_V0_BANK, AEON_DMEM_ADDR0,  CVT_DMEM_ADDR(AEON_V0_D_BASE)&0xff);
    wreg_b(AEON_V0_BANK, AEON_DMEM_ADDR1, (CVT_DMEM_ADDR(AEON_V0_D_BASE)>>8)&0xff);
    wreg_b(AEON_V0_BANK, AEON_DMEM_ADDR2, (CVT_DMEM_ADDR(AEON_V0_D_BASE)>>16)&0xff);
    wreg(AEON_V0_BANK, 0x73, 0x0006); // ECO1, ECO2
    // set CPU freq
    rreg(AEON_G_BANK, AEON_G_CPU_CLK0>>1, regval);
    regval &= ~0xff;
    regval |= AEON_V0_CPU_CLK<<2;
    regval |= AEON_G_CPU_INVERT_CLOCK<<1;
    regval |= AEON_G_CPU_GATE_CLOCK;
    wreg(AEON_G_BANK, AEON_G_CPU_CLK0>>1, regval);
    rreg(AEON_G_BANK, AEON_G_CPU_CLK0>>1, regval);
    //print_cpu_clk(AEON_V0_ID, AEON_V0_CPU_CLK);
    // set ISB length
    rreg(AEON_V0_BANK, AEON_ISB>>1, regval);
    regval &= ~0xe000;
    // ISB level 1/2/3/4 => regval 5/6/7/0
    regval |= ((AEON_V0_ISB + 4)&0x7)<<13;
    wreg(AEON_V0_BANK, AEON_ISB>>1, regval);
#endif
#if SUPPORT_AEON_V1
	RETAILMSG(DBG_ARGO_INIT, (_T("upload v1 image\r\n")));
	DataFilePut(TEXT("\\nandflash\\aeon_v1.bin"), AEON_V1_I_BASE);
    aeon_en_vector |= 1<<AEON_V1_ID;
    // aeon v1 enable and memory mapping configuration
    wreg_b(AEON_V1_BANK, AEON_ENABLE, 0x3);
    wreg_b(AEON_V1_BANK, AEON_IMEM_ADDR0,  CVT_IMEM_ADDR(AEON_V1_I_BASE)&0xff);
    wreg_b(AEON_V1_BANK, AEON_IMEM_ADDR1, (CVT_IMEM_ADDR(AEON_V1_I_BASE)>>8)&0xff);
    wreg_b(AEON_V1_BANK, AEON_IMEM_ADDR2, (CVT_IMEM_ADDR(AEON_V1_I_BASE)>>16)&0xff);
    wreg_b(AEON_V1_BANK, AEON_DMEM_ADDR0,  CVT_DMEM_ADDR(AEON_V1_D_BASE)&0xff);
    wreg_b(AEON_V1_BANK, AEON_DMEM_ADDR1, (CVT_DMEM_ADDR(AEON_V1_D_BASE)>>8)&0xff);
    wreg_b(AEON_V1_BANK, AEON_DMEM_ADDR2, (CVT_DMEM_ADDR(AEON_V1_D_BASE)>>16)&0xff);
    // configure QMEM to 0xc000000, size 2Kx2
    wreg(AEON_V1_BANK, AEON_QMEM_DMASK0>>1, 0xf800); // 2Kx2
    wreg(AEON_V1_BANK, AEON_QMEM_DMASK1>>1, 0xffff);
    wreg(AEON_V1_BANK, AEON_QMEM_DADDR0>>1, 0x0);
    wreg(AEON_V1_BANK, AEON_QMEM_DADDR1>>1, 0xc000);
    wreg(AEON_V1_BANK, 0x73, 0x0006);  // ECO1, ECO2
    // set CPU freq
    rreg(AEON_G_BANK, AEON_G_CPU_CLK1>>1, regval);
    regval &= ~0xff;
    regval |= AEON_V1_CPU_CLK<<2;
    regval |= AEON_G_CPU_INVERT_CLOCK<<1;
    regval |= AEON_G_CPU_GATE_CLOCK;
    wreg(AEON_G_BANK, AEON_G_CPU_CLK1>>1, regval);
    rreg(AEON_G_BANK, AEON_G_CPU_CLK1>>1, regval);
    //print_cpu_clk(AEON_V1_ID, AEON_V1_CPU_CLK);
    // set ISB length
    rreg(AEON_V1_BANK, AEON_ISB>>1, regval);
    regval &= ~0xe000;
    // ISB level 1/2/3/4 => regval 5/6/7/0
    regval |= ((AEON_V1_ISB + 4)&0x7)<<13;
    wreg(AEON_V1_BANK, AEON_ISB>>1, regval);
#endif

#if SUPPORT_AEON_A0
	RETAILMSG(DBG_ARGO_INIT, (_T("upload A0 image\r\n")));
	DataFilePut(TEXT("\\nandflash\\aeon_a0.bin"), AEON_A0_I_BASE);
    aeon_en_vector |= 1<<AEON_A0_ID;
    // aeon a0 enable and memory mapping configuration
    wreg_b(AEON_A0_BANK, AEON_ENABLE, 0x3);
    wreg_b(AEON_A0_BANK, AEON_IMEM_ADDR0,  CVT_IMEM_ADDR(AEON_A0_I_BASE)&0xff);
    wreg_b(AEON_A0_BANK, AEON_IMEM_ADDR1, (CVT_IMEM_ADDR(AEON_A0_I_BASE)>>8)&0xff);
    wreg_b(AEON_A0_BANK, AEON_IMEM_ADDR2, (CVT_IMEM_ADDR(AEON_A0_I_BASE)>>16)&0xff);
    wreg_b(AEON_A0_BANK, AEON_DMEM_ADDR0,  CVT_DMEM_ADDR(AEON_A0_D_BASE)&0xff);
    wreg_b(AEON_A0_BANK, AEON_DMEM_ADDR1, (CVT_DMEM_ADDR(AEON_A0_D_BASE)>>8)&0xff);
    wreg_b(AEON_A0_BANK, AEON_DMEM_ADDR2, (CVT_DMEM_ADDR(AEON_A0_D_BASE)>>16)&0xff);
    // configure QMEM to 0xc000000, size 8K
    wreg(AEON_A0_BANK, AEON_QMEM_DMASK0>>1, 0xe000); // 8K
    wreg(AEON_A0_BANK, AEON_QMEM_DMASK1>>1, 0xffff);
    wreg(AEON_A0_BANK, AEON_QMEM_DADDR0>>1, 0x0);
    wreg(AEON_A0_BANK, AEON_QMEM_DADDR1>>1, 0xc000);
    wreg(AEON_A0_BANK, 0x73, 0x0006);  // ECO1, ECO2
    // set CPU freq
    rreg(AEON_G_BANK, AEON_G_CPU_CLK0>>1, regval);
    regval &= ~0xff00;
    regval |= AEON_A0_CPU_CLK<<10;
    regval |= AEON_G_CPU_INVERT_CLOCK<<1;
    regval |= AEON_G_CPU_GATE_CLOCK;
    wreg(AEON_G_BANK, AEON_G_CPU_CLK0>>1, regval);
    rreg(AEON_G_BANK, AEON_G_CPU_CLK0>>1, regval);
    //print_cpu_clk(AEON_A0_ID, AEON_A0_CPU_CLK);
    // set ISB length
    rreg(AEON_A0_BANK, AEON_ISB>>1, regval);
    regval &= ~0xe000;
    // ISB level 1/2/3/4 => regval 5/6/7/0
    regval |= ((AEON_A0_ISB + 4)&0x7)<<13;
    wreg(AEON_A0_BANK, AEON_ISB>>1, regval);
#endif

    if (first_aeon_vec==0)
        first_aeon_vec = aeon_en_vector;
	RETAILMSG(DBG_ARGO_INIT, (_T("AEON enable vector: 0x%x\r\n"),aeon_en_vector));

    // map spi address
    wreg(AEON_V0_BANK, AEON_SPI_SPACE>>1, 0xa200);
    wreg(AEON_V1_BANK, AEON_SPI_SPACE>>1, 0xa200);
    wreg(AEON_A0_BANK, AEON_SPI_SPACE>>1, 0xa200);

    uart_sel = AEON_UART_A0; //andyuart
    // uart source selection
    rreg(AEON_PH_BANK, AEON_UARTSEL>>1, regval);
    regval &= ~7;
    regval |= uart_sel;
    wreg_b(AEON_PH_BANK, AEON_UARTSEL, regval);

	// enable rx
    //rreg(0, 0x4f, regval);
    //regval |= (1<<8);
    //wreg(0, 0x4f, regval);
    // new setting
    rreg(0, 0x29, regval);
    regval |= (1<<5);
    wreg(0, 0x29, regval);

	{
    // aeon sw reset
    int mask_pat;
    rreg(AEON_PH_BANK, AEON_SWRESET>>1, regval);
	//printf("!!!!%d!!!!",regval);
	RETAILMSG(DBG_ARGO_INIT, (_T("AEON sw reset: 0x%x\r\n"),regval));
    mask_pat = aeon_en_vector;//(1<<(argc-1))-1;
    regval &= ~mask_pat;
	//printf("!!!!%d %d!!!!",regval, ~mask_pat);
	RETAILMSG(DBG_ARGO_INIT, (_T("!!!!%d %d!!!!\r\n"),regval, ~mask_pat));
	wreg_b(AEON_PH_BANK, AEON_SWRESET, regval);
	rreg(AEON_PH_BANK, AEON_SWRESET>>1, regval);
    //printf("!!!!%d!!!!",regval);
	RETAILMSG(DBG_ARGO_INIT, (_T("!!!!%d!!!!\r\n"),regval));
	}
}

#endif

