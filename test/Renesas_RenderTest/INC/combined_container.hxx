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
#ifndef _COMBINED_CONTAINER_INCLUDED_
#define _COMBINED_CONTAINER_INCLUDED_

namespace ce
{

template<class T, class CONTAINER>
class combined_container
{
public:
    class const_iterator;
    class iterator;
    typedef typename CONTAINER::size_type size_type;
    
    combined_container(CONTAINER& container1, CONTAINER& container2)
        : container1(container1),
          container2(container2)
    {
    }
    
    // begin
    iterator begin()
        {return iterator(container1.end(), container2.begin(), container1.begin()); }
    
    const_iterator begin() const
        {return const_iterator(container1.end(), container2.begin(), container1.begin()); }
    
    // end
    iterator end()
        {return iterator(container1.end(), container2.begin(), container2.end()); }
    
    const_iterator end() const
        {return const_iterator(container1.end(), container2.begin(), container2.end()); }
    
    // size
	size_type size() const
		{return container1.size() + container2.size(); }

	// empty
	bool empty() const
		{return size() == 0; }
		
    // operator[]
    const T& operator[](size_type i) const
    {
        if(i < container1.size())
            return container1.operator[](i);
        else
            return container2.operator[](i - container1.size());
    }
    
    T& operator[](size_type i)
    {
        if(i < container1.size())
            return container1.operator[](i);
        else
            return container2.operator[](i - container1.size());
    }
    

    // CLASS const_iterator
	class const_iterator
	{
	public:
		const_iterator()
		    {}
		
		const_iterator(const iterator& it)
		    : itEnd1(it.itEnd1), itBegin2(it.itBegin2), itCurrent(it.itCurrent)
		    {}
		
		const_iterator(typename CONTAINER::const_iterator itEnd1, typename CONTAINER::const_iterator itBegin2, typename CONTAINER::const_iterator it)
			: itEnd1(itEnd1), itBegin2(itBegin2), itCurrent(it)
        {
            if(itCurrent == itEnd1)
                itCurrent = itBegin2;
        }
			
		const T& operator*() const
			{return *itCurrent; }
			
		const T* operator->() const
			{return itCurrent.operator->(); }
			
		const_iterator& operator++()
		{
		    assert(itCurrent != itEnd1);
		    
		    if(++itCurrent == itEnd1)
		        itCurrent = itBegin2;
			
			return (*this);
	    }
	    
		const_iterator operator++(int)
		{
		    assert(itCurrent != itEnd1);
		    
		    const_iterator Tmp = *this;
		    
		    ++*this;
		    
			return Tmp;
	    }
	    
		const_iterator& operator--()
	    {
	        assert(itCurrent != itEnd1 || itEnd1 == itBegin2);
	        
	        if(itCurrent == itBegin2)
	            itCurrent = itEnd1;
	            
	        --itCurrent;
			
			return (*this);
		}
			
		const_iterator operator--(int)
	    {
	        assert(itCurrent != itEnd1 || itEnd1 == itBegin2);
	        
	        const_iterator Tmp = *this;
			
			--*this;
			
			return Tmp;
	    }
			
		bool operator==(const const_iterator& it) const
			{return itCurrent == it.itCurrent; }
			
		bool operator!=(const const_iterator& it) const
			{return itCurrent != it.itCurrent; }
			
	protected:
		typename CONTAINER::const_iterator itEnd1, itBegin2, itCurrent;
	};
	
	// CLASS iterator
	class iterator
	{
	public:
		friend class const_iterator;
		
		iterator()
		    {}
		
		iterator(typename CONTAINER::iterator itEnd1, typename CONTAINER::iterator itBegin2, typename CONTAINER::iterator it)
			: itEnd1(itEnd1), itBegin2(itBegin2), itCurrent(it)
        {
            if(itCurrent == itEnd1)
                itCurrent = itBegin2;
        }
			
		T& operator*() const
			{return *itCurrent; }
			
		T* operator->() const
			{return itCurrent.operator->(); }
			
		iterator& operator++()
		{
		    assert(itCurrent != itEnd1);
		    
		    if(++itCurrent == itEnd1)
		        itCurrent = itBegin2;
			
			return (*this);
	    }
	    
		iterator operator++(int)
		{
		    assert(itCurrent != itEnd1);
		    
		    iterator Tmp = itCurrent;
		    
		    ++*this;
		    
			return Tmp;
	    }
	    
		iterator& operator--()
	    {
	        assert(itCurrent != itEnd1);
	        
	        if(itCurrent == itBegin2)
	            itCurrent = itEnd1;
	            
	        --itCurrent;
			
			return (*this);
		}
			
		iterator operator--(int)
	    {
	        assert(itCurrent != itEnd1);
	        
	        iterator Tmp = itCurrent;
			
			--*this;
			
			return Tmp;
	    }
			
		bool operator==(const iterator& it) const
			{return itCurrent == it.itCurrent; }
			
		bool operator!=(const iterator& it) const
			{return itCurrent != it.itCurrent; }
	
	protected:
		typename CONTAINER::iterator itEnd1, itBegin2, itCurrent;
	};
	
protected:
    CONTAINER& container1;
    CONTAINER& container2;
};

}; // namespace ce

#endif // _COMBINED_CONTAINER_INCLUDED_
