
#include "htmlbuf.hpp"



namespace jademl {


HtmlElement::HtmlElement()
    :   HTML::Element() 
{}

HtmlElement::HtmlElement( const char* apName, const char* apContent )
    :   HTML::Element( apName, apContent )
{}

std::vector< HTML::Element >& HtmlElement::children() noexcept {
    return mChildren;
}

const std::vector< HTML::Element >& HtmlElement::children() const noexcept {
    return mChildren;
}

HtmlElement& HtmlElement::operator<<( const HTML::Element& htmlelem ) {
    mChildren.push_back( htmlelem );
    return *this;
}


HtmlBuffer::HtmlBuffer()
    :   HTML::Document()
{}


HtmlBuffer& HtmlBuffer::operator<<( Element&& htmlelem ) {
    if ( this->edit == Edit::Body )
        this->body() << std::move( htmlelem );
    else if ( this->edit == Edit::Head )
        this->head() << std::move( htmlelem );
    else *this << htmlelem.toString();

    return *this;
}


HtmlBuffer& HtmlBuffer::operator<<( JadeElement* jade ) {
    HTML::Element* htmlelem = new HTML::Element( jade->tag.c_str(), jade->content );
    for ( unsigned int i = 0; i < jade->html_meta.elements.size(); i++ ) {
        HTML::HtmlMetaElementPair* elem = jade->html_meta.elements.at( i );
        if ( elem->key != "id" ) {
            htmlelem->addAttribute( elem->key.c_str(), elem->value );
        } else if ( elem->key != "id" ) {
            htmlelem->id( elem->value );
        }
    }
    *this << std::move( *htmlelem );
    return *this;
}

HtmlBuffer& HtmlBuffer::operator<<( JinjaTag* jinja ) {
    //std::string_view jinjastrv( jinja)
    
    return *this;
}

} // namespace jademl