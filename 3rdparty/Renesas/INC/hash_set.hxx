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

#ifndef __HASH_SET__
#define __HASH_SET__

//
// hash_set is a no-nonsense STL like set class template.
// 
// hash_set is a container used for storage and fast retrieval of data from a collection 
// in which the values of the elements contained are unique and serve as the key values.
// The implementation is based on hash table.
// Elements in hash_set are not ordered; enumerating elements returns them in undefined order.
// Each element in the set is unique. The implementation doesn't require exception handling.
//

#if 0
/**/
/**/    // 1. Default hash function can be used for numeric types
/**/
/**/    typedef ce::hash_set<int> set_of_int;
/**/    
/**/
/**/    // 2. Custom hash function can be defined by specializing hash_traits template
/**/    
/**/    template<>
/**/    struct ce::hash_traits<MyType>
/**/    {
/**/        size_t hash_function(const MyType& Key) const;
/**/    };
/**/
/**/    typedef ce::hash_set<MyType> MySet;
/**/    
/**/
/**/    // 3. You may also define your own traits object and instantiate hash_set with it.
/**/    //    Note that the traits object may have data members, constructor etc.
/**/
/**/    struct MyHashTraits
/**/    {
/**/        MyHashTraits(int nInit);
/**/        size_t hash_function(const MyType& Key) const;
/**/        /* ... */
/**/    };
/**/
/**/    typedef ce::hash_set<MyType, MyHashTraits> MySet1;
/**/    
/**/    // To initialize traits object you may pass it the hash_set ctor
/**/    MyHashTraits traits(10);
/**/    
/**/    MySet1 m_set(traits);
/**/    
/**/
/**/    // 4. Size of hash table defaults to 32 buckets but you may override it.
/**/    //    Note that the number of buckets must be power of 2
/**/    
/**/    set_of_int m_setInts;
/**/    MySet      m_setOpenedFiles(64);
/**/    
/**/
/**/    // 5. Standard container operations work with hash_set as expected.
/**/    
/**/    m_setInts.empty();   // true if set is empty
/**/    m_setInts.size();    // returns number of elements
/**/    m_setInts.clear();   // erases all the elements
/**/    
/**/
/**/    // 6. Iterators can be used to enumerate items, just like for any other container.
/**/    
/**/    for(MySet::iterator it = m_set.begin(), itEnd = m_set.end(); it != itEnd; ++it)
/**/    {
/**/        *it;  // item
/**/    }
/**/    
/**/
/**/    // 7. Use find method to quickly lookup item based on key.
/**/    //    Note that find returns end() if no item exists.
/**/
/**/    it = m_setInts.find(34);
/**/
/**/
/**/    // 8. Use insert method to insert an arbitrary item
/**/
/**/    it = m_setInts.insert(12);
/**/    it = m_setInts.insert(50);
/**/
/**/    // 10. Erase items by specifying key or iterator
/**/
/**/    m_setInts.erase(12);
/**/    m_setInts.erase(it);
/**/
#endif

// AUTOEXP.DAT
// 
// Add following line to your AUTOEXP.DAT to tell debugger how to display
// objects of this class in watch windows:
//
// ce::hash_set<*>=Size=<_List._Size>

// 4786 - identifier was truncated to 256 characters in the debug information
#pragma warning(disable: 4786)

#include "hash.hxx"

namespace ce
{

namespace details
{

template <typename K, typename _Tr = ce::hash_traits<K>, typename _Al = ce::allocator>
class hash_set_traits : protected _Tr
{
public:
    typedef K               value_type;
    typedef K               key_type;
    typedef _Al             allocator_type;
    
    hash_set_traits()
        {}
    
    hash_set_traits(const _Tr Traits)
        : _Tr(Traits)
        {}
    
    const key_type& _key(const value_type& Val) const
        {return Val; }
};

}; // namespace details


// hash_set template class
template <typename K, typename _Tr = ce::hash_traits<K>, typename _Al = ce::allocator>
class hash_set : public details::hash<details::hash_set_traits<K, _Tr, _Al> >
{
    typedef details::hash<details::hash_set_traits<K, _Tr, _Al> >   _Mybase;

public:
    typedef hash_set<K, _Tr, _Al>                                   _Myt;
    typedef K                                                       mapped_type;
    typedef K                                                       key_type;
    typedef typename _Mybase::size_type                             size_type;
    typedef typename _Mybase::allocator_type                        allocator_type;
    typedef typename _Mybase::iterator                              iterator;
    typedef typename _Mybase::const_iterator                        const_iterator;

public:
    // ctor
    explicit hash_set(size_type nBuckets = 32)
        : _Mybase(nBuckets)
        {}
        
    explicit hash_set(const _Tr& Traits, size_type nBuckets = 32)
        : _Mybase(Traits, nBuckets)
        {}
    
    explicit hash_set(const allocator_type& Alloc, size_type nBuckets = 32)
        : _Mybase(Alloc, nBuckets)
        {}
        
    explicit hash_set(const _Tr& Traits, const allocator_type& Alloc, size_type nBuckets = 32)
        : _Mybase(Traits, Alloc, nBuckets)
        {}
};

}; // namespace ce

#endif // __HASH_SET__
