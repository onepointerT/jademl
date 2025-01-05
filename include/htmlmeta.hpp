
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
    static std::string pre_equiv( const char* elementstr );
    static std::string post_equiv( const char* elementstr );

public:
    const std::string key;
    const std::string value;

    HtmlMetaElementPair( const char* elementstr );
    HtmlMetaElementPair( const char* keystr, const char* valuestr );
    
    enum Type {
        id,
        css,
        metatag
    } const type;

    std::string toString() const;
};


class HtmlMetaElement
    :   public std::string
{
protected:

public:
    std::vector< HtmlMetaElementPair* > elements;
    CSS::CssMetaTag* css;

    HtmlMetaElement( const char* meta_elements_str );

    std::string toString() const;
};

} // namespace HTML