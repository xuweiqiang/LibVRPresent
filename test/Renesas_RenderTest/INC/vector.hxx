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
#ifndef __VECTOR__
#define __VECTOR__

// vector is a no-nonsense STL like vector class template.
// The template merely manages dynamic memory for a vector
// and thus has minimal or no overhead compared to manually allocation/freeing 
// dynamic memory.
// It keeps all items in a consecutive memory block so in case it needs to grow 
// memory it must copy items which might include calling copy ctor and dtor. 
// You may reserve some memory upfront to minimize reallocations.

#if 0
/**/    // 1. can be used with simple types or classes (must have copy ctor)
/**/
/**/    ce::vector<int>         vectorInt;
/**/    ce::vector<ce::string>  vectorStr;
/**/    ce::vector<POINT>       vectorPoint;
/**/
/**/    // 2. reserve memory to avoid reallocations
/**/
/**/    vectorInt.reserve(100); // reserve space for 100 items
/**/
/**/    vectorInt[99] = 99;
/**/    
/**/    // 3. adding items
/**/
/**/    // add item at the end of the vector
/**/    vectorStr.push_back(str);
/**/
/**/    // insert item at specified position
/**/    // note that this requires that all items from insertion point
/**/    // must be moved, this may be expensive
/**/    it = vectorStr.begin();
/**/    vectorStr.insert(it, L"string");
/**/
/**/    // insert n items at specified position
/**/    vectorStr.insert(it, 10, L"string");
/**/
/**/    // 4. removing items
/**/
/**/    // remove last item
/**/    vectorInt.pop_back();
/**/
/**/    // remove all items - does not free memeory
/**/    vectorInt.clear(); 
/**/
/**/    // remove specified item
/**/    // note that this requires that all items from deletion point
/**/    // must be moved, this may be expensive
/**/    vectorInt.erase(it);
/**/
/**/    // remove range of items
/**/    vectorInt.erase(it1, it2);
/**/    
/**/    // 5. iterating the STL way
/**/
/**/    for(ce::vector<POINT>::iterator it = vectorPoint.begin(), itEnd = vectorPoint.end(); it != itEnd; ++it)
/**/        if(it->x == 10)
/**/            break;
/**/
/**/    // 6. growth policy
/**/    //    whenever vector needs to grow as a result of inserting item(s), by default it will grow by extra 10 items
/**/    //    this behavior can be modified by specifying an alternate growth policy
/**/
/**/    // vector grows exponentially
/**/    typedef ce::vector<int, ce::allocator, ce::exp_growth> int_vector1;
/**/
/**/    // vector grows only by exact amount needed for current insertion
/**/    typedef ce::vector<int, ce::allocator, ce::incremental_growth<0> > int_vector2;
/**/
#endif

#include "allocator.hxx"
#include <new>
#include <assert.h>

namespace ce
{

//
// Growth policy where vector grows by extra Increment items
//
template<size_t Increment>
struct incremental_growth
{
    static size_t get_growth(size_t /*Size*/, size_t /*Capacity*/, size_t Need)
        {return Need + Increment; }
};


//
// Growth policy where vector grows by at least its current size
//
struct exp_growth
{
    static size_t get_growth(size_t Size, size_t /*Capacity*/, size_t Need)
        {return Need < Size ? Size : Need; }
};


// vector template class
template <typename T, typename _Al = ce::allocator, typename _Tr = ce::incremental_growth<10> >
class vector : _Al
{
public:
    typedef T*              iterator;
    typedef const T*        const_iterator;
    typedef vector<T, _Al>  _Myt;
    typedef size_t          size_type;
    typedef _Al             allocator_type;
    typedef T               value_type;
    
    vector()
        : _First(NULL), _Last(NULL), _End(NULL)
        {}
        
    explicit vector(const allocator_type& _Alloc)
        : _First(NULL), _Last(NULL), _End(NULL),
          _Al(_Alloc)
        {}        

    vector(size_type n)
        : _First(NULL), _Last(NULL), _End(NULL)
        {resize(n); }
        
