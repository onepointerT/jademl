
#pragma once

#include <list>
#include <string>

#include "util.hpp"


namespace jademl {

namespace db {
class DataSet;
} // namespace db


class Variable {
public:
    const std::string key;
    std::string value;

    Variable( const char* keystr, const char* valuestr = "" );
};


class EnvStack
    :   public std::list< Variable* >
{
protected:


public:
    EnvStack();

    db::DataSet* toDataSet() const;
};


class VarEnvironment
    :   public EnvStack
    ,   public Singleton< VarEnvironment >
{
public:
    VarEnvironment();
};

#define env VarEnvironment::ref()
#define envptr VarEnvironment::get()


} // namespace jademl


namespace std {

template<>
struct greater< jademl::Variable > {

bool operator()( const jademl::Variable& lhs, const jademl::Variable& rhs ) {
    return lhs.key < rhs.key;
}

bool operator()( const jademl::Variable* lhs, const jademl::Variable* rhs ) {
    return this->operator()( *lhs, *rhs );
}

};

} // namespace std
