/**
 * @file WebURLImpl.h
 *
 * @breif Internal implementation of class WebURL.
 */
#ifndef AWESOMIUM_WEB_URL_IMPL_H_
#define AWESOMIUM_WEB_URL_IMPL_H_

#include <Awesomium/WebString.h>

#include <include/cef_url.h>
#include <include/internal/cef_types.h>

namespace Awesomium {

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

}

#endif // AWESOMIUM_WEB_URL_IMPL_H_
