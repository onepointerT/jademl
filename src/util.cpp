
#include "util.hpp"


namespace jademl {

std::string_view until( const char* delim, const std::string str, const std::size_t pos ) {
    std::size_t pos_until = str.find_first_of( delim, pos );
    return str.substr( pos, pos_until - 1 );
}


std::string_view between( const char* delim1, const char* delim2, const std::string str, const std::size_t pos ) {
    std::size_t pos_since = str.find_first_of( delim1, pos );
    std::size_t pos_until = str.find_first_of( delim2, pos_since );
    return str.substr( pos_since + 1, pos_until - 1 );
}

std::list<std::size_t> occurences( const char* delim, const std::string str, const std::size_t pos, const std::size_t pos_until ) {
    std::string substr;
    if ( pos != 0 ) {
        if ( pos_until != std::string::npos ) substr = str.substr( pos, pos_until - pos );
        else substr = str.substr( pos );
    } else {
        if ( pos_until != std::string::npos ) substr = str.substr( 0, pos_until );
        else substr = str;
    }

    size_t pos_next = 0;
    std::list<std::size_t>* lst = new std::list<std::size_t>();
    while( ( pos_next = substr.find_first_of( delim, pos_next ) ) != substr.npos ) {
        lst->push_back( pos_next );
    }

    return *lst;
}

std::string substract_str( const char* str1, const char* str_to_substract ) {
    const std::string mainstr( str1 );
    const std::string substract_str( str_to_substract );

    size_t pos_start_substr = mainstr.find( substract_str );
    size_t pos_end_substr = mainstr.rfind( substract_str );

    if ( pos_start_substr == 0 && pos_end_substr != mainstr.npos )
        return mainstr.substr( pos_end_substr + 1 );
    else if ( pos_start_substr == 0 && pos_end_substr == mainstr.npos )
        return "";
    else if ( pos_end_substr == mainstr.npos )
        return mainstr.substr( 0, pos_start_substr - 1 );
    else
        return mainstr.substr( 0, pos_start_substr - 1 ) + mainstr.substr( pos_end_substr + 1 );
}


} // namespace jademl