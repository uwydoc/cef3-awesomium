/**
 * @file WebStringArray.cpp
 *
 * @breif WebStringArray Impl.
 */
#include <Awesomium/WebStringArray.h>

#include "WebVector.h"

namespace Awesomium {

WebStringArray::WebStringArray(unsigned int n)
    : vector_(new WebVector<WebString>(n))
{}

WebStringArray::WebStringArray(const WebStringArray& rhs)
    : vector_(new WebVector<WebString>(*rhs.vector_))
{}

WebStringArray& WebStringArray::operator=(const WebStringArray& rhs)
{
    *vector_ = *rhs.vector_;
    return *this;
}

unsigned int WebStringArray::size() const
{
    return vector_->size();
}

WebString& WebStringArray::At(unsigned int idx)
{
    return vector_->at(idx);
}

const WebString& WebStringArray::At(unsigned int idx) const
{
    const WebVector<WebString>* cv = vector_;
    return cv->at(idx);
}

WebString& WebStringArray::operator[](unsigned int idx)
{
    return (*vector_)[idx];
}

const WebString& WebStringArray::operator[](unsigned int idx) const
{
    return (*vector_)[idx];
}

void WebStringArray::Push(const WebString& item)
{
    vector_->push_back(item);
}

}
