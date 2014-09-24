/**
 * @file InternalHelper.h
 *
 * @breif Helper class acting as friend of many wrapper classes, to provide
 * restricted access to necessary classes.
 */
#ifndef AWESOMIUM_INTERNAL_HELPER_H_
#define AWESOMIUM_INTERNAL_HELPER_H_

#include <Awesomium/Platform.h>
#include <Awesomium/WebString.h>
#include <Awesomium/WebURL.h>

#include <include/cef_base.h>

namespace Awesomium {

class InternalHelper {
public:
    static WebString ToWebString(const WebURL& url);
    static WebString ToWebString(const CefString& str);
    static CefString ToCefString(const WebString& str);
    static CefString ToCefString(const WebURL& url);
    static Cursor ToOSMCursor(CefCursorHandle cursor);
    static FocusedElementType ToFocusedElementType(CefDOMNode::Type type);
};

}

#endif // AWESOMIUM_INTERNAL_HELPER_H_
