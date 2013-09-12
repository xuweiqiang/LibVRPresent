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

#if 0
/**/ 
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 1. Using allocators with containers
/**/    
/**/    // list will use a heap allocator to get memory
/**/    typedef ce::list<ce::string, ce::heap_allocator> heap_list;
/**/
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 2. Initializing allocators
/**/    
/**/    // Allocator used by l1 will create and use a new heap by calling HeapCreate(0, 0, 0);
/**/    heap_list l1;
/**/    
/**/    // Allocator used by l2 will be initialized with the handle to default process heap
/**/    heap_list l2(GetProcessHeap());
/**/
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 3. Using free_list allocator
/**/
/**/    // list will use an allocator obtaining memory using operator new and keeping a list of 
/**/    // 20 free block for quick reallocation
/**/    typedef ce::list<ce::string, ce::free_list<20> > free20_list;
/**/
/**/    // list will use an allocator obtaining memory from a heap and keeping a list of 
/**/    // 20 free blocks for quick reallocation
/**/    typedef ce::list<ce::string, ce::free_list<20, ce::heap_allocator> > free20_heap_list;
/**/
/**/    // create a private heap and specify it as an allocator for the list l5
/**/    ce::heap_allocator my_heap(0, 2 * 1024, 0);
/**/    free20_heap_list l5(my_heap);
/**/
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 4. Using singleton allocators
/**/    
/**/    // shared_free_list_allocator uses a single instance of 20 element free_list allocator
/**/    typedef ce::singleton_allocator<ce::free_list<20> > shared_free_list_allocator;
/**/
/**/    // my_list will use a singleton free_list allocator to get memory, this means that all instances
/**/    // of my_list will share the same free list allocator, memory freed by items removed from one
/**/    // instance of my_list will be put on the free list for quick allocation for items for other instances.
/**/    typedef ce::list<ce::string, shared_free_list_allocator> my_list;
/**/
/**/    // The lists l1 and l2 will allocate/deallocate memory for their elements using the same free_list allocator
/**/    singleton_heap_list l1, l2;
/**/    
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 5. Initializing singleton allocators
/**/
/**/    // allocator that uses a singleton instance of heap_allocator
/**/    typedef ce::singleton_allocator<ce::heap_allocator> shared_heap_allocator;
/**/
/**/    // heap_allocator singleton
/**/    typedef ce::singleton<ce::heap_allocator>           heap_allocator_singleton;
/**/    
/**/    // all instance of private_heap_list will allocate memory from the same private heap
/**/    typedef ce::list<ce::string, shared_heap_allocator> private_heap_list;
/**/
/**/    // initialize heap_allocator_singleton to use a private heap created using specified parameters
/**/    heap_allocator_singleton::init(0, 4 * 1024, 8 * 1024);
/**/
/**/    // l3 and l4 items will be allocated from the heap initialized above
/**/    private_heap_list l3, l4;
/**/
/**/
#endif


#ifndef __ALLOCATOR__
#define __ALLOCATOR__

#include <new>
#include <assert.h>
#include "singleton.hxx"
#include "auto_xxx.hxx"
#include "sync.hxx"

namespace ce
{

//
// Default allocator class
//
class allocator
{
public:
    void* allocate(size_t Size) const
        {return operator new(Size); }
        
    void deallocate(void* Ptr, size_t = 0) const
        {operator delete(Ptr); }
};


inline bool operator==(const allocator&, const allocator&)
    {return true; }
    
inline bool operator!=(const allocator&, const allocator&)
    {return false; }


//
// singleton_allocator template creates an allocator that uses a singleton instance
// of the allocator _Al specified as template argument. The template argument Unique
// can be used to created more than one singleton from given type _Al.
//
template <typename _Al, size_t Unique = 0>
class singleton_allocator : singleton_lifetime<singleton<_Al, Unique> >
{
public:
    void* allocate(size_t Size) const
        {return singleton<_Al, Unique>::instance().allocate(Size); }
        
    void deallocate(void* Ptr, size_t Size) const
        {singleton<_Al, Unique>::instance().deallocate(Ptr, Size); }
};


template <typename _Al, size_t Unique>
inline bool operator==(const singleton_allocator<_Al, Unique>&, const singleton_allocator<_Al, Unique>&)
    {return true; }
    
template <typename _Al, size_t Unique>
inline bool operator!=(const singleton_allocator<_Al, Unique>&, const singleton_allocator<_Al, Unique>&)
    {return false; }


//
// Allocator maintaining a free list
//
template <size_t Count, typename _Al = ce::allocator>
class free_list : _Al
{
public:
    typedef free_list<Count, _Al>   _Myt;
    typedef _Al                     _Mybase;
    typedef _Al                     allocator_type;

    free_list()
        : _FreeBlocks(0),
          _BlockSize((size_t)-1)
    {}
    
