
#include "cssmeta.hpp"

#include <ranges>
#include <string_view>


namespace CSS {

CssClass::CssClass( const char* clsstr )
    :   std::string_view( clsstr )
{}


CssMetaTag::CssMetaTag( const char* idstr )
    :   classes()
    ,   id( idstr )
{}

CssMetaTag& CssMetaTag::operator<<( CssClass& csscls ) {
    classes.push_back( &csscls );
    return *this;
}

CssMetaTag& CssMetaTag::operator<<( const std::string classes ) {
    std::string_view clses( classes.c_str() );
    constexpr std::string_view delim{" "};
    std::ranges::split_view cls_extractor{clses, delim};
    for ( const auto specific_cls : cls_extractor ) {
        *this << *( new CssClass( specific_cls.data() ) );
    }
    return *this;
}

} // namespace CSS
