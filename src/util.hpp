
#pragma once

#include <list>
#include <utility>


namespace jademl {


template< class T >
class SortableType
{
public:
    template< typename sort_t >
    virtual sort_t sorting_value() const = 0
};

template< class Derived >
class SortingType {
protected:
    friend class Derived;
    
public:
    virtual bool operator<( Derived& another ) const = 0;
};

template< typename sort_t, typename T >
class SortableElement
    :   std::pair< sort_t, T >
{
public:
    SortableElement( sort_t sort_type, T& element )
        :   std::pair< sort_t, T >( sort_type, element )
    {}
    SortableElement( SortableType< T >* element )
        :   std::pair< sort_t, T >( element->sorting_value(), *element )
    {}

    T& element() const {
        return second;
    }

    bool operator<( sort_t& another ) const {
        return first < another;
    }
};


template< class sort_t, class T >
class SortedStack {
public:
    typedef SortedStack< sort_t, T > self_t;

protected:



public:
    SortedStack();

    self_t& operator<<( T* element );
    self_t& operator<<( T& element );
};


} // namespace jademl