    vector(size_type n, const allocator_type& _Alloc)
        : _First(NULL), _Last(NULL), _End(NULL),
          _Al(_Alloc)
        {resize(n); }
        
    vector(size_type n, const T& Val)
        : _First(NULL), _Last(NULL), _End(NULL)
        {resize(n, Val); }
        
    vector(size_type n, const T& Val, const allocator_type& _Alloc)
        : _First(NULL), _Last(NULL), _End(NULL),
          _Al(_Alloc)
        {resize(n, Val); }
    
    // copy ctor
    vector(const _Myt& Right)
        : _First(NULL), _Last(NULL), _End(NULL),
          _Al(Right.get_allocator())
    {
        _First = reinterpret_cast<iterator>(allocate(Right.size() * sizeof(T)));
        
        if(_First)
        {
            _Last = _Copy(Right.begin(), Right.end(), _First);
            _End = _Last; 
        }
    }

    ~vector()
    {
        // clear all elements in vector
        clear();

        // delete memory
        deallocate(_First, capacity() * sizeof(T));
    }
    
    // operator=
    _Myt& operator=(const _Myt& Right)
    {
        _Myt(Right)._Swap(*this);
        
        return *this;
    }
    
    // begin
    const_iterator begin() const
        {return _First; }

    iterator begin()
        {return _First; }

    // end
    const_iterator end() const
        {return _Last; }

    iterator end()
        {return _Last; }

    // front
    T& front()
        {return *(begin());}
    const T& front() const
        {return *(begin());}
    
    // back
    T& back()
    {
        iterator it = end();
        return *(--it);
    }
    const T& back() const
    {
        const_iterator it = end();
        return *(--it);
    }

    // resize
    bool resize(size_type _N)
    {
        if (size() < _N)
			return insert(end(), _N - size(), T());
		else if (_N < size())
			erase(begin() + _N, end());
            
        return true;
    }
        
    template<class _Val>
    bool resize(size_type _N, const _Val& _X)
    {
        if (size() < _N)
			return insert(end(), _N - size(), _X);
		else if (_N < size())
			erase(begin() + _N, end());
            
        return true;
    }
    
    // reserve
    bool reserve(size_type _N)
    {
        if(_N > capacity())
        {
            iterator _New = reinterpret_cast<iterator>(allocate(_N * sizeof(T)));
            
            if(NULL == _New)
                return false;
                
            //
            // Copy elements to new memory
            //
            _Copy(begin(), end(), _New);

            //
            // Destroy elements and deallocate old memory
            //
            _Destroy(begin(), end());
            deallocate(_First, capacity() * sizeof(T));

            _End = _New + _N;
            _Last = _New + size();
            _First = _New;
        }
        
        assert(_N <= capacity());
        
        return true;
    }

    // clear
    void clear()
    {
        erase(begin(), end());
    }
    
    void swap(_Myt& _Right)
        {_Swap(_Right); }
    
    // size
    size_type size() const
        {return _Last - _First; }
        
    // capacity
    size_type capacity() const
        {return _End - _First; }

    // empty
    bool empty() const
        {return (size() == 0); }

    // const operator[]
    const T& operator[](size_type i) const
        {return *(_First + i); }
    
    // operator[]
    T& operator[](size_type i)
        {return *(_First + i); }

    // insert
    template<class _Val>
    bool insert(iterator _Where, size_type _N, const _Val& _X)
    {
        assert(_Where >= begin());
        assert(_Where <= end());
        
        if(size() + _N > capacity())
        {
            //
            // The vector doesn't have space for _N additional items so it has to grow
            //
            size_type _NewCapacity = size() + _Tr::get_growth(size(), capacity(), _N);
            
            iterator _New = reinterpret_cast<iterator>(allocate(_NewCapacity * sizeof(T)));
            
            if(NULL == _New)
                return false;
                
            //
            // Copy items from the beginning to the insertion possition
            //
            iterator _Item = _Copy(begin(), _Where, _New);
            
            //
            // Insert new item(s)
            //
            for(; _N > 0; --_N, ++_Item)
                new (_Item)T(_X);
                
            //
            // Copy items from the insertion possition to the end
            //
            _Item = _Copy(_Where, end(), _Item);
            
            //
            // Destroy elements and deallocate old memory
            //
            _Destroy(begin(), end());
            deallocate(_First, capacity() * sizeof(T));
            
            _First = _New;
            _Last = _Item;
            _End = _First + _NewCapacity;
        }
        else
        {
            //
            // Vector has enough space, we need to move existing items and insert new ones
            //
            _MoveForward(_Where, end(), _Where + _N);
            
            //
            // Insert new item(s)
            //
            for(; _N > 0; --_N, ++_Where, ++_Last)
                new (_Where)T(_X);
        }
        
        return true;
    }
    
