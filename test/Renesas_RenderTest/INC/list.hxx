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
#ifndef __LIST__
#define __LIST__

// list is a no-nonsense STL like list class template.

#if 0
/**/ 
/**/    ///////////////////////////////////////////////////
/**/    // 1. can be used with simple types or classes
/**/
/**/    ce::list<int>           listInt;
/**/    ce::list<POINT>         listPoint;
/**/    ce::list<ce::string>    listString;
/**/
/**/    typedef ce::list<int>   int_list_type;
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 2. inserting items
/**/
/**/    // insert item at the end
/**/    if(!listPoint.push_back(pt))        // insert item using copy ctor
/**/        return E_OUTOFMEMORY;
/**/
/**/    if(!listString.push_back("text"))   // construct string item "in-place" from char*
/**/        return E_OUTOFMEMORY;
/**/
/**/    // insert item at the beginning
/**/    if(!listPoint.push_front(pt))       // insert item using copy ctor
/**/        return E_OUTOFMEMORY;
/**/
/**/    if(!listString.push_back("text"))   // construct string item "in-place" from char*
/**/        return E_OUTOFMEMORY;
/**/
/**/    // insert "empty" item initialized with default ctor
/**/    it = listInt.insert(listInt.end());         // at the end
/**/    it = listPoint.insert(listPoint.begin());   // at the begining
/**/    it = listString.insert(it);                 // before it
/**/
/**/    // insert item
/**/    it = listPoint.insert(it, pt);      // insert item using copy ctor
/**/    it = listString.insert(it, "text"); // construct string item "in-place" from char*
/**/
/**/    if(it == listString.end())          // check for insertion failure because of OOM
/**/        return E_OUTOFMEMORY;
/**/
/**/    // insert item in order into a sorted list
/**/    listInt.insert(std::upper_bound(listInt.begin(), listInt.end(), x), x);
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 3. removing items
/**/
/**/    // remove first item
/**/    listString.erase(listString.begin());
/**/
/**/    // remove last item
/**/    listString.erase(--listString.end());
/**/
/**/    // remove items from it1 (inclusive) to it2 (exclusive)
/**/    listString.erase(it1, it2);
/**/
/**/    // remove all items
/**/    listString.clear();
/**/
/**/    // remove all items with specified value
/**/    listInt.remove(10);
/**/
/**/    // conditional remove
/**/    listInt.remove_if(std::bind2nd(std::less<int>(), 5));   // remove all items lesser than 5
/**/
/**/    ///////////////////////////////////////////////////
/**/    // 4. list properties
/**/
/**/    if(listInt.empty())
/**/        // ...
/**/
/**/    n = listString.size();
/**/    
/**/    ///////////////////////////////////////////////////
/**/    // 5. iterators
/**/
/**/    // iterator types
/**/    ce::list<int>::iterator        it;
/**/    int_list_type::const_iterator  it; // const iterator (a la const pointer)
/**/
/**/    // iterating through a list
/**/    for(int_list_type::iterator it = listInt.begin(), itEnd = listInt.end(); it != itEnd; ++it)
/**/    {
/**/        printf("%d", *it);  // use iterator like a pointer
/**/    }
/**/
/**/    // using reverse iterator to iterate through a list backwards
/**/    for(int_list_type::reverse_iterator it = listInt.rbegin(), itEnd = listInt.rend(); it != itEnd; ++it)
/**/    {
/**/        printf("%d", *it);  // use iterator like a pointer
/**/    }
/**/    
/**/    ///////////////////////////////////////////////////
/**/    // 6. using STL algorithms
/**/
/**/    // find first item equal 20
/**/    it = std::find(listInt.begin(), listInt.end(), 20);
/**/
/**/    // count how many items are greater than 5
/**/    n  = std::count_if(listInt.begin(), listInt.end(), std::bind2nd(std::greater<int>(), 5));
/**/
/**/    // find first occurance of largest element
/**/    it = std::max_element(listString.begin(), listString.end());
/**/    
/**/    // copy items from another container
/**/    ce::vector<int> v;
/**/    // ...
/**/    std::copy(v.begin(), v.end(), std::back_inserter(l1));
/**/    std::copy(v.begin(), v.end(), std::front_inserter(l1));
/**/    
#endif

#pragma warning(push)

// warning C4284: warning for operator-> returning non-pointer;
//                compiler issues it even if -> is not used for the specific instance
#pragma warning(disable: 4284)

