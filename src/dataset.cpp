
#include "dataset.hpp"


namespace SQL {

SQLQuery::SQLQuery( const std::string selectstr, const std::string fromstr, const std::string wherestr )
    :   select( selectstr )
    ,   from( fromstr )
    ,   selectors( wherestr )
{}


SQLQuerySet::SQLQuerySet()
    :   std::list< SQLQuery* >()
{}

} // namespace SQL

namespace jademl::db {

DataSet::DataSet()
    :   std::map< const std::string, std::string_view >()
{}

} // namespace jademl::db