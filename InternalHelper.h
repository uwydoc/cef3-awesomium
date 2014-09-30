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
#include <Awesomium/WebViewListener.h>
#include <Awesomium/WebView.h>

#include <include/cef_base.h>
#include <include/cef_dom.h>
#include <include/cef_request_handler.h>
#include <include/cef_context_menu_handler.h>
#include <include/cef_browser.h>

namespace Awesomium {

class InternalHelper {
public:
    static WebString ToWebString(const WebURL& url);
    static WebString ToWebString(const CefString& str);
    static CefString ToCefString(const WebString& str);
    static CefString ToCefString(const WebURL& url);
    static Cursor ToOSMCursor(CefCursorHandle cursor);
    static FocusedElementType ToFocusedElementType(CefRefPtr<CefDOMNode> node);
    static TerminationStatus ToTerminationStatus(CefRequestHandler::TerminationStatus status);
    static MediaType ToMediaType(CefContextMenuParams::MediaType media_type);
    static int ToMediaState(CefContextMenuParams::MediaStateFlags state_flags);

    static CefBrowserHost::MouseButtonType FromMouseButton(MouseButton button);
    static int FromKeyboardModifiers(int modifiers);
};

}

#endif // AWESOMIUM_INTERNAL_HELPER_H_
