
#pragma once

#include <list>
#include <string>

#include "htmlmeta.hpp"
#include "jinja.hpp"
#include "util.hpp"


namespace jademl {


class JadeTag
    :   public std::string
{
public:
    JadeTag( const std::string tagname )
        :   std::string( tagname )
    {}

};


class JadeContent
    :   public std::string
{
public:
    JadeContent( const std::string content_str )
        :   std::string( content_str )
    {}
};


class JadeElement
{
public:
    const unsigned int indent;
    const JadeTag tag;
    const HTML::HtmlMetaElement html_meta;
    JadeContent content;

    JadeElement( unsigned int indent_spaces
               , const std::string tagname
               , const std::string content_str
               , const char* optional_parameters = "" // The HTML meta
    )
        :   indent( indent_spaces )
        ,   tag( tagname )
        ,   html_meta( optional_parameters == "" ? nullptr : HTML::HtmlMetaElement(optional_parameters) )
        ,   content( content_str )
    {}
};


} // namespace jademl