// warning C4345: behavior change: an object of POD type constructed with an initializer of the form () will be default-initialized
#pragma warning(disable: 4345)

#include <stddef.h>
#include "allocator.hxx"
#include <assert.h>
#include <algorithm>
#include <iterator>

namespace ce
{

// list template class
template <typename T, typename _Al = ce::allocator>
class list : _Al
{
protected:
    struct          _Node;
    friend struct   _Node;
    typedef _Node*  _NodePtr;
    
    struct _Node
    {
        T           _Value;         // _Value MUST be first field of _Node structure; 
        _NodePtr    _Next, _Prev;   // only _Next and _Prev, but not _Value, are valid for _Head
    };

public:
    class iterator;
    class const_iterator;

    typedef list<T, _Al>                                                    _Myt;
    typedef size_t                                                          size_type;
    typedef _Al                                                             allocator_type;
    typedef T                                                               value_type;
    typedef std::reverse_bidirectional_iterator<iterator, value_type>       reverse_iterator;
    typedef std::reverse_bidirectional_iterator<const_iterator, value_type> const_reverse_iterator;

    // ctor
    list()
        : _Head((_NodePtr)((char*)&_HeadNext - FIELD_OFFSET(_Node, _Next))), 
          _Size(0)
    {
        // _HeadPrev and _HeadNext must have the same sizes and relative layout as _Node::_Prev and _Node::_Next
        assert(&_Head->_Prev == &_HeadPrev);
        assert(&_Head->_Next == &_HeadNext);
        
        _Head->_Prev = _Head->_Next = _Head;
    }
    
    explicit list(const allocator_type& _Alloc)
        : _Head((_NodePtr)((char*)&_HeadNext - FIELD_OFFSET(_Node, _Next))), 
          _Size(0),
          _Al(_Alloc)
    {
        // _HeadPrev and _HeadNext must have the same sizes and relative layout as _Node::_Prev and _Node::_Next
        assert(&_Head->_Prev == &_HeadPrev);
        assert(&_Head->_Next == &_HeadNext);
        
        _Head->_Prev = _Head->_Next = _Head;
    }

    
    list(const _Myt& Right)
        : _Head((_NodePtr)((char*)&_HeadNext - FIELD_OFFSET(_Node, _Next))),
          _Size(0),
          _Al(Right.get_allocator())
    {
        // _HeadPrev and _HeadNext must have the same sizes and relative layout as _Node::_Prev and _Node::_Next
        assert(&_Head->_Prev == &_HeadPrev);
        assert(&_Head->_Next == &_HeadNext);
        
        _Head->_Prev = _Head->_Next = _Head;
        
        insert(begin(), Right.begin(), Right.end());
    }

    // dtor
    ~list()
    {
        clear();
    }

    // operator=
    _Myt& operator=(const _Myt& Right)
    {
        clear();
        insert(begin(), Right.begin(), Right.end());
        
        return (*this);
    }

    // begin
    iterator begin()
        {return (iterator(_Head->_Next)); }

    const_iterator begin() const
        {return (const_iterator(_Head->_Next)); }

    // end
    iterator end()
        {return (iterator(_Head)); }

    const_iterator end() const
        {return (const_iterator(_Head)); }

    // reverse begin
    reverse_iterator rbegin()
        {return (reverse_iterator(end())); }
        
    const_reverse_iterator rbegin() const
        {return (const_reverse_iterator(end())); }

    // reverse end
    reverse_iterator rend()
        {return (reverse_iterator(begin())); }
        
    const_reverse_iterator rend() const
        {return (const_reverse_iterator(begin())); }

    // size
    size_type size() const
        {return _Size; }

    // empty
    bool empty() const
        {return _Size == 0; }

    // clear
    void clear()
        {erase(begin(), end());}

