/*****************************************************************************
Copyright 2003-2009 RMI Corporation. All rights reserved.

Any transfer or redistribution of the source code, with or without modification,
IS PROHIBITED, unless prior written consent was obtained. Any transfer or
redistribution of the binary code for use on the RMI Alchemy Family,
with or without modification, is permitted, provided that the following
condition is met:

Redistributions in binary form must reproduce the above copyright notice,

this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution:

THIS SOFTWARE IS PROVIDED BY RMI Corporation 'AS IS' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL RMI OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#ifndef _AU1X_COMMON__
#define _AU1X_COMMON__

/************************************************************************
						Prototypes for our 'C' Callers
		Some routines in the Src\Common\ dir are defined by Microsoft
************************************************************************/
#if !defined(ASSEMBLER)

#ifdef __cplusplus
	extern "C" {
#endif

	// msb.S
	extern ULONG getMSB(ULONG);
	// cp0.S
	extern ULONG cp0RdPRId(VOID);
	extern ULONG cp0RdCount(VOID);
	extern VOID  cp0WrCount(ULONG);
	extern ULONG cp0RdIndex(VOID);
	extern VOID  cp0WrIndex(ULONG);
	extern ULONG cp0RdRandom(VOID);
	extern VOID  cp0WrRandom(ULONG);
	extern ULONG cp0RdEntryLo0(VOID);
	extern VOID  cp0WrEntryLo0(ULONG);
	extern ULONG cp0RdEntryLo1(VOID);
	extern VOID  cp0WrEntryLo1(ULONG);
	extern ULONG cp0RdContext(VOID);
	extern VOID  cp0WrContext(ULONG);
	extern ULONG cp0RdPageMask(VOID);
	extern VOID  cp0WrPageMask(ULONG);
	extern ULONG cp0RdWired(VOID);
	extern VOID  cp0WrWired(ULONG);
	extern ULONG cp0RdBadVAddr(VOID);
	extern VOID  cp0WrBadVAddr(ULONG);
	extern ULONG cp0RdCount(VOID);
	extern VOID  cp0WrCount(ULONG);
	extern ULONG cp0RdEntryHi(VOID);
	extern VOID  cp0WrEntryHi(ULONG);
	extern ULONG cp0RdCompare(VOID);
	extern VOID  cp0WrCompare(ULONG);
	extern ULONG cp0RdStatus(VOID);
	extern VOID  cp0WrStatus(ULONG);
	extern ULONG cp0RdCause(VOID);
	extern VOID  cp0WrCause(ULONG);
	extern ULONG cp0Rd(VOID);
	extern VOID  cp0Wr(ULONG);
	extern ULONG cp0RdEPC(VOID);
	extern VOID  cp0WrEPC(ULONG);
	extern ULONG cp0RdConfig(VOID);
	extern VOID  cp0WrConfig(ULONG);
	extern ULONG cp0RdConfig0(VOID);
	extern VOID  cp0WrConfig0(ULONG);
	extern ULONG cp0RdLLAddr(VOID);
	extern VOID  cp0WrLLAddr(ULONG);
	extern ULONG cp0RdWatchLo(VOID);
	extern VOID  cp0WrWatchLo(ULONG);
	extern ULONG cp0RdWatchHi(VOID);
	extern VOID  cp0WrWatchHi(ULONG);
	extern ULONG cp0RdScratch(VOID);
	extern VOID  cp0WrScratch(ULONG);
	extern ULONG cp0RdDebug(VOID);
	extern VOID  cp0WrDebug(ULONG);
	extern ULONG cp0RdDEPC(VOID);
	extern VOID  cp0WrDEPC(ULONG);
	extern ULONG cp0RdPerfCnt(VOID);
	extern VOID  cp0WrPerfCnt(ULONG);
	extern ULONG cp0RdDTag(VOID);
	extern VOID  cp0WrDTag(ULONG);
	extern ULONG cp0RdITag(VOID);
	extern VOID  cp0WrITag(ULONG);
	extern ULONG cp0RdErrorEPC(VOID);
	extern VOID  cp0WrErrorEPC(ULONG);
	extern ULONG cp0RdDESave(VOID);
	extern VOID  cp0WrDESave(ULONG);
	extern ULONG DISABLE_INTERRUPTS(VOID);
	extern ULONG RESTORE_INTERRUPTS(ULONG);
	// cache.s
    extern void FlushDCache(VOID);
    extern void FlushDCacheLines(LPVOID, ULONG);
    extern void CleanDCacheLines(LPVOID, ULONG);
    extern void InvalidateDCacheLines(LPVOID, ULONG);
    extern void FlushICache(VOID);
    extern void FlushICacheLines(LPVOID, ULONG);
	// tlb.s
    extern void MipsClearTLB(ULONG);

	typedef struct {
	    unsigned long EntryLo0;
	    unsigned long EntryLo1;
	    unsigned long EntryHi;
	    unsigned long PageMask;

	} TLBENTRY, *PTLBENTRY;

	extern void TLBWrite(ULONG, PTLBENTRY);
	extern void TLBRead(ULONG, PTLBENTRY);
	// stall.c
	extern VOID OALStallExecutionEntry(VOID);
	extern VOID OALStallExecution(ULONG);
	// freq.c
	extern ULONG OEMGetCpuFrequency(VOID);
	extern ULONG OEMGetAuxFrequency(VOID);
	extern ULONG OEMGetSBUSFrequency(VOID);
	extern ULONG OEMGetPBUSFrequency(VOID);

	// General purpose sync routine
	static __inline void WBSYNC (VOID)
	{
		__asm("sync");
	}

#ifdef __cplusplus
	}
#endif


#endif // ASSEMBLER

//
#endif // _AU1X_COMMON__
