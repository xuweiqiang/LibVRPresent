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


Module Name:

    genblt_pcode.h

Abstract:

    This contains the definition for the PCode used by the GeneratedBlts
    component.

--*/

#ifndef __GENBLT_PCODE_H__
#define __GENBLT_PCODE_H__

#ifdef __cplusplus
extern "C" {
#endif

//
// Registers
//

// Struct for a single instruction

struct PCodeInstruction_t
{
    DWORD dwOpCode;
    int   iArgCount;
    DWORD dwArg1;
    DWORD dwArg2;
    DWORD dwArg3;
};

// Struct for register file

struct PCodeRegisters_t
{
    union
    {
        struct
        {
            ULONG s[64];
            ULONG r[16];
            ULONG v[14];
            ULONG d0;
            ULONG o0;        // Result pixels are stored here
            ULONG c0[256];   // Only valid if b{DSTPAL} == 1
            ULONG c1[256];   // Only valid if b{SRCPAL} == 1
            ULONG b;         // LSB == b0, MSB == b31
        };
        ULONG reg[609];
    };
};

// Function to parse and decode and instruction

extern
DWORD
InstructionFetchAndDecode(
    DWORD              * pdwPCode,
    PCodeInstruction_t * pInstr
    );

#ifdef __cplusplus
};
#endif

// In general, the registers are represented by a 32 bit bitfield.
//
// Format:  0cbodvrs 00000000 00000000 iiiiiiii
// where "i" represents and index.

#define PCODE_REG_S_SET     0x01000000
#define PCODE_REG_R_SET     0x02000000
#define PCODE_REG_V_SET     0x04000000
#define PCODE_REG_D_SET     0x08000000
#define PCODE_REG_O_SET     0x10000000
#define PCODE_REG_B_SET     0x20000000
#define PCODE_REG_C_SET     0x40000000

#define PCODE_REG_S_MASK    0x0100003f
#define PCODE_REG_R_MASK    0x0200000f
#define PCODE_REG_V_MASK    0x0400000f
#define PCODE_REG_D_MASK    0x0800000e
#define PCODE_REG_O_MASK    0x1000000f
#define PCODE_REG_B_MASK    0x2000001f
#define PCODE_REG_C_MASK    0x40000001

// B register defines.
#define PCODE_REG_B_SOLIDFILL    0x00000001   // b0
#define PCODE_REG_B_XPOSITIVE    0x00000002   // b1
#define PCODE_REG_B_YPOSITIVE    0x00000004   // b2
#define PCODE_REG_B_STRETCHX     0x00000008   // b3
#define PCODE_REG_B_STRETCHY     0x00000010   // b4
#define PCODE_REG_B_SHRINKX      0x00000020   // b5
#define PCODE_REG_B_SHRINKY      0x00000040   // b6
#define PCODE_REG_B_SOURCEALPHA  0x00000080   // b7
#define PCODE_REG_B_PERPIXEL     0x00000100   // b8
#define PCODE_REG_B_SRCCOLORKEY  0x00000200   // b9
#define PCODE_REG_B_DSTCOLORKEY  0x00000400   // b10
#define PCODE_REG_B_DSTMEMORY    0x00000800   // b11
#define PCODE_REG_B_SRCMEMORY    0x00001000   // b12
#define PCODE_REG_B_PATMEMORY    0x00002000   // b13
#define PCODE_REG_B_MASKMEMORY   0x00004000   // b14
#define PCODE_REG_B_MIRRORX      0x00008000   // b15
#define PCODE_REG_B_MIRRORY      0x00010000   // b16
#define PCODE_REG_B_LOOKUP       0x00020000   // b17
#define PCODE_REG_B_CONVERT      0x00040000   // b18
#define PCODE_REG_B_STRETCHING   0x00080000   // b19
#define PCODE_REG_B_NEEDDST      0x00100000   // b20
#define PCODE_REG_B_NEEDSRC      0x00200000   // b21
#define PCODE_REG_B_NEEDPAT      0x00400000   // b22
#define PCODE_REG_B_NEEDMASK     0x00800000   // b23
#define PCODE_REG_B_NEEDROP      0x01000000   // b24
#define PCODE_REG_B_SRC24BPP     0x02000000   // b25
#define PCODE_REG_B_DST8BPP      0x04000000   // b26
#define PCODE_REG_B_DST16BPP     0x08000000   // b27
#define PCODE_REG_B_DST24BPP     0x10000000   // b28
#define PCODE_REG_B_DST32BPP     0x20000000   // b29
#define PCODE_REG_B_UNUSED1      0x40000000   // b30
#define PCODE_REG_B_UNUSED2      0x80000000   // b31

