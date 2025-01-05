
#include "functionmap.hpp"


namespace jademl {

StaticFunction::StaticFunction( std::string(*func)(std::string, db::DataSet*) )
    :   std::function< std::string(std::string, db::DataSet*) >( func )
{}


std::string StaticFunction::call( std::string basename, db::DataSet* parameters ) {
    return (*this)(basename, parameters);
}

FunctionMap::FunctionMap()
    :   std::map< std::string, StaticFunction* >()
{}


} // namespace jademl
