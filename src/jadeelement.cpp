
#include "jadeelement.hpp"

#include "cssmeta.hpp"
#include "htmlbuf.hpp"
#include "util.hpp"


namespace jademl {

JadeTag::JadeTag( const std::string tagname, const unsigned int pos_lane_per_tag
        , const unsigned int pos_char_per_lane
)   :   std::string( tagname )
    ,   position( pos_lane_per_tag, pos_char_per_lane )
{}

std::string JadeTag::opening_tag( HTML::HtmlMetaElement& tag_parameters ) const {
    return "<" + *this + " " + tag_parameters.toString() + ">";
}

std::string JadeTag::closing_tag() const {
    return "</" + *this + ">";
}


JadeTag::Position::Position( const Position& other )
    :   positional_lane( other.positional_lane )
    ,   positional_character( positional_character )
{}

JadeTag::Position::Position( const unsigned int lane, const unsigned int character )
    :   positional_lane( lane )
    ,   positional_character( character )
{}

JadeTag::Position& JadeTag::Position::operator=( const Position& pos ) {
    Position* position_new = new Position( pos );
    return *position_new;
}

std::strong_ordering JadeTag::Position::operator<=>( const Position& rhs ) const {
    if ( rhs.positional_lane < positional_lane ) return std::strong_ordering::greater;
    else if ( rhs.positional_lane > positional_lane  ) return std::strong_ordering::less;
    else if ( rhs.positional_character < positional_character ) return std::strong_ordering::greater;
    else if ( rhs.positional_character > positional_character ) return std::strong_ordering::less;
    else return std::strong_ordering::equivalent;
}



JadeContent::JadeContent( const JadeContent& other )
    :   std::string( other )
    ,   pos( other.pos )
    ,   children( new JadeChildrenTags() )
{}

JadeContent::JadeContent( const std::string content_str
        , const unsigned int line_behind_jadetag
        , const unsigned int length_of_jadetag
)   :   std::string( content_str )
    ,   pos( line_behind_jadetag, length_of_jadetag )
    ,   children( new JadeChildrenTags() )
{}

std::string JadeContent::toString() const {
    return *this;
}

JadeContent& JadeContent::operator=( const JadeContent& rhs ) {
    JadeContent* jadecontent_new = new JadeContent( rhs );
    return *jadecontent_new;
}



JadeElement::JadeElement( unsigned int indent_spaces
               , const std::string tagname
               , const std::string content_str
               , const unsigned int lane
               , const unsigned int length_tag
               , const char* optional_parameters // The HTML meta
)
    :   indent( indent_spaces )
    ,   tag( tagname )
    ,   html_meta( optional_parameters )
    ,   content( content_str, lane, length_tag )
    ,   children( new JadeChildrenTags() )
{}


bool JadeElement::greater( const JadeElement& lhs, const JadeElement& rhs ) noexcept {
    if ( (*lhs.pos_in_doc <=> *rhs.pos_in_doc) == std::strong_ordering::greater )
        return true;
    else if ( (*lhs.pos_relative_to_parent <=> *rhs.pos_relative_to_parent) == std::strong_ordering::greater )
        return true;
    else if ( (*lhs.pos_behind_last_elem <=> *rhs.pos_behind_last_elem) == std::strong_ordering::greater )
        return true;
    return false;
}

bool JadeElement::greater_ptr( const JadeElement* lhs, const JadeElement* rhs ) noexcept {
    return JadeElement::greater( *lhs, *rhs );
}


JadeElement& JadeElement::operator<<( JadeElement* jadeelem ) {
    if ( jadeelem->indent > indent ) {
        children->push_back( jadeelem );
    }
    return *this;
}


JadeElement& JadeElement::operator<<( JadeElement&& jadeelem ) {
    *this << &jadeelem;
    return *this;
}

JadeElement& JadeElement::operator<<( std::list< JadeElement* > new_children ) {
    this->children->merge( new_children );
    return *this;
}

JadeElement& JadeElement::operator<<( JadeContent* jadecontent ) {
    this->content = *jadecontent;
    return *this;
}

HtmlBuffer* JadeElement::toHtmlDoc( EnvStack* env_variables ) const {
    HtmlBuffer* htmlbuf = new HtmlBuffer();

    HTML::Element* htmlelem;
    if ( tag.compare( 0, 1, "#" ) == 0 ) {
        std::string div_id( tag.substr( 1 ) );
        std::string div_clses;
        
        CSS::CssMetaTag* css = new CSS::CssMetaTag( div_id.c_str() );
        *css << div_clses.c_str();

        htmlelem = new HTML::Div(  );
    }

    // #TODO

    return htmlbuf;
}

std::string JadeElement::toHtml( EnvStack* env_variables ) const {
    std::string html = "<" + tag + " " + html_meta.toString() + ">";
    html += content + tag.closing_tag();
    return html;
}



JadeChildrenTags::JadeChildrenTags()
    :   std::list< JadeElement* >()
{}


JadeElement* JadeChildrenTags::at( size_t pos ) const noexcept {
    return jademl::SortableList<JadeElement>::at( this, pos );
}

JadeChildrenTags& JadeChildrenTags::operator<<( JadeElement* jadeelement ) {
    push_back( jadeelement );
    sort<>(&JadeElement::greater_ptr);
    return *this;
}

JadeChildrenTags& JadeChildrenTags::operator<<( std::list< JadeElement* > other_list ) {
    merge( other_list );
    sort<>(&JadeElement::greater_ptr);
    return *this;
}


JadeElement* JadeChildrenTags::find_at_pos( const unsigned int line_behind_jadetag
                            , const unsigned int pos_behind_of_jadetag_before
) {
    for ( unsigned int jc = 0; jc < size(); jc++ ) {
        JadeElement* jadeelement = at( jc );
        if ( jadeelement->pos_relative_to_parent->positional_lane == line_behind_jadetag
          && jadeelement->pos_relative_to_parent->positional_character == pos_behind_of_jadetag_before
        ) {
            return jadeelement;
        }
    }
    return nullptr;
}

std::list< JadeElement* > JadeChildrenTags::find_at_line( const unsigned  int line ) {
    std::list< JadeElement* >* lst = new std::list< JadeElement* >();
    for ( unsigned int jc = 0; jc < size(); jc++ ) {
        JadeElement* jadeelement = at( jc );
        if ( jadeelement->pos_relative_to_parent->positional_lane == line
          || jadeelement->pos_in_doc->positional_lane == line
        ) {
            lst->push_back( jadeelement );
        }
    }
    return *lst;
}



} // namespace jademl
