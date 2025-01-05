
#include "syntax.hpp"

#include <ranges>
#include <string>
#include <vector>

#include "cssmeta.hpp"
#include "regex.hpp"


namespace jademl {
namespace syntax {

unsigned int count( const char* str, const char* countstr ) {
    unsigned int i = 0;
    size_t pos = 0;
    std::string_view strv( str );
    while ( ( pos = strv.find_first_of( countstr, pos ) ) != strv.npos ) {
        ++i;
    }
    return i;
}

unsigned int countuntil( const char* str, const char* countstr, const unsigned int pos_until ) {
    unsigned int i = 0;
    size_t pos = 0;
    std::string_view strv( str );
    while ( ( pos = strv.find_first_of( countstr, pos ) ) != strv.npos ) {
        if ( pos > pos_until ) break;
        ++i;
    }
    return i;
}

unsigned int countprefix( const char* str, const char* prefix ) {
    unsigned int i = 0;
    size_t pos = 0;
    size_t pos_before = 0;
    std::string_view strv( str );
    bool all_prefix = true;
    std::string prefixstr( prefix );
    while ( all_prefix
         && ( pos = strv.find_first_of( prefix, pos, prefixstr.length() ) ) != strv.npos )
    {
        ++i;
        if ( pos != pos_before + prefixstr.length() ) {
            all_prefix = false;
        } else {
            pos_before = pos;
        }
    }
    return i;
}

std::string_view removeindent( const char* str ) {
    unsigned int indent = countprefix( str, " " );
    std::string_view strv( str );
    strv.remove_prefix( indent );
    return strv;
}

bool is_in_quotation_marks( const char* str, const unsigned int pos ) {
    std::string_view jadecontent = str;
    size_t pos_opening_citation_mark = jadecontent.rfind( "=\"", pos );
    size_t pos_closing_citation_mark = jadecontent.find_first_of( "\"", pos );
    if ( pos_opening_citation_mark < jadecontent.rfind( '\"' ) ) return false;
    else if ( pos_opening_citation_mark == jadecontent.npos
            || pos_closing_citation_mark == jadecontent.npos
    ) return false;
    return true;
};


namespace jade {


std::string findCssClasses( const char* str, JadeElement* jade ) {
    std::string_view strv = syntax::removeindent( str );
    std::string clses;
    if ( syntax::countprefix( str, "class=" ) == 1 ) {
        strv.remove_prefix( 6 );
        strv.remove_prefix( 1 );
        strv.remove_suffix( 1 );
        clses = strv.data();
    } else if ( syntax::count( str, "." ) > 0 ) {
        unsigned int cls_count = syntax::count( str, "." );
        bool brackets_reached = false;
        bool first_space_reached = false;

        size_t pos_brackets = strv.find_first_of("(");
        size_t pos_first_space = strv.find_first_of(" ");

        auto still_css = [strv,pos_brackets,pos_first_space](unsigned int c, unsigned int cpos) {
            size_t pos_end_css = pos_first_space;
            if ( pos_brackets < pos_first_space ) pos_end_css = pos_brackets;
            size_t pos_next_dot = strv.find_first_of(".", cpos);
            if ( pos_next_dot > pos_end_css ) return strv.npos;
            return pos_next_dot;
        };

        std::string html_id = findHtmlId( str, jade );
        auto setup_csscls = [jade, html_id](const std::string_view current_cls, std::string result) {
            CSS::CssClass* css = new CSS::CssClass( current_cls.data() );
            if ( jade->html_meta.css == nullptr ) {
                jade->html_meta.css = new CSS::CssMetaTag( html_id.c_str() );
            }
            jade->html_meta.css->classes.push_back( css );
            result += ' ' + current_cls.data();
        };

        unsigned int c = 0;
        unsigned int cpos = strv.find_first_of( "." );
        size_t pos_next_dot;
        while ( ( pos_next_dot = still_css(c, cpos) ) !=  strv.npos ) {
            c++;
            cpos = pos_next_dot;
            std::string_view current_cls = strv.substr( cpos+1, pos_next_dot - cpos - 2 );
            setup_csscls( current_cls, clses );
            if ( c > cls_count ) break;
        }
    }
    return clses;
}

std::string findHtmlId( const char* str, JadeElement* jade ) {
    std::string_view strv = syntax::removeindent( str );
    size_t pos_brackets = strv.find_first_of("(");
    size_t pos_space = strv.find_first_of(" ");
    size_t pos_hashtag = strv.find_first_of("#");

    auto setup_id = [jade](const std::string_view idstrv) {
        jade->html_meta = idstrv.data();
    };

    size_t pos_end_id = strv.find_first_of(".");
    if ( pos_end_id != strv.npos && pos_end_id < pos_space ) {
        setup_id(strv.substr(pos_hashtag+1, pos_end_id-pos_hashtag-2));
    } else if ( pos_end_id == strv.npos ) {
        size_t pos_start_id = strv.find_first_of( "id=\"" ) + 4;
        pos_end_id = strv.find_first_of( "\"", pos_start_id );
        setup_id(strv.substr( pos_start_id, pos_end_id - pos_start_id - 1 ));
    } else if ( pos_hashtag != strv.npos && pos_hashtag < pos_space && pos_hashtag < pos_brackets ) { 
        if ( pos_space < pos_brackets ) pos_end_id = pos_space - 1;
        else pos_end_id = pos_brackets - 1;
        setup_id(strv.substr( pos_hashtag+1, pos_end_id ));
    }

    return jade->html_meta.c_str();
}

std::vector< HTML::HtmlMetaElementPair* > findHtmlAttributes( const char* str, JadeElement* jade ) {
    std::vector< HTML::HtmlMetaElementPair* > results;
    std::string_view strv( str );

    size_t pos_next_elem = strv.find_first_of("(");
    size_t pos_leaving_bracket = strv.find_first_of(")");

    if ( pos_next_elem == strv.npos || pos_leaving_bracket == strv.npos )
        return results;

    auto setup_htmlmeta_elem = [jade,results,strv,pos_leaving_bracket](size_t pos_last_elem, std::vector< HTML::HtmlMetaElementPair* > vec) {

        if ( pos_last_elem == strv.npos ) return strv.npos;

        size_t pos_start_elem = strv.find_first_of( " ", pos_last_elem );
        if ( pos_start_elem == strv.npos || pos_start_elem > pos_leaving_bracket )
            return strv.npos;
        ++pos_start_elem;
        size_t pos_end_elem = strv.find_first_of( " ", pos_start_elem );
        if ( pos_end_elem == strv.npos ) {
            pos_end_elem = pos_leaving_bracket - 1;
        } else {
            --pos_end_elem;
        }

        std::string_view elem_only = strv.substr( pos_start_elem, pos_end_elem );

        size_t pos_equiv = elem_only.find_first_of( "=", pos_start_elem );
        bool key_only = false;
        if ( pos_equiv == elem_only.npos ) key_only = true;

        HTML::HtmlMetaElementPair* htmlmetaelem = nullptr;
        if  ( ! key_only ) {
            std::string key;
            std::string value;
            if ( pos_equiv > 0 ) key = elem_only.substr( 0, pos_equiv-1 ).data();
            if ( pos_equiv < elem_only.npos ) value = elem_only.substr( pos_equiv + 2, pos_end_elem - 1 - pos_equiv ).data();
                
            htmlmetaelem
                = new HTML::HtmlMetaElementPair( key.c_str(), value.c_str() );
        } else {
            htmlmetaelem
                = new HTML::HtmlMetaElementPair( elem_only.data(), "" );
        }
        
        vec.push_back( htmlmetaelem );

        pos_last_elem = pos_end_elem;
        return pos_last_elem;
    };

    while ( ( pos_next_elem = setup_htmlmeta_elem(pos_next_elem+1, results) != strv.npos ) ) {
        if ( pos_next_elem > pos_leaving_bracket ) break;
    }

    jade->html_meta.elements = results;

    return results;
}

std::string findJadeContent( const char* str, JadeElement* jade ) {
    std::string_view strv( str );
    std::string result;

    size_t pos_closing_tag = strv.find_first_of(")");
    if ( pos_closing_tag == strv.npos ) {
        pos_closing_tag = strv.find_first_of(" ");
    }

    const std::string_view strv_content = strv.substr( pos_closing_tag + 1 );
    size_t count_newlines = syntax::countuntil( strv.data(), "\n", pos_closing_tag );

    if ( strv_content.find_first_of("\n") != strv_content.npos ) {
        *jade << makeJadeContentFromStringMultiline( strv_content.data(), pos_closing_tag, count_newlines );
    } else {
        *jade << makeJadeContentFromStringLane( strv_content.data(), pos_closing_tag, count_newlines );
    }

    result = jade->content;

    std::list< JadeElement* > new_children
        = findJadeElementsInContent( &jade->content, jade );
    *jade << new_children;

    return result;
}


std::list< JadeElement* > findJadeElementsInContent( JadeContent* jadecontent, JadeElement* jadeparent ) {

    unsigned int count_hashtags = syntax::count( jadecontent->c_str(), "#" );
    unsigned int count_opening_brackets = syntax::count( jadecontent->c_str(), "(" );

    

    std::list< JadeElement* >* lst = new std::list< JadeElement* >();

    // Extract Tags
    std::list<std::size_t> lst_hashtags = jademl::occurences( "#", *jadecontent );
    std::list<std::size_t> lst_brackets = jademl::occurences( "(", *jadecontent );

    typename std::list<std::size_t>::const_iterator posIt = lst_hashtags.begin();
    if ( posIt != lst_hashtags.end() ) {
        for ( unsigned int pht = 0; pht < lst_hashtags.size(); pht++ ) {

            JadeElement* all_until_content_str
                = findFullElement( jadecontent->data(), *posIt );

            const unsigned int count_indent = syntax::countprefix( jadecontent->c_str(), " " );

            JadeElement* jadeelement
                = new JadeElement( count_indent
                                 , all_until_content_str->toString().c_str()
                                 , jademl::substract_str( jadecontent->c_str(), all_until_content_str->toString().c_str() )
                                 , syntax::countuntil( jadecontent->c_str(), "\n", *posIt )
                                 , all_until_content_str->toString().length()
            );
            lst->push_back( jadeelement );

            ++posIt;
        }
    }

    posIt = lst_brackets.begin();
    if ( posIt != lst_brackets.end() ) {
        for ( unsigned int pht = 0; pht < lst_hashtags.size(); pht++ ) {


            JadeElement* all_until_content_str
                = findFullElement( jadecontent->data(), *posIt );

            const unsigned int count_indent = syntax::countprefix( jadecontent->c_str(), " " );

            JadeElement* jadeelement
                = new JadeElement( count_indent
                                 , all_until_content_str->toString().c_str()
                                 , jademl::substract_str( jadecontent->c_str(), all_until_content_str->toString().c_str() )
                                 , syntax::countuntil( jadecontent->c_str(), "\n", *posIt )
                                 , all_until_content_str->toString().length()
            );
            lst->push_back( jadeelement );

            ++posIt;
        }
    }

    if ( jadeparent != nullptr )
        *jadeparent << *lst;

    return *lst;
}


JadeElement* findFullElement( const char* str, const std::size_t pos_start ) {

    std::string jadecontent = str;
    JadeElement* jadeelement = nullptr;

    std::string_view jadecontent_no_indent
        = regex::no_indent.match( jadecontent.c_str() );

    unsigned int prefix_indent = syntax::countprefix( jadecontent.c_str(), " " );

    Match( jadecontent.c_str(), jadecontent_ml, regex::multiline_text )

    size_t pos_closing_bracket = jadecontent.find_first_of( ")", pos_start );
    size_t pos_opening_bracket = pos_closing_bracket;
    if ( pos_closing_bracket != jadecontent.npos ) {
        pos_opening_bracket = jadecontent.rfind( "(", pos_closing_bracket );

        std::string bracket_str = jadecontent.substr( pos_opening_bracket, pos_closing_bracket-pos_opening_bracket );
        Match( jadecontent_ml.data(), jadetag_name, regex::JADE::jade_tag_name )
        Match( jadecontent_ml.data(), html_id_no_css, regex::JADE::html_id_no_css )
        Match( jadecontent_ml.data(), css_clses, regex::CSS::css_clses_dotted )
        
        std::string ml_content_str = jadecontent_ml.substr( pos_closing_bracket+2 ).data();
        Match( ml_content_str.c_str(), content_str, regex::multiline_text )

        jadeelement
            = new JadeElement( prefix_indent, jadetag_name.data(), content_str.data() );
        jadeelement->html_meta.css = new CSS::CssMetaTag( html_id_no_css.data() );
        *jadeelement->html_meta.css << css_clses.data();
        jadeelement->html_meta = html_id_no_css.data();

    } else if ( pos_closing_bracket == pos_opening_bracket ) { // No braces, do it with html id and css or jinja.

        Match( jadecontent_ml.data(), match_id_css, regex::JADE::html_id_with_css )

        if ( match_id_css.length() > 0 ) {
            Match( match_id_css.data(), html_id, regex::no_indent )
            Match( html_id.data(), html_id_no_css, regex::JADE::html_id_no_css )
            Match( match_id_css.data(), css_clses, regex::CSS::css_clses_dotted )

            jadeelement
                = new JadeElement( prefix_indent, "", "" );
            jadeelement->html_meta.css = new CSS::CssMetaTag( html_id_no_css.data() );
            *jadeelement->html_meta.css << css_clses.data();
            jadeelement->html_meta = html_id_no_css.data();
            findJadeContent( jadecontent_ml.data(), jadeelement );
        }
    }
    
    return jadeelement;
}


JadeElement* findAllElements( const char* str ) {

    std::string jadestr = str;

    Matchall( str, matches_jade_tags, regex::JADE::jade_tag_with_content )
    
    JadeElement* jaderoot = nullptr;
    std::list< JadeElement* > lst;
    for ( typename Regex::match_iterator mIt = matches_jade_tags.begin()
        ; mIt != matches_jade_tags.end(); ++mIt
    ) {
        const std::string_view match = *mIt;

        JadeElement* jadeelement = findFullElement( match.data() );
        lst.push_back( jadeelement );

        if ( jadeelement->indent == 0 ) jaderoot = jadeelement;
    }

    if ( jaderoot == nullptr ) {
        unsigned int size_indent = 0;
        for ( JadeElement* jadeelement : lst ) {
            if ( jadeelement->indent > size_indent ) size_indent = jadeelement->indent;
        }
        for ( JadeElement* jadeelement : lst ) {
            if ( jadeelement->indent < size_indent ) {
                jaderoot = jadeelement;
                size_indent = jadeelement->indent;
            }
        }
    }

    if ( jaderoot != nullptr )
        *jaderoot << lst;
    
    return jaderoot;
}


JadeElement* findAllElements( JadeElement* jadeelement ) {

    std::list< JadeElement* > lst
        = findJadeElementsInContent( &jadeelement->content, jadeelement );

    return jadeelement;

}


} // namespace jade

namespace jinja {


JinjaTag* findJinjaMacro( const char* str, size_t pos_start ) {

    std::string_view strv = str;

    size_t pos_next_nl = strv.find_first_of( '\n', pos_start );
    if ( pos_next_nl == strv.npos ) return nullptr;
    
    std::string macro_opener = strv.substr( pos_start, pos_next_nl - pos_start - 1 ).data();
    unsigned int indent = syntax::countprefix( strv.data(), " " );

    Match( macro_opener.c_str(), macro_opener_str, regex::JINJA::brace_delimiters )

    

}



} // namespace jinja
} // namespace syntax
} // namespace jademl
