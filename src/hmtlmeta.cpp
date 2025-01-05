
#include "htmlmeta.hpp"

#include <ranges>
#include <string_view>

#include "cssmeta.hpp"
#include "util.hpp"


namespace HTML {

std::string HtmlMetaElementPair::pre_equiv( const char* elementstr ) {
    std::string str( elementstr );
    size_t pos_equiv = str.find_first_of( "=" ) - 1;
    return str.substr( 0, pos_equiv );
}

std::string HtmlMetaElementPair::post_equiv( const char* elementstr ) {
    std::string str( elementstr );
    size_t pos_equiv = str.find_first_of( "=" ) + 1;
    return str.substr( pos_equiv + 1, str.length() - pos_equiv - 2 );
}

HtmlMetaElementPair::HtmlMetaElementPair( const char* elementstr )
    :   key( pre_equiv( elementstr ) )
    ,   value( post_equiv( elementstr ) )
    ,   type( key == "id" ? Type::id : key == "class" ? Type::css : Type::metatag )
{}

HtmlMetaElementPair::HtmlMetaElementPair( const char* keystr, const char* valuestr )
    :   key( keystr )
    ,   value( valuestr )
    ,   type( key == "id" ? Type::id : key == "class" ? Type::css : Type::metatag )
{}

std::string HtmlMetaElementPair::toString() const {
    return key + "=\"" + value + "\"";
}


HtmlMetaElement::HtmlMetaElement( const char* meta_elements_str )
    :   elements()
    ,   css()
{
    std::string_view meta_tags( meta_elements_str );
    constexpr std::string_view delim{"\" "};
    std::ranges::split_view meta_tag_extractor{meta_tags, delim};
    bool id_found = false;
    std::string css_clses;
    for ( const auto specific_meta_tag : meta_tag_extractor ) {
        std::string_view meta_tag_strv( specific_meta_tag.data() );
        if ( meta_tag_strv.starts_with("id") && !id_found ) {
            this->css = new CSS::CssMetaTag( specific_meta_tag.data() );
            HtmlMetaElementPair* htmlmeta = new HtmlMetaElementPair( specific_meta_tag.data() );
            this->elements.push_back( htmlmeta );
        } else if ( meta_tag_strv.starts_with("class") ) {
            css_clses = meta_tag_strv;
            HtmlMetaElementPair* htmlmeta = new HtmlMetaElementPair( specific_meta_tag.data() );
            this->elements.push_back( htmlmeta );
        } else if ( ! meta_tag_strv.starts_with("id") ) {
            HtmlMetaElementPair* htmlmeta = new HtmlMetaElementPair( specific_meta_tag.data() );
            this->elements.push_back( htmlmeta );
        }
    }

    if ( this->css != nullptr ) {
        *this->css << css_clses;
    }
}

std::string HtmlMetaElement::toString() const {
    std::string metastr;
    for ( unsigned int i = 0; i < elements.size(); i++ ) {
        HtmlMetaElementPair* htmlmeta = elements.at( i );
        metastr += " " + htmlmeta->toString();
    }
    return metastr;
}


} // namespace HTML