    // insert item (initialized using ctor taking one argument)
    template<class _Val>
    iterator insert(iterator Where, const _Val& Value)
    {
        _NodePtr NewNode = _Buynode(Where._MyNode());
        
        if(NewNode == NULL)
            return end();

        // To avoid using operator& for type T (which may be overloaded)
        // we are constructing item in the memory pointed to by NewNode.
        // This assumes that _Value is the first member.
        new (NewNode) T(Value);
        
        return NewNode;
    }
    
    
    // insert item (initialized using ctor taking two arguments)
    template<class _Val1, class _Val2>
    iterator insert(iterator Where, const _Val1& Value1, const _Val2& Value2)
    {
        _NodePtr NewNode = _Buynode(Where._MyNode());
        
        if(NewNode == NULL)
            return end();

        // To avoid using operator& for type T (which may be overloaded)
        // we are constructing item in the memory pointed to by NewNode.
        // This assumes that _Value is the first member.
        new (NewNode) T(Value1, Value2);
        
        return NewNode;
    }
    
    
    // insert item (initialized using ctor taking three arguments)
    template<class _Val1, class _Val2, class _Val3>
    iterator insert(iterator Where, const _Val1& Value1, const _Val2& Value2, const _Val3& Value3)
    {
        _NodePtr NewNode = _Buynode(Where._MyNode());
        
        if(NewNode == NULL)
            return end();

        // To avoid using operator& for type T (which may be overloaded)
        // we are constructing item in the memory pointed to by NewNode.
        // This assumes that _Value is the first member.
        new (NewNode) T(Value1, Value2, Value3);
        
        return NewNode;
    }
    
    
    // insert default item (initialized using default ctor)
    iterator insert(iterator Where)
    {
        _NodePtr NewNode = _Buynode(Where._MyNode());
        
        if(NewNode == NULL)
            return end();

        // To avoid using operator& for type T (which may be overloaded)
        // we are constructing a default item in the memory pointed to by NewNode.
        // This assumes that _Value is the first member.
        new (NewNode) T();
        
        return NewNode;
    }
    
    
    bool insert(iterator Where, const_iterator First, const_iterator Last)
    {
        for(; First != Last; ++First)
            if(end() == insert(Where, *First))
                return false;
            
        return true;
    }
    
    
    bool insert(iterator Where, iterator First, iterator Last)
    {
        for(; First != Last; ++First)
            if(end() == insert(Where, *First))
                return false;
            
        return true;
    }

    // erase
    iterator erase(iterator Where)
    {
        assert(Where != end());
        
        _NodePtr WhereNode = (Where++)._MyNode();

        assert(WhereNode);
        __assume(WhereNode);
        assert(WhereNode->_Prev);
        __assume(WhereNode->_Prev);
        assert(WhereNode->_Next);
        __assume(WhereNode->_Next);
        
        WhereNode->_Prev->_Next = WhereNode->_Next;
        WhereNode->_Next->_Prev = WhereNode->_Prev;
        
        WhereNode->_Value.~T();
        _Freenode(WhereNode);
        
        --_Size;
        
        return Where;
    }

    iterator erase(iterator First, iterator Last)
    {    
        while (First != Last)
            erase(First++);
        
        return First; 
    }
    
    
    // remove
    void remove(const T& Value)
    {
        for(iterator it = begin(), itEnd = end(); it != itEnd;)
            if(*it == Value)
                erase(it++);
            else
                ++it;
    }
    
    template<class _Pr1>
    void remove_if(_Pr1 _Pred)
    {
        for(iterator it = begin(), itEnd = end(); it != itEnd;)
            if(_Pred(*it))
                erase(it++);
            else
                ++it;
    }


    // splice all of _Right at _Where
    void splice(iterator _Where, _Myt& _Right)
    {
        if(this != &_Right && !_Right.empty())
            _Splice(_Where, _Right, _Right.begin(), _Right.end(), _Right._Size);
    }

    // splice _Right [_First] at _Where
    void splice(iterator _Where, _Myt& _Right, iterator _First)
    {
        iterator _Last = _First;
        if(_First != _Right.end() && _Where != _First && _Where != ++_Last)
            _Splice(_Where, _Right, _First, _Last, 1);
    }

    // splice _Right [_First, _Last] at _Where
    void splice(iterator _Where, _Myt& _Right, iterator _First, iterator _Last)
    {    
        if(_First != _Last && _Where != _Last)
        {
            size_type _Count = 0;
            
            if(this != &_Right)
                _Count = std::distance(_First, _Last);
            
            _Splice(_Where, _Right, _First, _Last, _Count);
        }
    }
    
   
    // push_back
    template<class _Val>
    bool push_back(const _Val& Value)
        {return end() != insert(end(), Value); }

    // push_front
    template<class _Val>
    bool push_front(const _Val& Value)
        {return end() != insert(begin(), Value); }

    // pop_back
    void pop_back()
        {erase(--end()); }
        
