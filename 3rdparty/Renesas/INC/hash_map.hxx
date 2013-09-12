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

#ifndef __HASH_MAP__
#define __HASH_MAP__

//
// hash_map is a no-nonsense STL like map class template.
// 
// hash_map is a associative container that allows quick lookup of elements of one type 
// by a key of a different type. The implementation is based on hash table.
// Elements in hash_map are not ordered; enumerating elements returns them in undefined order.
// Each element in the map has a unique key. The implementation doesn't require exception handling.
//

#if 0
/**/
/**/    // 1. For numeric keys you only need to specify types of the key and value and used predefined hash function.
/**/
/**/    typedef ce::hash_map<int, ce::wstring> map_of_strings;
/**/    
/**/
/**/    // 2. Define a custom hash function by specializing hash_traits template
/**/    
/**/    template<>
/**/    struct ce::hash_traits<MyKey>
/**/    {
/**/        size_t hash_function(const MyKey& Key) const;
/**/    };
/**/
/**/    typedef ce::hash_map<MyKey, MyValue> MyMap;
/**/    
/**/
/**/    // 3. You may also define your own traits object and instantiate hash_map with it.
/**/    //    Note that the traits object may have data members, constructor etc.
/**/
/**/    struct MyHashTraits
/**/    {
/**/        MyHashTraits(int nInit);
/**/        size_t hash_function(const MyKey& Key) const;
/**/        /* ... */
/**/    };
/**/
/**/    typedef ce::hash_map<MyKey, MyValue, MyHashTraits> MyMap1;
/**/    
/**/    // To initialize traits object you may pass it the hash_map ctor
/**/    MyHashTraits traits(10);
/**/    
/**/    MyMap1 m_map(traits);
/**/    
/**/
/**/    // 4. Size of hash table defaults to 32 buckets but you may override it.
/**/    //    Note that the number of buckets must be power of 2
/**/    
/**/    map_of_strings m_mapStrings;
/**/    MyMap          m_mapOpenedFiles(64);
/**/    
/**/
/**/    // 5. Standard container operations work with hash_map as expected.
/**/    
/**/    m_mapStrings.empty();   // true if map is empty
/**/    m_mapStrings.size();    // returns number of elements
/**/    m_mapStrings.clear();   // erases all the elements
/**/    
/**/
/**/    // 6. Iterators can be used to enumerate items, just like for any other container.
/**/    //    Note that items in the hash_map are pairs consisting of item Key and Value.
/**/    
/**/    for(MyMap::iterator it = m_map.begin(), itEnd = m_map.end(); it != itEnd; ++it)
/**/    {
/**/        it->first;  // key
/**/        it->second; // value
/**/    }
/**/    
/**/
/**/    // 7. Use find method to quickly lookup item based on key.
/**/    //    Note that find returns end() if no item for specified key exists.
/**/
/**/    it = m_mapStrings.find(34);
/**/
/**/    if(it == m_mapStrings.end())
/**/        return; // not found
/**/
/**/
/**/    // 8. Use operator[] to find existing item or insert a new "empty" (initialized using default ctor) item with given key.
/**/    //    Note that operator[] returns NULL if item doesn't exist and new item can’t be created (OOM).
/**/    
/**/    ce::wstring* pString = m_mapStrings[50]; 
/**/    
/**/
/**/    // 9. Use insert method to insert an arbitrary item
/**/
/**/    it = m_mapStrings.insert(12, L"string 12");
/**/
/**/    // 10. Erase items by specifying key or iterator
/**/
/**/    m_mapStrings.erase(50);
/**/    m_mapStrings.erase(it);
/**/
#endif

// AUTOEXP.DAT
// 
// Add following line to your AUTOEXP.DAT to tell debugger how to display
// objects of this class in watch windows:
//
// ce::hash_map<*>=Size=<_List._Size>

// 4786 - identifier was truncated to 256 characters in the debug information
#pragma warning(disable: 4786)

#include "hash.hxx"

namespace ce
{

namespace details
{

template <typename K, typename T, typename _Tr = ce::hash_traits<K>, typename _Al = ce::allocator>
class hash_map_traits : protected _Tr
{
public:
    typedef std::pair<K, T> value_type;
    typedef K               key_type;
    typedef _Al             allocator_type;
    
    hash_map_traits()
    {}
    
    hash_map_traits(const _Tr& Traits)
        : _Tr(Traits)
    {}
    
    const key_type& _key(const value_type& Val) const
        {return Val.first; }
};

}; // namespace details


// hash_map template class
template <typename K, typename T, typename _Tr = ce::hash_traits<K>, typename _Al = ce::allocator>
class hash_map : public details::hash<details::hash_map_traits<K, T, _Tr, _Al> >
{
    typedef details::hash<details::hash_map_traits<K, T, _Tr, _Al> >    _Mybase;

public:
    typedef hash_map<K, T, _Tr, _Al>                                    _Myt;
    typedef T                                                           mapped_type;
    typedef K                                                           key_type;
    typedef typename _Mybase::size_type                                 size_type;
    typedef typename _Mybase::allocator_type                            allocator_type;
    typedef typename _Mybase::iterator                                  iterator;
    typedef typename _Mybase::const_iterator                            const_iterator;

public:
    // ctor
    explicit hash_map(size_type nBuckets = 32)
        : _Mybase(nBuckets)
        {}
        
    explicit hash_map(const _Tr& Traits, size_type nBuckets = 32)
        : _Mybase(Traits, nBuckets)
        {}
    
    explicit hash_map(const allocator_type& Alloc, size_type nBuckets = 32)
        : _Mybase(Alloc, nBuckets)
        {}
        
    explicit hash_map(const _Tr& Traits, const allocator_type& Alloc, size_type nBuckets = 32)
        : _Mybase(Traits, Alloc, nBuckets)
        {}
        
    // insert
    iterator insert(const key_type& Key, const mapped_type& Mapped)
        {return _Mybase::insert(value_type(Key, Mapped)); }
        
    iterator insert(const value_type& Val)
        {return _Mybase::insert(Val); }
        
    iterator insert(iterator _Where, const value_type& Val)
        {return _Mybase::insert(_Where, Val); }
    
    template<class Iter>
    void insert(Iter First, Iter Last)
        {return _Mybase::insert(First, Last); }
    
    // operator[]
    mapped_type* operator[](const key_type& Key)
    {
        _Mybucket&  Bucket = _get_bucket(Key);
        size_type   nCount = _bucket_size(Bucket);
        
        //
        // Check if item with this key is already in the hash_map
        //
        for(iterator it = _bucket_begin(Bucket); nCount--; ++it)
        {
            assert(it != end());
            
            if(_key(*it) == Key)
                return &it->second;
        }
                
        //
        // Insert the new default item as the first item in the bucket
        //
        if(_List.end() == _List.insert(_bucket_begin(Bucket), value_type(Key, mapped_type())))
            return NULL;
        
        return ++_bucket_size(Bucket), &(--_bucket_begin(Bucket))->second;
    }
};

}; // namespace ce

#endif // __HASH_MAP__
