/* VPU4 register definition                 */
/* vpu_sample.h                             */
/* 2007.01.24                               */

#define	REG_VP4_CTRL		0x0000
#define	REG_VP4_VOL_CTRL	0x0004
#define	REG_VP4_IMAGE_SIZE	0x0008
#define	REG_VP4_MB_NUM		0x000C
#define	REG_VP4_DWY_ADDR	0x0010
#define	REG_VP4_DWC_ADDR	0x0014
#define	REG_VP4_D2WY_ADDR	0x0018
#define	REG_VP4_D2WC_ADDR	0x001C
#define	REG_VP4_DP1_ADDR	0x0020
#define	REG_VP4_DP2_ADDR	0x0024
#define	REG_VP4_STRS_ADDR	0x0028
#define	REG_VP4_STRE_ADDR	0x002C
#define	REG_VP4_VOP_CTRL	0x0030
#define	REG_VP4_VOP_TIME	0x0034
#define	REG_VP4_263_CTRL	0x0038
#define	REG_VP4_264_CTRL	0x003C
#define	REG_VP4_VLC_CTRL	0x0040
#define	REG_VP4_ENDIAN		0x0044
#define	REG_VP4_CMD		0x0048
#define	REG_VP4_ME_TH1		0x004C
#define	REG_VP4_ME_TH2		0x0050
#define	REG_VP4_ME_COST		0x0054
#define	REG_VP4_ME_SKIP		0x0058
#define	REG_VP4_ME_CTRL		0x005C
#define	REG_VP4_MBRF_CTRL	0x0060
#define	REG_VP4_MC_CTRL		0x0064
#define	REG_VP4_PRED_CTRL	0x0068
#define	REG_VP4_SLC_SIZE	0x006C
#define	REG_VP4_VOP_MINBIT	0x0070
#define	REG_VP4_MB_MAXBIT	0x0074
#define	REG_VP4_MB_TBIT		0x0078
#define	REG_VP4_RCQNT		0x007C
#define	REG_VP4_RCRP		0x0080
#define	REG_VP4_RCDJ		0x0084
#define	REG_VP4_RCWQ		0x0088
#define	REG_VP4_FWD_TIME	0x0094
#define	REG_VP4_BWD_TIME	0x0098
#define	REG_VP4_PST_TIME	0x009C
#define	REG_VP4_ILTFRAME	0x00A0
#define	REG_VP4_EC_REF		0x00A4
#define	REG_VP4_STATUS		0x0100
#define	REG_VP4_IRQ_ENB		0x0104
#define	REG_VP4_IRQ_STA		0x0108
#define	REG_VP4_VOP_BIT		0x010C
#define	REG_VP4_PRV_BIT		0x0110
#define	REG_VP4_SLC_MB		0x0114
#define	REG_VP4_QSUM		0x0118
#define	REG_VP4_DEC_ERR		0x011C
#define	REG_VP4_ERR_AREA	0x0120
#define	REG_VP4_NEXT_CODE	0x0124
#define	REG_VP4_MB_ATTR		0x0128
#define	REG_VP4_ERR_DET		0x0134
#define	REG_VP4_CLK_STOP	0x0138
#define	REG_VP4_MB_SADA		0x013C
#define	REG_VP4_MB_SADR		0x0140
#define	REG_VP4_MAT_RAM		0x1000
#define	REG_VP4_NC_RAM		0x2000
#define	REG_VP4_WT_RAM		0x20CC
#define	REG_VP4_CPY_ADDR	0x2264
#define	REG_VP4_CPC_ADDR	0x2268
#define	REG_VP4_R0Y_ADDR	0x226C
#define	REG_VP4_R0C_ADDR	0x2270
#define	REG_VP4_R1Y_ADDR	0x2274
#define	REG_VP4_R1C_ADDR	0x2278
#define	REG_VP4_R2Y_ADDR	0x227C
#define	REG_VP4_R2C_ADDR	0x2280
#define	REG_VP4_R3Y_ADDR	0x2284
#define	REG_VP4_R3C_ADDR	0x2288
#define	REG_VP4_R4Y_ADDR	0x228C
#define	REG_VP4_R4C_ADDR	0x2290
#define	REG_VP4_R5Y_ADDR	0x2294
#define	REG_VP4_R5C_ADDR	0x2298
#define	REG_VP4_R6Y_ADDR	0x229C
#define	REG_VP4_R6C_ADDR	0x22A0
#define	REG_VP4_R7Y_ADDR	0x22A4
#define	REG_VP4_R7C_ADDR	0x22A8
#define	REG_VP4_R8Y_ADDR	0x22AC
#define	REG_VP4_R8C_ADDR	0x22B0
#define	REG_VP4_R9Y_ADDR	0x22B4
#define	REG_VP4_R9C_ADDR	0x22B8
#define	REG_VP4_RAY_ADDR	0x22BC
#define	REG_VP4_RAC_ADDR	0x22C0
#define	REG_VP4_RBY_ADDR	0x22C4
#define	REG_VP4_RBC_ADDR	0x22C8
#define	REG_VP4_RCY_ADDR	0x22CC
#define	REG_VP4_RCC_ADDR	0x22D0
#define	REG_VP4_RDY_ADDR	0x22D4
#define	REG_VP4_RDC_ADDR	0x22D8
#define	REG_VP4_REY_ADDR	0x22DC
#define	REG_VP4_REC_ADDR	0x22E0
#define	REG_VP4_RFY_ADDR	0x22E4
#define	REG_VP4_RFC_ADDR	0x22E8