
#include "env.hpp"


#include "dataset.hpp"


namespace jademl {


Variable::Variable( const char* keystr, const char* valuestr )
    :   key( keystr )
    ,   value( valuestr )
{}


EnvStack::EnvStack()
    :   std::list< Variable* >()
{}


db::DataSet* EnvStack::toDataSet() const {
    db::DataSet* ds = new db::DataSet();
    for ( Variable* var : *this ) {
        (*ds)[var->key] = var->value;
    }
    return ds;
}


VarEnvironment::VarEnvironment()
    :   EnvStack()
{}


} // namespace jademl