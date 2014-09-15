/**
 * @file InternalHelper.cpp
 *
 * @breif \class\ InternalHelper Impl.
 */
#include "InternalHelper.h"

#include "WebURLImpl.h"
#include "WebStringImpl.h"

namespace Awesomium {

WebString InternalHelper::ToWebString(const WebURL& url)
{
    WebURLImpl* instance = static_cast<WebURLImpl*>(url.instance_);
    return instance->url;
}

CefString InternalHelper::ToCefString(const WebString& web_str)
{
    WebStringImpl* instance = static_cast<WebStringImpl*>(web_str.instance_);
    return instance->utf16_;
}

CefString InternalHelper::ToCefString(const WebURL& url)
{
    return ToCefString(ToWebString(url));
}

}
