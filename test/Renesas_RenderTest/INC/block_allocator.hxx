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

#ifndef FIXED_BLOCK_ALLOCATOR
#define FIXED_BLOCK_ALLOCATOR

#if 0
/**/
/**/    ////////////////////////////////////////////////////////////////////////
/**/
/**/    ce::fixed_block_allocator
/**/
/**/    --- Overview -------------------------------------------------------
/**/
/**/    Allocator for fixed-size items; reduces memory fragmentation and improves
/**/    allocation performance.  See allocator.hxx for an overview of allocator usage.
/**/ 
/**/    In general, wherever ce::allocator is allowed, you can substitute a fixed-
/**/    block allocator by specifying
/**/ 
/**/        ce::fixed_block_allocator<N>
/**/
/**/    where N is the number of items to reserve on initial allocation.
/**/
/**/    To use this class you need to link to ceosutil.lib.
/**/
/**/    --- Examples -------------------------------------------------------
/**/
/**/    /// List will use an allocator obtaining memory using fixed blocks with
/**/    /// an initial block size of 100 items (1 item is the size of a list node).
/**/    typedef ce::list<ce::string, ce::fixed_block_allocator<100> > fixed100_list;
/**/    fixed100_list lst;
/**/
/**/    /// Creating a block allocator that has 50 initial blocks at compile-time...
/**/    ce::fixed_block_allocator<50> al;
/**/    /// ...and at run-time
/**/    size_t cBlocks = 50;
/**/    ce::fixed_block_allocator<> al2(cBlocks);
/**/
/**/    /// Allocating a single item from a fixed block
/**/    MyStruct* pStruct = (MyStruct*)al.allocate(sizeof(MyStruct));
/**/
/**/    /// Freeing a single item from a fixed block
/**/    al.deallocate(pStruct, sizeof(MyStruct));
/**/
/**/    /// Compacting the fixed block allocator's blocks to reclaim memory
/**/    al.compact();
/**/    lst.get_allocator().compact();
/**/
/**/    ////////////////////////////////////////////////////////////////////////
/**/
#endif

#include <svsutil.hxx>

namespace ce
{

// Allocator adapter class for ceosutil's fixed-block allocator
template <size_t _InitCount=20>
class fixed_block_allocator
{
public:    
    // Construction/destruction
    fixed_block_allocator() 
    {
        _Initialize(_InitCount);
    }
    fixed_block_allocator(size_t cInit)
    {
        _Initialize(cInit);
    }

    fixed_block_allocator(const fixed_block_allocator& al) 
    {
        // Copy ctor must initialize to empty; otherwise, two 
        // instances of this class would end up using the same blocks.
        _Initialize(al.m_cInit);
    }

    ~fixed_block_allocator() 
    {
#ifdef DEBUG
        // If this assert fires, you have unfreed items
        ASSERT(0 == m_cAllocated);
#endif

        // Shut down the allocator
        if (NULL != m_pMemDescr)
        {
            // Assume that all objects have been properly deallocated
            // (to help with leak tracking).
            svsutil_ReleaseFixedEmpty(m_pMemDescr);
            m_pMemDescr = NULL;
        }

        DeleteCriticalSection(&m_cs);
    }

    // Allocates an item of cbSize bytes.  The first time this function
    // is called, the size is stored; future allocations must be of the
    // same size.
    void* allocate(size_t cbSize) const
    {
        // Initialize the memory descriptor if needed
        if (NULL == m_pMemDescr) 
        {
            if (!_InitializeDescr(cbSize))
            {
                // Couldn't create the allocator
                return NULL;
            }
        }
        
        // Fixed-block allocator can't allocate variable-size items
        ASSERT(m_pMemDescr != NULL);
        ASSERT(cbSize == m_cbItemSize);

        void *pRet = svsutil_GetFixed(m_pMemDescr);     
            
#ifdef DEBUG
        if (NULL != pRet)
        { 
            InterlockedIncrement(&m_cAllocated);
        }
#endif
            
        return pRet; 
    }

    // Frees unallocated blocks to reclaim memory.  Does not allocate
    // or deallocate items.  This can be an expensive operation.
    void compact() const
    {
        if (NULL != m_pMemDescr) {
            svsutil_CompactFixed(m_pMemDescr);
        }
    }

    // Frees an allocated item previously returned by allocate.
    void deallocate(void* pv, size_t) const
    {
        if (pv == NULL)
        {
            return;
        }
        
        ASSERT(NULL != m_pMemDescr);
        svsutil_FreeFixed(pv, m_pMemDescr); 
        
#ifdef DEBUG
        InterlockedDecrement(&m_cAllocated);
#endif
    }

    // Compares two fixed block allocators.  Compares to
    // equivalence only if the two objects point to the same
    // descriptor.
    inline bool operator==(const fixed_block_allocator& al)
    {
        return al.m_pMemDescr == m_pMemDescr;
    }

    inline bool operator!=(const fixed_block_allocator& al)
    {
        return !(*this == al); 
    }

protected:
    void _Initialize(size_t cInit)
    {
        m_pMemDescr  = NULL;
        m_cInit      = cInit;
        InitializeCriticalSection(&m_cs);
#ifdef DEBUG
        m_cbItemSize = 0;
        m_cAllocated = 0;
#endif
    }
    
    bool _InitializeDescr(size_t cbSize) const
    {
        // Create a new descriptor
        FixedMemDescr* pMemDescr = svsutil_AllocFixedMemDescrSynch(cbSize, 
            m_cInit, &m_cs);

        if (NULL == pMemDescr)
        {
            return false;
        }

        // Assign the new memory descriptor atomically
        if (InterlockedCompareExchangePointer(
            &m_pMemDescr, 
            pMemDescr,
            NULL
            ) != NULL)
        {
            // Some other thread assigned the descriptor between the allocation
            // and assignment; clean up the new descriptor
            svsutil_ReleaseFixedEmpty(pMemDescr);
            ASSERT(m_pMemDescr != NULL);
        }
        
#ifdef DEBUG
        if (NULL != m_pMemDescr)
        {
            // Remember the allocation size for future allocations
            m_cbItemSize = cbSize;
        }
#endif            

        return (NULL != m_pMemDescr);
    }

protected:
    mutable size_t m_cInit;                 // The number of items to reserve on initial alloc
    mutable FixedMemDescr* volatile m_pMemDescr; // Fixed memory descriptor
    mutable CRITICAL_SECTION m_cs;
    
#ifdef DEBUG
    mutable volatile size_t m_cbItemSize;   // Size, in bytes, per item
    mutable LONG m_cAllocated;              // Number of currently-allocated items
#endif
};

};

#endif // FIXED_BLOCK_ALLOCATOR

