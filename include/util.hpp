
#pragma once

#include <list>
#include <memory>
#include <string>
#include <string_view>
#include <utility>


namespace jademl {


std::string_view until( const char* delim, const std::string str, const std::size_t pos = 0 );
std::string_view between( const char* delim1, const char* delim2, const std::string str, const std::size_t pos );
std::list<std::size_t> occurences( const char* delim, const std::string str, const std::size_t pos = 0, const std::size_t pos_until = std::string::npos );

std::string substract_str( const char* str1, const char* str_to_substract );


template< class T >
class SortableList
{
public:
    static T* at( const std::list<T*>* lst, size_t pos ) noexcept {
        if ( pos > lst->size() - 1 ) return nullptr;

        typename std::list<T*>::const_iterator cIt = lst->begin();
        for ( unsigned int p = 0; p < lst->size(); p++ ) {
            if ( p == pos ) return *cIt;
        }

        return nullptr;
    }
};


template< class DerivedT >
class Singleton {
protected:
    static std::unique_ptr< DerivedT > ms_instance;

public:
    static DerivedT* get() {
        if ( ! ms_instance ) {
            ms_instance.reset( new DerivedT() );
        }
        return ms_instance.get();
    }
    static DerivedT& ref() {
        return *(get());
    }
};

template< class DerivedT >
inline std::unique_ptr< DerivedT > Singleton< DerivedT >::ms_instance
    = std::unique_ptr< DerivedT >();

/**
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
**/

} // namespace jademl