
#pragma once

#include <compare>
#include <list>
#include <string>
#include <HTML/Element.h>

#include "env.hpp"
#include "htmlmeta.hpp"
#include "jinja.hpp"


namespace jademl {

class HtmlBuffer;
class HtmlElement;


class JadeTag
    :   public std::string
{
public:
    JadeTag( const std::string tagname
        , const unsigned int pos_lane_per_tag = 0
        , const unsigned int pos_char_per_lane = 0
    );

    std::string opening_tag( HTML::HtmlMetaElement& tag_parameters ) const;
    std::string closing_tag() const;

    class Position 
        //:   public std::three_way_comparable< Position, std::strong_ordering >
    {
    public:
        const unsigned int positional_lane;
        const unsigned int positional_character;

        Position( const Position& other );
        Position( const unsigned int lane, const unsigned int character );

        Position& operator=( const Position& pos );
        std::strong_ordering operator<=>( const Position& rhs ) const;
    } position;
};


class JadeChildrenTags;


class JadeContent
    :   public std::string
{
public:
    const JadeTag::Position pos;
    JadeChildrenTags* children;

    JadeContent( const JadeContent& other );
    JadeContent( const std::string content_str
        , const unsigned int line_behind_jadetag
        , const unsigned int length_of_jadetag = 0
    );

    std::string toString() const;

    JadeContent& operator=( const JadeContent& rhs );
};


class JadeElement
{
public:
    const unsigned int indent;
    const JadeTag tag;
    HTML::HtmlMetaElement html_meta;
    JadeContent content;
    JadeChildrenTags* children;

    JadeTag::Position* pos_relative_to_parent = nullptr;
    JadeTag::Position* pos_in_doc = nullptr;
    JadeTag::Position* pos_behind_last_elem = nullptr;

    static bool greater( const JadeElement& lhs, const JadeElement& rhs ) noexcept;
    static bool greater_ptr( const JadeElement* lhs, const JadeElement* rhs ) noexcept;

    JadeElement( const unsigned int indent_spaces
               , const std::string tagname
               , const std::string content_str
               , const unsigned int lane = 0
               , const unsigned int length_tag = 0
               , const char* optional_parameters = "" // The HTML meta
    );

    JadeElement& operator<<( JadeElement* jadeelem );
    JadeElement& operator<<( JadeElement&& jadeelem );
    JadeElement& operator<<( std::list< JadeElement* > new_children );
    JadeElement& operator<<( JadeContent* jadecontent );

    std::string toString() const;
    HtmlBuffer* toHtmlDoc( EnvStack* env_variables = nullptr ) const;
    std::string toHtml( EnvStack* env_variables = nullptr ) const;
};



class JadeChildrenTags
    :   public std::list< JadeElement* >
{
public:
    JadeChildrenTags();

    JadeElement* at( size_t pos ) const noexcept;

    JadeChildrenTags& operator<<( JadeElement* jadeeleement );
    JadeChildrenTags& operator<<( std::list< JadeElement* > other_list );

    JadeElement* find_at_pos( const unsigned int line_behind_jadetag
                            , const unsigned int pos_behind_of_jadetag_before );
    std::list< JadeElement* > find_at_line( const unsigned int line );
};


} // namespace jademl