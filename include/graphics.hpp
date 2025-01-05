
#pragma once

#include <HTML/Document.h>

#include "htmlbuf.hpp"


namespace jademl {


class GraphicsWidget {
protected:
    const HtmlBuffer* html;

    GraphicsWidget( const HtmlBuffer* htmldoc );

    virtual bool generate() = 0;
};


} // namespace jademl

