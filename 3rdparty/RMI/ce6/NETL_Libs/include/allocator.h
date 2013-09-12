
/*
 *    File: include/allocator.h
 *  Author: Eric DeVolder
 * Purpose: Interface for common/allocator.c
 *   Notes: 
 *
 */

#ifndef _ALLOCATOR_H
#define _ALLOCATOR_H

//////////////////////////////////////////////////////////////////////

#include <osal.h>

#include <ctypes.h>
#include <logging.h>
#include <mediaErrors.h>

//////////////////////////////////////////////////////////////////////

typedef struct
{
    // Original values
    char name[32];
    uint32 memAlign;
    uint32 memPhysAddr;
    uint32 memPhysSize;
    uint8 * memVirtAddr;

    // Working values
    uint32 endPhysAddr;
    uint32 memAlignment;
    uint32 memAlignMask;

    uint32 nextPhysAddr;
    uint8 * nextVirtAddr;

} allocator_t;

void allocatorInit (allocator_t *context, const char *name, uint32 physAddr, void *virtAddr, uint32 size, uint32 align);
void allocatorReset (allocator_t *context);
void allocatorAllocBuffer (allocator_t *context, void **virtAddr, uint32 *physAddr, unsigned int size, unsigned int alignmentMask);
uint32 allocatorRemaining (allocator_t *context);
uint32 allocatorUsed (allocator_t *context);

//////////////////////////////////////////////////////////////////////

#endif // _ALLOCATOR_H

