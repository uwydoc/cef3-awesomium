/**
 * @file WebURL.cpp
 *
 * @breif WebURL Impl.
 */
#include <Awesomium/WebURL.h>

#include <include/cef_url.h>
#include <include/internal/cef_string.h>

namespace Awesomium {

namespace {  // internal
    
    // WebURL Impl. class
    struct WebURLImpl {
        WebURLImpl()
            : is_valid(false), is_empty(true)
        {}
        WebURLImpl(const WebString& url)
            : url(url), is_valid(true), is_empty(url.IsEmpty())
        {
            CefString url1(url.data(), url.length(), false);
            is_valid = CefParseURL(url1, parts);
        }
        // Reformat url parts to form a canonical url string
        CefString canonical_url() const
        {
            CefString can_url;
            CefCreateURL(parts, can_url);
            return can_url;
        }
        // member variables
        WebString url;
        bool is_valid;
        bool is_empty;
        CefURLParts parts;
        friend class WebURL;
    };

    inline WebURLImpl* to_impl(void* instance) {
        return static_cast<WebURLImpl*>(instance);
    }
}

OSM_EXPORT WebURL::WebURL()
    : instance_(new WebURLImpl)
{}

OSM_EXPORT WebURL::WebURL(const WebString& url_string)
    : instance_(new WebURLImpl(url_string))
{}

OSM_EXPORT WebURL::WebURL(const WebURL& rhs)
    : instance_(new WebURLImpl(*to_impl(rhs.instance_)))
{}

WebURL& OSM_EXPORT WebURL::operator=(const WebURL& rhs)
{
    WebURLImpl* internal_instance = new WebURLImpl(*to_impl(rhs.instance_));
    if (instance_)
        delete to_impl(instance_);
    instance_ = internal_instance;
    return *this;
}

bool OSM_EXPORT WebURL::IsValid() const
{
    return to_impl(instance_)->is_valid;
}

bool OSM_EXPORT WebURL::IsEmpty() const
{
    return to_impl(instance_)->is_empty;
}

WebString OSM_EXPORT WebURL::spec() const
{
    CefString spec(&to_impl(instance_)->parts.spec);
    return WebString(spec.c_str(), spec.length());
}

WebString OSM_EXPORT WebURL::scheme() const
{
    CefString scheme(&to_impl(instance_)->parts.scheme);
    return WebString(scheme.c_str(), scheme.length());
}

WebString OSM_EXPORT WebURL::username() const
{
    CefString username(&to_impl(instance_)->parts.username);
    return WebString(username.c_str(), username.length());
}

WebString OSM_EXPORT WebURL::password() const
{
    CefString password(&to_impl(instance_)->parts.password);
    return WebString(password.c_str(), password.length());
}

WebString OSM_EXPORT WebURL::host() const
{
    CefString host(&to_impl(instance_)->parts.host);
    return WebString(host.c_str(), host.length());
}

WebString OSM_EXPORT WebURL::port() const
{
    CefString port(&to_impl(instance_)->parts.port);
    return WebString(port.c_str(), port.length());
}

WebString OSM_EXPORT WebURL::path() const
{
    CefString path(&to_impl(instance_)->parts.path);
    return WebString(path.c_str(), path.length());
}

WebString OSM_EXPORT WebURL::query() const
{
    CefString query(&to_impl(instance_)->parts.query);
    return WebString(query.c_str(), query.length());
}

WebString OSM_EXPORT WebURL::anchor() const
{
    // @todo to be fixed
    //CefString anchor(&to_impl(instance_)->parts.anchor);
    CefString anchor;
    return WebString(anchor.c_str(), anchor.length());
}

WebString OSM_EXPORT WebURL::filename() const
{
    // @todo to be fixed
    //CefString filename(&to_impl(instance_)->parts.filename);
    CefString filename;
    return WebString(filename.c_str(), filename.length());
}

bool OSM_EXPORT WebURL::operator==(const WebURL& other) const
{
    CefString str1 = to_impl(instance_)->canonical_url();
    CefString str2 = to_impl(other.instance_)->canonical_url();
    return (str1 == str2);
}

bool OSM_EXPORT WebURL::operator!=(const WebURL& other) const
{
    return !(*this == other);
}

bool OSM_EXPORT WebURL::operator<(const WebURL& other) const
{
    CefString str1 = to_impl(instance_)->canonical_url();
    CefString str2 = to_impl(other.instance_)->canonical_url();
    return (str1 < str2);
}

}
