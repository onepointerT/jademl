
#pragma once

#include <HTML/Document.h>

#include "jadeelement.hpp"


namespace jademl {


class HTMLBuffer
    :   public HTML::Document
{
protected:


public:
    HTMLBuffer();

    HTMLBuffer& operator<<( JadeElement* jade );
    HTMLBuffer& operator<<( JinjaTag* jinja );
};


} // namespace jademl