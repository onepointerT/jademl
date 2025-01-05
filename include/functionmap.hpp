

#pragma once


#include <functional>
#include <map>
#include <memory>
#include <string>

#include "dataset.hpp"
#include "util.hpp"


namespace jademl {


class StaticFunction
    :   public std::function< std::string(std::string, db::DataSet*) >
{
public:
    StaticFunction( std::string(*func)(std::string, db::DataSet*) );

    std::string call( std::string basename, db::DataSet* parameters );
};


class FunctionMap
    :   public std::map< std::string, StaticFunction* >
    ,   public Singleton< FunctionMap >
{
protected:

public:
    FunctionMap();
};


#define fmap FunctionMap::ref()
#define fmapptr FunctionMap::get()


} // namespace jademl