/**
 * @file WebString.cpp
 *
 * @breif WebString Impl.
 */
#include <Awesomium/WebString.h>

#include "WebStringImpl.h"

namespace Awesomium {

namespace {
    inline WebStringImpl* to_impl(void* instance) {
        return static_cast<WebStringImpl*>(instance);
    }
}

///
/// Constructors
OSM_EXPORT WebString::WebString()
    : instance_(new WebStringImpl) {}

OSM_EXPORT WebString::WebString(const WebString& src, unsigned int pos, unsigned int n)
    : instance_(new WebStringImpl(*to_impl(src.instance_), pos, n)) {}

OSM_EXPORT WebString::WebString(const wchar16* data)
    : instance_(new WebStringImpl(data)) {}

OSM_EXPORT WebString::WebString(const wchar16* data, unsigned int len)
    : instance_(new WebStringImpl(data, len)) {}

OSM_EXPORT WebString::WebString(const WebString& src)
    : instance_(new WebStringImpl(*to_impl(src.instance_))) {}

OSM_EXPORT WebString::~WebString()
{
    WebStringImpl* impl = to_impl(instance_);
    if (impl) {
        delete impl;
        impl = 0;
    }
    instance_ = 0;
}

///
/// Member functions
WebString& OSM_EXPORT WebString::operator=(const WebString& rhs)
{
    *to_impl(instance_) = *to_impl(rhs.instance_);
    return *this;
}

const wchar16* OSM_EXPORT WebString::data() const
{
    return to_impl(instance_)->data();
}

unsigned int OSM_EXPORT WebString::length() const
{
    return to_impl(instance_)->length();
}

bool OSM_EXPORT WebString::IsEmpty() const
{
    return to_impl(instance_)->IsEmpty();
}

int OSM_EXPORT WebString::Compare(const WebString& src) const
{
    return to_impl(instance_)->Compare(*to_impl(src.instance_));
}

WebString& OSM_EXPORT WebString::Assign(const WebString& src)
{
    to_impl(instance_)->Assign(*to_impl(src.instance_));
    return *this;
}

WebString& OSM_EXPORT WebString::Assign(const WebString &src, unsigned int pos, unsigned int n)
{
    to_impl(instance_)->Assign(*to_impl(src.instance_), pos, n);
    return *this;
}

WebString& OSM_EXPORT WebString::Assign(const wchar16* data)
{
    to_impl(instance_)->Assign(data);
    return *this;
}

WebString& OSM_EXPORT WebString::Assign(const wchar16* data, unsigned int len)
{
    to_impl(instance_)->Assign(data, len);
    return *this;
}

WebString& OSM_EXPORT WebString::Append(const WebString& src)
{
    to_impl(instance_)->Append(*to_impl(src.instance_));
    return *this;
}

void OSM_EXPORT WebString::Swap(WebString& src)
{
    to_impl(instance_)->Swap(*to_impl(src.instance_));
}

void OSM_EXPORT WebString::Clear()
{
    to_impl(instance_)->Clear();
}

unsigned int OSM_EXPORT WebString::ToUTF8(char* dest, unsigned int len) const
{
    return to_impl(instance_)->ToUTF8(dest, len);
}

bool OSM_EXPORT WebString::operator==(const WebString& other) const
{
    return (*to_impl(instance_) == *to_impl(other.instance_));
}

bool OSM_EXPORT WebString::operator!=(const WebString& other) const
{
    return !(*this == other);
}

bool OSM_EXPORT WebString::operator<(const WebString& other) const
{
    return (*to_impl(instance_) < *to_impl(other.instance_));
}


///
/// Static members
WebString OSM_EXPORT WebString::CreateFromUTF8(const char* data, unsigned int len)
{
    const void* internal_instance = new WebStringImpl(data, len);
    return WebString(internal_instance);
}


///
/// Private members
OSM_EXPORT WebString::WebString(const void* internal_instance)
    : instance_(const_cast<void*>(internal_instance)) {}

}
