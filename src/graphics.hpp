
#pragma once

#include <HTML/Document.h>

#include "htmlbuf.hpp"


namespace jademl {


class GraphicsWidget {
protected:
    const HTMLBuffer* html;

    GraphicsWidget( const HTMLBuffer* htmldoc );

    virtual bool generate() = 0;
};


} // namespace jademl

