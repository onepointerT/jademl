
#pragma once

#include <HTML/Document.h>
#include <HTML/Element.h>

#include "jadeelement.hpp"


namespace jademl {


class HtmlElement
    :   public HTML::Element
{
public:
    HtmlElement();
    HtmlElement( const char* apName, const char* apContent = "" );

    std::vector< HTML::Element >& children() noexcept;
    const std::vector< HTML::Element >& children() const noexcept;

    HtmlElement& operator<<( const HTML::Element& htmlelem );

};


class HtmlBuffer
    :   public std::stringstream
    ,   public HTML::Document
{
public:
    HtmlBuffer();

    enum Edit {
        Head,
        Body
    } edit = Edit::Head;

    HtmlBuffer& operator<<( std::string&& htmlelem );
    HtmlBuffer& operator<<( HTML::Element&& htmlelem );
    HtmlBuffer& operator<<( JadeElement* jade );
    HtmlBuffer& operator<<( JinjaTag* jinja );
    HtmlBuffer& operator<<( HtmlElement* htmlelement );
    HtmlBuffer& operator<<( std::vector< HTML::Element > htmlchildren );
};


} // namespace jademl