#define MAKE_S_REG(i) (PCODE_REG_S_SET | (PCODE_REG_S_MASK & i))
#define MAKE_R_REG(i) (PCODE_REG_R_SET | (PCODE_REG_R_MASK & i))
#define MAKE_V_REG(i) (PCODE_REG_V_SET | (PCODE_REG_V_MASK & i))
#define MAKE_D_REG(i) (PCODE_REG_D_MASK)
#define MAKE_O_REG(i) (PCODE_REG_O_MASK)
#define MAKE_B_REG(i) (PCODE_REG_B_SET | (PCODE_REG_B_MASK & i))
#define MAKE_C_REG(i) (PCODE_REG_C_SET | (PCODE_REG_C_MASK & i))

#define PCODE_REG_TO_INDEX(reg, index) \
    ( (reg & PCODE_REG_S_SET) ? (reg & 0x3f) : \
        (reg & PCODE_REG_R_SET) ? (64 + (reg & 0x0f)) : \
            (reg & PCODE_REG_V_SET) ? (80 + (reg & 0x0f)) : \
                (reg & PCODE_REG_D_SET) ? (94) : \
                    (reg & PCODE_REG_O_SET) ? (95) : \
                        (reg == 0x40000000) ? (96 + index) : \
                            (reg == 0x40000001) ? (352 + index) : (-1) )

#define PCODE_REG_TO_OFFSET(reg) (PCODE_REG_TO_INDEX(reg, 0) * 4)

//
// Instructions
//

// Format: 00opcode u000comp xxxxxxxx xxxxrr0l
//    opcode   - opcode
//    u        - unsigned?
//    comp     - comparison
//    rr       - number of registers to follow
//    l        - literal to follow (after regs)
//    xxxxxxxx - "loop count" or "if count"

#define PCODE_OPCODE_MASK     0xff800000
#define PCODE_CMP_MASK        0x000f0000

#define PCODE_OPCODE_NOP      0x00000000
#define PCODE_OPCODE_SIG      0x01000000
#define PCODE_OPCODE_IF       0x02000000
#define PCODE_OPCODE_ELSE     0x03000000
#define PCODE_OPCODE_ENDIF    0x04000000
#define PCODE_OPCODE_LOOP     0x05000000
#define PCODE_OPCODE_ENDLOOP  0x06000000
#define PCODE_OPCODE_BREAK    0x07000000
#define PCODE_OPCODE_IFCMPS   0x08000000
#define PCODE_OPCODE_IFCMPU   0x08800000
#define PCODE_OPCODE_IFANDI   0x09000000
#define PCODE_OPCODE_IFORI    0x0a000000
#define PCODE_OPCODE_MOV      0x0b000000
#define PCODE_OPCODE_SHRI     0x0c000000
#define PCODE_OPCODE_SHLI     0x0d000000
#define PCODE_OPCODE_ADDI     0x0e000000
#define PCODE_OPCODE_SUBI     0x0f000000
#define PCODE_OPCODE_NEGI     0x10000000
#define PCODE_OPCODE_NOTI     0x11000000
#define PCODE_OPCODE_XORI     0x12000000
#define PCODE_OPCODE_ANDI     0x13000000
#define PCODE_OPCODE_ORI      0x14000000
#define PCODE_OPCODE_MAXSI    0x15000000
#define PCODE_OPCODE_MAXUI    0x15800000
#define PCODE_OPCODE_MINSI    0x16000000
#define PCODE_OPCODE_MINUI    0x16800000
#define PCODE_OPCODE_MULI     0x17000000
#define PCODE_OPCODE_LDB      0x18000000
#define PCODE_OPCODE_LDH      0x19000000
#define PCODE_OPCODE_LDR      0x1a000000
#define PCODE_OPCODE_STB      0x1b000000
#define PCODE_OPCODE_STH      0x1c000000
#define PCODE_OPCODE_STR      0x1d000000
#define PCODE_OPCODE_PRE      0x1e000000
#define PCODE_OPCODE_EOF      0x1f000000

#define PCODE_CMP_LE          0x00010000
#define PCODE_CMP_LT          0x00020000
#define PCODE_CMP_GE          0x00030000
#define PCODE_CMP_GT          0x00040000
#define PCODE_CMP_EQ          0x00050000
#define PCODE_CMP_NE          0x00060000

#define MAKE_OPCODE_CMP(opcode, comp, count, regcount, litcount) \
    (opcode | comp | ((count & 0xfff) << 4) | ((regcount & 3) << 2) | (litcount & 1))
#define MAKE_OPCODE_END(opcode, count) \
    (opcode | ((count & 0xfff) << 4))
#define MAKE_OPCODE(opcode, regcount, litcount) \
    (opcode | ((regcount & 3) << 2) | (litcount & 1))

#define GET_OPCODE_REGCOUNT(opcode) \
    ((opcode >> 2) & 3)
#define GET_OPCODE_LITCOUNT(opcode) \
    (opcode & 1)
#define GET_OPCODE_SIG(opcode) \
    ((opcode >> 4) & 0xfff)
#define GET_OPCODE_COND(opcode) \
    (opcode & PCODE_CMP_MASK)

#endif // __GENBLT_PCODE_H__
