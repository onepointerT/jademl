
#include "jinja.hpp"

#include "functionmap.hpp"
#include "jadeelement.hpp"
#include "util.hpp"

namespace jademl {


Brackets::Brackets( const char* openingstr, const char* closingstr )
    :   opening( openingstr )
    ,   closing( closingstr )
{}


const char reverse( const char chr ) {
    switch (chr)
    {
    case '{':
        return '}';
    
    case '(':
        return ')';
    
    case '}':
        return '{';

    case ')':
        return '(';
    
    case '+':
        return '+';
    
    case '%':
        return '%';
    
    default:
        return '\0';
    }
}


bool Brackets::valid() const {
    return (
            opening[1] == reverse(closing[0])
        && opening[0] == reverse(closing[1])
        && ( opening == "{%" 
          || opening == "{{"
          || opening == "{("
          || opening == "{+"
        )
    );
}


Brackets::Type Brackets::type() const {
    if ( opening == "{{" ) return Type::Variable;
    else if ( opening == "{%" ) return Type::Macro;
    else if ( opening == "{+" ) return Type::Functional;
    else if ( opening == "{(" ) return Type::Script;
    return Type::generic;
}



JinjaContentChildren::JinjaContentChildren()
    :   std::list< JinjaTag* >()
{}


JinjaTag* JinjaContentChildren::at( size_t pos ) const noexcept {
    return SortableList< JinjaTag >::at( this, pos );
}

JinjaContentChildren& JinjaContentChildren::operator<<( JinjaTag* jinjatag ) {
    push_back( jinjatag );
    sort<>(&JinjaTag::greater_ptr);
    return *this;
}

JinjaContentChildren& JinjaContentChildren::operator<<( std::list< JinjaTag* > other_list ) {
    merge( other_list );
    sort<>(&JinjaTag::greater_ptr);
    return *this;
}



JinjaTagContent::JinjaTagContent( const unsigned int indent )
    :   std::string_view()
    ,   tag_children()
    ,   extra_content()
    ,   extra_children()
    ,   closing_tag()
    ,   indentation( indent )
{}


JinjaTagContent::JinjaTagContent( const char* tagstr, const char* extra_content_str, const unsigned int indent )
    :   std::string_view( tagstr )
    ,   tag_children()
    ,   extra_content( extra_content_str )
    ,   extra_children()
    ,   closing_tag()
    ,   indentation( indent )
{}


/*JadeElement* JinjaTagContent::toJadeElement() const {
    if ( closing_tag.length() > 0 ) { // Only loops, ifs and similar have a closing tag

    } else if ( )
}*/


JinjaTag::JinjaTag( const std::string tagstr )
    :   Brackets( tagstr.substr( 0, 2 ).c_str()
                , tagstr.substr( tagstr.length() - 3, 2 ).c_str() )
    ,   JinjaTagContent( tagstr.c_str() )
    ,   type_tag( Brackets::type() )
{}


JadeElement* JinjaTag::toJadeElement() const {
    // Only loops, ifs and similar have a closing tag
    if ( type_tag == Type::Macro && closing_tag.length() > 0 ) {
        if ( closing_tag == "endfor" ) {
            // TODO: For macro
        } else if ( closing_tag == "endif" ) {
            // TODO: If macro
        }
    } else if ( type_tag == Type::Variable ) {
        if ( contains( "||" ) ) { // Greps database

        } else if ( contains("|") ) { // Has variable parameter list

        } else { // Variable will be substituted by the current graphical environment renderer

        }
    } else if ( type_tag == Type::Functional ) { // Calls a function from functionmap

        db::DataSet* dset = new db::DataSet();
        std::string fname(substr().data());
        std::string function_result = fmap[fname]->call("", dset);

        
        return new JadeElement( indentation, "", function_result );
    }

    return nullptr;
}


} // namespace jademl
