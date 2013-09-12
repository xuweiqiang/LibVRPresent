
/*
 *    File: include/perf.h
 *  Author: Eric DeVolder
 * Purpose: Interface to common/perf.c
 *   Notes:
 *
 */

#ifndef _PERF_H
#define _PERF_H

//////////////////////////////////////////////////////////////////////

#include <ctypes.h>

//////////////////////////////////////////////////////////////////////

#ifndef PERF_NUM_HISTOGRAM
#define PERF_NUM_HISTOGRAM 50
#endif

typedef struct
{
    const char *description;
    int enable;

    uint32 sampleFreq; // this is NOT a periodic sample, it is clock frequency of MIPS counter
    uint32 clocksPerUnitOfTime;
    int numberOfSamples;
    uint32 prevBeg, prevEnd;
    uint32 active_min, active_max, active_sum;
    uint32 idle_min, idle_max, idle_sum;
    int sampleMax;
    uint32 active_histogram[PERF_NUM_HISTOGRAM+1];
    uint32 idle_histogram[PERF_NUM_HISTOGRAM+1];
#ifdef PERF_NUM_SAMPLES
    struct
    {
        uint16 activeTime;
        uint32 dataSetSize;
    } samples[PERF_NUM_SAMPLES];
#endif

} perfContext_t;

#define MAX_PERF 16

extern perfContext_t *perf_registered[MAX_PERF];
extern int perf_count;

void perf_reset (void);
void perf_init (perfContext_t *perf, const char *description, uint32 sampleFreq);
void perf_enable (perfContext_t *perf, int enable);
void perf_sample (perfContext_t *perf, uint32 sampleBegin, uint32 sampleEnd, uint32 dataSetSize);
void perf_dump (perfContext_t *perf);

//////////////////////////////////////////////////////////////////////

#endif // _PERF_H

