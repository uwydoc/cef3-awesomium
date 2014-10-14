///
/// @file JSArray.cpp
///
#include <Awesomium/JSArray.h>

#include <Awesomium/WebVector.h>

namespace Awesomium {

JSArray::JSArray()
    : vector_(new WebVector<JSValue>())
{}

explicit JSArray::JSArray(unsigned int n)
    : vector_(new WebVector<JSValue>())
{}

JSArray::JSArray(const JSArray& rhs)
    : vector_(new WebVector<JSValue>(*rhs.vector_))
{}

JSArray::~JSArray()
{
    if (vector_) {
        delete vector_;
        vector_ = 0;
    }
}

JSArray& JSArray::operator=(const JSArray& rhs)
{
    WebVector<JSValue>* tmp = new WebVector<JSValue>(*rhs.vector_);
    delete vector_;
    vector_ = tmp;
    return *this;
}

unsigned int JSArray::size() const
{
    return vector_->size();
}

unsigned int JSArray::capacity() const
{
    return vector_->capacity();
}

JSValue& JSArray::At(unsigned int idx)
{
    return vector_->at(idx);
}

const JSValue& JSArray::At(unsigned int idx) const
{
    return vector_->at(idx);
}

JSValue& JSArray::operator[](unsigned int idx)
{
    return (*vector_)[idx];
}

const JSValue& JSArray::operator[](unsigned int idx) const
{
    return (*vector_)[idx];
}

void JSArray::Push(const JSValue& item)
{
    vector_->push_back(item);
}

void JSArray::Pop()
{
    vector_->pop_back();
}

void JSArray::Insert(const JSValue& item, unsigned int idx)
{
    vector_->insert(item, idx);
}

void JSArray::Erase(unsigned int idx)
{
    vector_->erase(idx);
}

void JSArray::Clear()
{
    vector_->clear();
}

}
