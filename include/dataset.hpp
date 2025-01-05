
#pragma once

#include <list>
#include <map>
#include <string>
#include <string_view>


// Software query language queries
namespace SQL {

class SQLQuery {
public:
    const std::string select; // Like "SELECT $select"
    const std::string from; // Like "FROM $tablename"
    const std::string selectors; // Like "WHERE $selectors"

    SQLQuery( const std::string selectstr, const std::string fromstr, const std::string wherestr );
};


class SQLQuerySet
    :   public std::list< SQLQuery* >
{
public:
    SQLQuerySet();
};


} // namespace SQL

namespace jademl::db {


class DataSet final
    :   public std::map< const std::string, std::string_view >
{
public:
    DataSet();
};



class DataSetFacility {
public:
    DataSetFacility();

    virtual DataSet* getDataSet( const SQL::SQLQuery* query ) const = 0; // Needs to be implemented per DB-Adapter
    virtual DataSet* getDataSetView( const SQL::SQLQuerySet* queryset ) const = 0; // Needs to be implemented per DB-Adapter
};


} // namespace jademl::db