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

//
// The collection of class templates provides a thread safe implementation of singleton pattern.
// 
// This implementation solves following problems with common C++ idioms used to implement singleton: 
// 
// - Order of initialization
//   Singleton can be used at any point, even during initialization of global/static objects that 
//   are initialized in unspecified order. 
// - Thread safety
//   Singleton creation is guaranteed to be atomic and happen exactly once in multi-threaded environment.
// - On-demand creation
//   Singleton may be created on the first use thus not using resources unless necessary.
// - Non-default initialization
//   The instance of the singleton can be initialized using a non-default constructor
// - Destruction on the last use
//   Singleton is destroyed when no longer used rather than only during unloading of the module.
// - Dead-reference
//   Singleton is recreated if accessed after it was destroyed (Phoenix singleton)
//

#if 0
/**/ 
/**/    ///////////////////////////////////////////////////
/**/    // 1. Accessing the instance of a singleton
/**/
/**/    typedef ce::singleton<MyType> MyTypeSingleton;
/**/
/**/    // Access the singleton using instance() method
/**/    // The first call to instance() will create the singleton
/**/    MyTypeSingleton::instance().MyMethod();
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 2. Controlling lifetime of a singleton
/**/
/**/    // Using singleton_lifetime you can easily specify that lifetime of the singleton
/**/    // should be at least the lifetime of the object:
/**/    
/**/    // a) Inheriting a class from singleton_lifetime will result in the singleton 
/**/    //    being created when the first object of the class is created and destroyed 
/**/    //    when the last object of this class is destroyed.
/**/    class MyClass : ce::singleton_lifetime<MyTypeSingleton>
/**/    {
/**/    public:
/**/        MyClass()
/**/        {}
/**/        
/**/        // ...
/**/        void foo()
/**/        {
/**/            MyTypeSingleton::instance().MyMethod();
/**/        // ...
/**/        }
/**/    };
/**/
/**/    // b) Declaring a global instance of singleton_lifetime will result in the singleton
/**/    //    being created during module initialization and destroyed during module unloading.
/**/    ce::singleton_lifetime<MyTypeSingleton> g_factory;
/**/
/**/    // You can control singleton lifetime by explicitly calling addref and release methods:
/**/
/**/    void Foo_Init()
/**/    {
/**/        // ...
/**/        MyTypeSingleton::addref();
/**/        // ...
/**/    }
/**/
/**/    void Foo_Deinit()
/**/    {
/**/        // ...
/**/        MyTypeSingleton::release();
/**/        // ...
/**/    }
/**/
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 3. Initializing a singleton
/**/
/**/    // When singleton is automatically created on the first use  
/**/    // it is initializes using its default ctor. 
/**/    // If there is a need to create singleton instance using non-default ctor, 
/**/    // user can call init method before the first use and pass initialization arguments. 
/**/    // init() returns true if the call created the singleton instance and false 
/**/    // if the instance already existed.
/**/    // Note that even if singleton is going to be initialized with non-default ctor,
/**/    // the class still MUST provide a default ctor. The default ctor may assert if it is
/**/    // required that the class is initialized with non-default ctor.
/**/
/**/    MyTypeSingleton::init("init string", 10);
/**/
/**/    MyTypeSingleton::addref();
/**/
/**/    // ...
/**/
/**/    MyTypeSingleton::instance().MyFun();
/**/
/**/    // ...
/**/
/**/    MyTypeSingleton::release();
/**/
#endif

#include <assert.h>
#include <new>
#include "sync.hxx"

#ifndef __SINGLETON__
#define __SINGLETON__

#pragma warning(push)
#pragma warning(disable : 4189) // warning C4189: 'identifier' : local variable is initialized but not referenced
#pragma warning(disable : 4345) // warning C4345: behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized

namespace ce
{

template<typename T, int Unique>    class static_allocator;
template<typename T>                class singleton_lifetime;


// singleton
template<typename T, int Unique = 0, typename _Al = ce::static_allocator<T, Unique> >
class singleton : _Al
{
    typedef singleton<T, Unique, _Al>   _Myt;

    singleton();
    ~singleton();
    singleton(const _Myt&);
    singleton& operator=(const _Myt&);
    
public:
    friend class singleton_lifetime<_Myt>;
        
    //
    // instance
    //
    static T& instance()
    {
        //
        // Using Double-Check idiom to assure atomic construction.
        // pInstance is a volatile pointer so that compiler optimizations
        // don't break Double-Check idiom.
        //
        if(NULL == pInstance)
            create();
        
        //
        // Singleton is constructed in static memory so we don't have to worry about OOM
        //
        assert(pInstance);
        __assume(pInstance);
        
        return *pInstance;
    }
    
    //
    // addref
    //
    static void addref()
    {
        assert(lRefCount >= 0);
        
        InterlockedIncrement(&lRefCount);
        
        //
        // Make sure that the singleton instance is created
        //
        if(NULL == pInstance)
            create();
    }
    
    //
    // release
    //
    static void release()
    {
        assert(lRefCount > 0);
        
        if(0 == InterlockedDecrement(&lRefCount))
        {
            gate<critical_section> gate(get_lock());
            
            if(pInstance)
            {
                pInstance->~T();
                deallocate(pInstance, sizeof(T));
                pInstance = NULL;
            }
        }
    }
    
