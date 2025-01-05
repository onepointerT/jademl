
#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <utility>

#include "jadeelement.hpp"
#include "jinja.hpp"


namespace HTML {
class HtmlMetaElementPair;
} // namespace HTML


namespace jademl {

namespace syntax {


unsigned int count( const char* str, const char* countstr );
unsigned int countuntil( const char* str, const char* countstr, const unsigned int pos_until );
unsigned int countprefix( const char* str, const char* prefix );
std::string_view removeindent( const char* str );
bool is_in_quotation_marks( const char* str, const unsigned int pos );

namespace jade {



std::string findCssClasses( const char* str, JadeElement* jade);
std::string findHtmlId( const char* str, JadeElement* jade );
std::vector< HTML::HtmlMetaElementPair* > findHtmlAttributes( const char* str, JadeElement* jade );
std::string findJadeContent( const char* str, JadeElement* jade );
std::list< JadeElement* > findJadeElementsInContent( JadeContent* jadecontent, JadeElement* jadeparent = nullptr );
JadeElement* findFullElement( const char* str, const std::size_t pos_start = 0 );
JadeElement* findAllElements( const char* str );
JadeElement* findAllElements( JadeElement* jadeelement );
JadeContent* makeJadeContentFromStringLane( const char* str, const unsigned int length_jadetag, const unsigned int lines_behind_jadetag = 0 );
JadeContent* makeJadeContentFromStringMultiline( const char* str, const unsigned int length_jadetag, const unsigned int lines_behind_jadetag = 0 );

std::list< JadeElement* > inspectMultilineStr( const char* str );

} // namespace jade

namespace jinja {

JinjaTag* findJinjaMacro( const char* str, size_t pos_start = 0 );


} // nameapace jinja
} // namespace syntax
} // namespace jademl
