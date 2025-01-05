
#pragma once

#include <list>
#include <string>
#include <string_view>

#include "dataset.hpp"
#include "jadeelement.hpp"


namespace jademl {

class JadeElement;
class JadeContent;

class Brackets {
public:
    const std::string opening;
    const std::string closing;

    static const char reverse( const char chr );

    bool valid() const;

    Brackets( const char* openingstr, const char* closingstr );

    enum Type {
        Functional,
        Variable,
        Macro,
        Script,
        generic
    };

    Type type() const;
};


class JinjaTag;


class JinjaContentChildren
    :   public std::list< JinjaTag* >
{
public:
    JinjaContentChildren();

    JinjaTag* at( size_t pos ) const noexcept;

    JinjaContentChildren& operator<<( JinjaTag* jinjatag );
    JinjaContentChildren& operator<<( std::list< JinjaTag* > other_list );
};



class JinjaTagContent
    :   public std::string_view
{
public:
    JinjaContentChildren tag_children;
    std::string_view extra_content;
    JinjaContentChildren extra_children;
    std::string_view closing_tag;
    unsigned int indentation;

    JinjaTagContent( const unsigned int indent = 0 );
    JinjaTagContent( const char* tagstr, const char* extra_content_str = "", const unsigned int indent = 0 );

    JadeElement* toJadeElement() const;
};


class JinjaTag
    :   public Brackets
    ,   public JinjaTagContent
{
public:
    const Brackets::Type type_tag;

    static bool greater( const JinjaTag& lhs, const JinjaTag& rhs ) noexcept;
    static bool greater_ptr( const JinjaTag* lhs, const JinjaTag* rhs ) noexcept;

    JinjaTag( const std::string tagstr );

    JadeElement* toJadeElement() const;
    JadeContent* toJadeContent() const;
    db::DataSet* toJadeMLDataset() const;
};


} // namespace jademl