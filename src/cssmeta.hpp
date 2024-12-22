
#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace CSS {

class CssClass
    :   public std::string_view
{
public:
    CssClass( const char* clsstr );
};


class CssMetaTag {
public:
    std::vector< CssClass* > classes;
    const std::string id;

    CssMetaTag( const char* id = "" );

    CssMetaTag& operator<<( CssClass& csscls );
    CssMetaTag& operator<<( const std::string classes );
};

} // namespace CSS