    template<class _Val>
    iterator insert(iterator _Where, const _Val& _X)
    {
        size_type _O = _Where - begin();
        
        if(!insert(_Where, 1, _X))
            return end();
        
        //
        // Insertion may require that vector is reallocated so
        // the final possition of the inserted item might not be 
        // the same as _Where
        //
        return begin() + _O;
    }
    
    
    // erase
    void erase(iterator _F, iterator _L)
    {
        assert(_F <= _L);
        assert(_F >= begin());
        assert(_L <= end());
        
        _MoveBackward(_L, end(), _F);
        
        
        _Last -= _L - _F;
    }
    
    void erase(iterator _Where)
        {erase(_Where, _Where + 1); }
    
    
    // push_back
    template<class _Val>
    bool push_back(const _Val& _X)
    {
        iterator it = insert(end(), _X); 
        return (it != end());
    }
        
    // pop_back
    void pop_back()
        {erase(end()-1); }
        
    const allocator_type& get_allocator() const
        {return *this; }

protected:
    // destroy elements from _F to _L
    void _Destroy(iterator _F, iterator _L)
    {
        for (; _F != _L; ++_F)
            _F->~T();
    }

    // copy elements from _F to _L to new location starting at _P
    iterator _Copy(const_iterator _F, const_iterator _L, iterator _P)
    {
        assert(_F <= _L);
        
        for(; _F != _L; ++_P, ++_F)
            new (_P) T(*_F);
        
        return (_P);
    }
    
    
    // move range of items from _F to _L forward to a new position _P
    void _MoveForward(iterator _F, iterator _L, iterator _P)
    {
        assert(_F >= begin() && _F <= end());
        assert(_L >= begin() && _L <= end());
        assert(_P >= begin() && _P <= _End);
        assert(_P + (_L - _F) > end());
        assert(_P + (_L - _F) <= _End);
        assert(_F <= _L);
        assert(_P > _F);
        
        //
        // Starting from last item in the range, re-construct the items
        // in their new position and destroy them in the old position.
        //
        for(--_L, _P += _L - _F; _L >= _F; --_P, --_L)
        {
            new (_P) T(*_L);
            _L->~T();
        }
    }
    

    // move range of items from _F to _L backward to a new position _P    
    void _MoveBackward(iterator _F, iterator _L, iterator _P)
    {
        assert(_F >= begin() && _F <= end());
        assert(_L >= begin() && _L <= end());
        assert(_P >= begin() && _P <= end());
        assert(_F <= _L);
        assert(_P <= _F);
        
        //
        // Starting from first item in the range, destroy existing item at new position
        // and then re-construct item being move in there.
        //
        for(; _F != _L; ++_P, ++_F)
        {
            _P->~T();
            new (_P) T(*_F);
        }
        
        // Destroy moved items that were not overwritten by other moved items
        _Destroy(_P, _L);
    }
    
    void _Swap(_Myt& Right)
    {
        // swapping vectors using incompatible allocators not supported
        assert(Right.get_allocator() == get_allocator());
        
        std::swap(Right._First, _First);
        std::swap(Right._Last, _Last);
        std::swap(Right._End, _End);
    }
    
private:
    iterator _First, _Last, _End;
};

template <typename T, typename _Al, typename _Tr>
inline void swap(vector<T, _Al, _Tr>& _Left, vector<T, _Al, _Tr>& _Right)
{
    _Left.swap(_Right);
}

}; // namespace ce

#endif // __VECTOR__