    // pop_front
    void pop_front()
        {erase(begin()); }
        
    // front
    T& front()
        {return (*begin());    }

    const T& front() const
        {return (*begin());    }

    // back
    T& back()
        {return (*(--end())); }

    const T& back() const
        {return (*(--end())); }

    const allocator_type& get_allocator() const
        {return *this; }

    // CLASS const_iterator
    friend class const_iterator;
    class const_iterator : public std::iterator<std::bidirectional_iterator_tag, T>
    {
    public:
        const_iterator()
            {}
        const_iterator(_NodePtr _P)
            : _Ptr(_P) {}
        const_iterator(const iterator& it)
            : _Ptr(it._Ptr) {}
        const T& operator*() const
            {return _Ptr->_Value; }
        const T* operator->() const
            {return (&**this); }
        const_iterator& operator++()
            {_Ptr = _Ptr->_Next;
            return (*this); }
        const_iterator operator++(int)
            {const_iterator Tmp = *this;
            ++*this;
            return (Tmp); }
        const_iterator& operator--()
            {_Ptr = _Ptr->_Prev;
            return (*this); }
        const_iterator operator--(int)
            {const_iterator Tmp = *this;
            --*this;
            return (Tmp); }
        bool operator==(const const_iterator& it) const
            {return (_Ptr == it._Ptr); }
        bool operator!=(const const_iterator& it) const
            {return (!(*this == it)); }
        _NodePtr _MyNode() const
            {return (_Ptr); }
    protected:
        _NodePtr _Ptr;
    };

    // CLASS iterator
    friend class iterator;
    class iterator : public const_iterator 
    {
    public:
        iterator()
            {}
        iterator(_NodePtr _P)
            : const_iterator(_P) {}
        T& operator*() const
            {return _Ptr->_Value; }
        T* operator->() const
            {return (&**this); }
        iterator& operator++()
            {_Ptr = _Ptr->_Next;
            return (*this); }
        iterator operator++(int)
            {iterator Tmp = *this;
            ++*this;
            return (Tmp); }
        iterator& operator--()
            {_Ptr = _Ptr->_Prev;
            return (*this); }
        iterator operator--(int)
            {iterator Tmp = *this;
            --*this;
            return (Tmp); }
        bool operator==(const iterator& it) const
            {return (_Ptr == it._Ptr); }
        bool operator!=(const iterator& it) const
            {return (!(*this == it)); }
    };

protected:
    _NodePtr _Buynode(_NodePtr WhereNode)
    {
        assert(WhereNode);
        __assume(WhereNode);
        assert(WhereNode->_Prev);
        __assume(WhereNode->_Prev);
        
        if(_NodePtr NewNode = (_NodePtr)allocate(sizeof(_Node)))
        {
            NewNode->_Next = WhereNode;
            NewNode->_Prev = WhereNode->_Prev;
            
            WhereNode->_Prev->_Next = NewNode;
            WhereNode->_Prev = NewNode;
            
            ++_Size;
            
            return NewNode;
        }
        else
            return NULL;
    }
    
    void _Freenode(_NodePtr Node)
        {deallocate(Node, sizeof(_Node)); }

    
    // splice _Right [_First, _Last) before _Where
    void _Splice(iterator _Where, _Myt& _Right, iterator _First, iterator _Last, size_type _Count)
    {    
        if(get_allocator() == _Right.get_allocator())
        {
            if(this != &_Right)
            {
                _Size += _Count;
                _Right._Size -= _Count;
            }
            
            _NodePtr WhereNode = _Where._MyNode();
            _NodePtr FirstNode = _First._MyNode();
            _NodePtr LastNode = _Last._MyNode();
            _NodePtr BeforeWhereNode = WhereNode->_Prev;
            
            FirstNode->_Prev->_Next = LastNode;
            LastNode->_Prev->_Next = WhereNode;
            BeforeWhereNode->_Next = FirstNode;
            WhereNode->_Prev = LastNode->_Prev;
            LastNode->_Prev = FirstNode->_Prev;
            FirstNode->_Prev = BeforeWhereNode;
        }
        else
        {
            insert(_Where, _First, _Last);
            _Right.erase(_First, _Last);
        }
    }

protected:
    _NodePtr        _Head;
    _NodePtr        _HeadNext, _HeadPrev;
    size_type        _Size;
};

}; // namespace ce

#pragma warning(pop)

#endif // __LIST__
