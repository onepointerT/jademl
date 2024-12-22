
#pragma once

#include <string>
#include <string_view>

#include "dataset.hpp"
#include "jadeelement.hpp"


namespace jademl {


class Brackets {
public:
    const std::string opening;
    const std::string closing;

    bool valid() const;

    Brackets( const char* openingstr, const char* closingstr );

    enum Type {
        Functional,
        Variable,
        Macro
    };

    Type type() const;
};


class JinjaTagContent
    :   public std::string_view
{
public:
    const std::string_view extra_content;

    JinjaTagContent();
};


class JinjaTag
    :   public Brackets
    ,   public JinjaTagContent
{
public:
    const Brackets::Type type;

    JinjaTag( const char* tagstr );

    JadeElement* toJadeElement() const;
    JadeContent* toJadeContent() const;
    db::DataSet* toJadeMLDataset() const;
};


} // namespace jademl