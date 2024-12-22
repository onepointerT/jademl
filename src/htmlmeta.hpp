
#pragma once

#include <string>
#include <vector>
#include <utility>


namespace CSS {
class CssMetaTag;
} // namespace CSS

namespace HTML {


class HtmlMetaElementPair
{
public:
    const std::string key;
    const std::string value;

    HtmlMetaElementPair( const char* elementstr );
    
    enum Type {
        id,
        css,
        metatag
    } const type;
};


class HtmlMetaElement
    :   public std::string
{
protected:
    std::vector< HtmlMetaElementPair* > m_elements;

public:
    CSS::CssMetaTag* css;

    HtmlMetaElement( const char* meta_elements_str );
};

} // namespace HTML