    //
    // init template member functions can be used to initialize instance of the singleton using
    // a non-default constructor
    //
    template<typename A1>
    static bool init(const A1& a1)
    {
        gate<critical_section> gate(get_lock());
        
        if(NULL == pInstance)
        {
            T* volatile p = reinterpret_cast<T*>(allocate(sizeof(T)));
            new (p)T(a1);
            pInstance = p;
            
            return true;
        }
        
        return false;
    }
    
    
    template<typename A1, typename A2>
    static bool init(const A1& a1, const A2& a2)
    {
        gate<critical_section> gate(get_lock());
        
        if(NULL == pInstance)
        {
            T* volatile p = reinterpret_cast<T*>(allocate(sizeof(T)));
            new (p)T(a1, a2);
            pInstance = p;
            
            return true;
        }
        
        return false;
    }
    
    
    template<typename A1, typename A2, typename A3>
    static bool init(const A1& a1, const A2& a2, const A3& a3)
    {
        gate<critical_section> gate(get_lock());
        
        if(NULL == pInstance)
        {
            T* volatile p = reinterpret_cast<T*>(allocate(sizeof(T)));
            new (p)T(a1, a2, a3);
            pInstance = p;
            
            return true;
        }
        
        return false;
    }
    
    
    template<typename A1, typename A2, typename A3, typename A4>
    static bool init(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
    {
        gate<critical_section> gate(get_lock());
        
        if(NULL == pInstance)
        {
            T* volatile p = reinterpret_cast<T*>(allocate(sizeof(T)));
            new (p)T(a1, a2, a3, a4);
            pInstance = p;
            
            return true;
        }
        
        return false;
    }
    
    
    template<typename A1, typename A2, typename A3, typename A4, typename A5>
    static bool init(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
    {
        gate<critical_section> gate(get_lock());
        
        if(NULL == pInstance)
        {
            T* volatile p = reinterpret_cast<T*>(allocate(sizeof(T)));
            new (p)T(a1, a2, a3, a4, a5);
            pInstance = p;
            
            return true;
        }
        
        return false;
    }
    
protected:
    static void create()
    {
        gate<critical_section> gate(get_lock());
        
        if(NULL == pInstance)
        {
            T* volatile p = reinterpret_cast<T*>(allocate(sizeof(T)));
            new (p)T();
            pInstance = p;
        }
    }
    
    //
    // Critical section used to synchronize singleton can’t be defined as static member of 
    // singleton class because C++ doesn’t guarantee order of initialization for static variables 
    // from different translation units and thus singleton could not be safely instantiated by
    // global/static objects.
    //
    // To solve this problem the critical section is returned by get_lock function. This guarantees that
    // critical section is initialized before first use. To avoid race and assure that critical section is
    // initialized atomically and only once, the first call to the get_lock function is forced during module 
    // initialization which is done on a single thread.
    //
    // The end result is that critical section is initialized during module initialization (before multi-thread
    // races become an issue) AND at the latest before the first use.
    // 
    static critical_section& get_lock()
    {
        static critical_section cs;
        
        //
        // This statement forces compiler to instantiate _factory member
        // thus assuring that lock_factory ctor is called. 
        //
        lock_factory* volatile p = &_factory;

        return cs;
    }

protected:
    //
    // The side effect of lock_factory ctor is initialization of singleton critical section.
    // With the _factory member being static, this will happen during module initialization.
    //
    struct lock_factory
    {
        lock_factory()
            {get_lock(); }
    };
    
    static T* volatile  pInstance;
    static long         lRefCount;
    static lock_factory _factory;
};


//
// Definition of static members of singleton class
//
template<typename T, int Unique, typename _Al> T* volatile                                      singleton<T, Unique, _Al>::pInstance;
template<typename T, int Unique, typename _Al> long                                             singleton<T, Unique, _Al>::lRefCount;
template<typename T, int Unique, typename _Al> typename singleton<T, Unique, _Al>::lock_factory singleton<T, Unique, _Al>::_factory;


//
// A simple helper class that can be used to control lifetime of a singleton.
// It can also be used to create instance of a singleton that doesn't have a default ctor.
//
template<typename T>
class singleton_lifetime
{
public:
    typedef singleton_lifetime<T>   _Myt;
    typedef T                       _Mysingleton;
    
    singleton_lifetime()
        {_Mysingleton::addref(); }

    singleton_lifetime(const _Myt&)
        {_Mysingleton::addref(); }
    
    ~singleton_lifetime()
        {_Mysingleton::release(); }
};


//
// Static allocator class is a special purpose allocator used by singleton class.
// Since it can "allocate" only 1 memory block it is not a general purpose allocator.
// As consequence of the assumption about just 1 allocation, the allocator doesn't need any
// synchronization.
//
template<typename T, int Unique = 0>
class static_allocator
{
public:
    static void* allocate(size_t Size)
    {
        assert(sizeof(T) == Size);
          
        //
        // Asserting that only one memory block is requested from the allocator.
        // Hitting this assert might indicate that static_allocator is being used
        // in a context requiring a general purpose allocator.
        //
        assert(0 == nAllocated++);

        __declspec(align(16)) static char memory[sizeof(T)];
        
        //
        // We assume that 16 byte alignment is enough for T.
        //
        C_ASSERT(__alignof(memory) >= __alignof(T));
        
        return &memory;
    }
        
    static void deallocate(void*, size_t)
    {
        assert(0 == --nAllocated);
    }

#if defined DEBUG || defined _DEBUG
private:
    static int nAllocated;
#endif // DEBUG || _DEBUG
};

#if defined DEBUG || defined _DEBUG
template<typename T, int Unique> int static_allocator<T, Unique>::nAllocated = 0;
#endif // DEBUG || _DEBUG

//
// Comparison operators for static_allocator
//
template<typename T, int Unique>
inline bool operator==(const static_allocator<T, Unique>&, const static_allocator<T, Unique>&)
    {return true; }

template<typename T, int Unique>    
inline bool operator!=(const static_allocator<T, Unique>&, const static_allocator<T, Unique>&)
    {return false; }
    

}; // namespace ce

#pragma warning(pop)        

#endif //__SINGLETON__
