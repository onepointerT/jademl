

#pragma once


#include <functional>
#include <map>
#include <string>

#include "dataset.hpp"


namespace jademl {


class StaticFunction
    :   public std::function< bool(std::string, db::DataSet*) >
{
public:
    StaticFunction();

    bool operator()( std::string basename, db::DataSet* parameters );
};


class FunctionMap
    :   std::map< const std::string, StaticFunction* >
{
public:

};



} // namespace jademl