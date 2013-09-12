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


#ifndef __HANDLE_MAP__
#define __HANDLE_MAP__

#include <hash_map.hxx>
#include <sync.hxx>
//
// handle_map is used to create/work with a mapping from a handle to state(i.e. class/ memory blob).
// This is useful when you need to return a handle to a caller application, that will call you back 
// with that handle, and you need to get the associated state. Instead of giving the pointer to your state, you 
// return a handle and then look up the  map.
// 

// NOTES: Caller responsible for locking.

#if 0
    // T should be a pointer, and must cast to a PVOID.
    // NULL is not a a valid paramter.

    // Usage.

    
Init:
    typedef ce::smart_ptr <OBJ> POBJ;
    ce::handle_map<POBJ> g_ValidObjects;

Insert:
    POBJ p = new  OBJ1();
    g_ValidObjects.insert(p);


Find:
    DoWork ( HANDLE p)
    {
        POBJ  pObj =  g_ValidObjects.find(p);
        if (!pObj) return ERROR_INVALID_HANDLE;

        // Do Work.

    }

Cleanup:
    DoneWork ( HANDLE p)
    {
        g_ValidObjects.erase(p);
    }


GlobalCleanUp;
    ModuleGoingAway()
    {
        g_ValidObjects.clear();
    }
#endif 

namespace ce
{

template <typename T>
class handle_map 
{
public:
    typedef T                               mapped_type;
    typedef void*                              key_type;
    typedef hash_map<key_type,mapped_type>     hash_type; 

    // default constructor
    handle_map()
    {

    }
    // Returns: 
    //      Pointer to the state if found,
    //      or returns null if not found.
    mapped_type find(const key_type& Key)
    {
        hash_type::iterator it = m_InternalHash.find(Key);
        if (it == m_InternalHash.end()) return NULL; 
        return it->second;
        
    }
    
    // Returns: 
    //      Pointer to key if inserted.
    //      or returns null if not inserted.
    key_type insert(const mapped_type& Value)
    {
        if (Value == NULL) return NULL;

        const key_type Key = Value;
        hash_type::iterator it = m_InternalHash.insert(hash_type::value_type(Key,Value));
        if (it == m_InternalHash.end()) return NULL; 

        return it->second;
    }

    void erase(const key_type& Key)
    {
        m_InternalHash.erase(Key);
    }
    size_t size()
    {
        return m_InternalHash.size();
    }

    void clear()
    {
        m_InternalHash.clear();
    }

private:
    // copy ctor - Don't USE
    handle_map(const handle_map& Right);
    // operator= Don't USE
    handle_map& operator=(const handle_map& Right);
    hash_type m_InternalHash; 

};
}



#endif //__HANDLE_MAP__