    explicit free_list(const allocator_type& _Alloc)
        : _FreeBlocks(0),
          _Mybase(_Alloc),
          _BlockSize((size_t)-1)
    {}
    
    //
    // Copy ctor must initialize free list to empty;
    // otherwise, two instances of free_list allocator 
    // would end up using the same "free" blocks.
    // 
    free_list(const _Myt& Right)
        : _FreeBlocks(0),
          _Mybase(Right),
         _BlockSize((size_t)-1)
    {}
    
    //
    // The lifetime of the allocator is typically the same as
    // lifetime of the container using it.
    //
    ~free_list()
    {
        assert(_FreeBlocks <= Count);
        __assume(_FreeBlocks <= Count);
        
        //
        // Free memory block on the free list
        //
        while(_FreeBlocks)
            _Mybase::deallocate(_FreeList[--_FreeBlocks], _BlockSize);
    }

    void* allocate(size_t Size) const
    {
        assert(_FreeBlocks <= Count);
        __assume(_FreeBlocks <= Count);
        
        gate<critical_section_with_copy> gate(_Lock);
        
        //
        // Sanity check to verify that all calls
        // to the allocator request the same size.
        //
        // If the assert was hit from singleton_free_list
        // it means that two containers that require different block sizes
        // are sharing the same free list. To resolve the conflict, specify Size
        // parameter when instantiating singleton_free_list template.
        //
        if(_BlockSize != (size_t)-1)
            assert(_BlockSize == Size);
        else
        {
            assert(Size != -1);
            _BlockSize = Size;
        }
        //
        // Return a memory block from free list 
        // or allocate from heap if the free list is empty.
        //
        if(_FreeBlocks)
            return _FreeList[--_FreeBlocks];
        else
        {
            gate.leave();
            return _Mybase::allocate(Size);
        }
    }
    
    void deallocate(void* Ptr) const
    {
        gate<critical_section_with_copy> gate(_Lock);
        
        //
        // Put the memory block on the free list
        // or free it if the list is already full.
        //
        if(_FreeBlocks < Count)
            _FreeList[_FreeBlocks++] = Ptr;
        else
        {
            gate.leave();
            _Mybase::deallocate(Ptr);
        }
    }
    
    void deallocate(void* Ptr, size_t Size) const
    {
        gate<critical_section_with_copy> gate(_Lock);
        
        //
        // Put the memory block on the free list
        // or free it if the list is already full.
        //
        if(_FreeBlocks < Count)
            _FreeList[_FreeBlocks++] = Ptr;
        else
        {
            gate.leave();
            _Mybase::deallocate(Ptr, Size);
        }
    }
    
private:
    mutable void*                       _FreeList[Count];
    mutable unsigned                    _FreeBlocks;
    mutable critical_section_with_copy  _Lock;
    mutable size_t                      _BlockSize;
};


template <size_t Count>
inline bool operator==(const free_list<Count>& fl1, const free_list<Count>& fl2)
    {return &fl1 == &fl2; }
    
template <size_t Count>
inline bool operator!=(const free_list<Count>& fl1, const free_list<Count>& fl2)
    {return &fl1 != &fl2; }


//
// shared_free_list is deprecated, use singleton_allocator instead
//
template <size_t Count, size_t Size = 0>
class shared_free_list : public singleton_allocator<free_list<Count>, Size>
{};

#pragma deprecated(shared_free_list)

#ifdef _AUTO_WINDEF_H_

//
// Heap allocator
//
class heap_allocator
{
public:
    friend bool operator==(const heap_allocator&, const heap_allocator&);
    friend bool operator!=(const heap_allocator&, const heap_allocator&);
    
    heap_allocator(HANDLE h)
        : bOwn(false),
          hHeap(h)
        {}
    
    heap_allocator(DWORD flOptions = 0,       // heap allocation attributes
                   SIZE_T dwInitialSize = 0,  // initial heap size
                   SIZE_T dwMaximumSize = 0)  // maximum heap size
        : bOwn(true)
        {hHeap = HeapCreate(flOptions, dwInitialSize, dwMaximumSize); }
    
    ~heap_allocator()
    {
        if(!bOwn)
            hHeap.release(); 
    }
    
    void* allocate(size_t Size) const
        {return HeapAlloc(hHeap, 0, Size); }
        
    void deallocate(void* Ptr, size_t = 0) const
        {HeapFree(hHeap, 0, Ptr); }
    
private:
    smart_hheap hHeap;
    bool        bOwn;
};

inline bool operator==(const heap_allocator& Left, const heap_allocator& Right)
    {return static_cast<HANDLE>(Left.hHeap) == static_cast<HANDLE>(Right.hHeap); }
    
inline bool operator!=(const heap_allocator& Left, const heap_allocator& Right)
    {return static_cast<HANDLE>(Left.hHeap) != static_cast<HANDLE>(Right.hHeap); }

#endif // _AUTO_WINDEF_H_

}; // namespace ce

#endif // __ALLOCATOR__
