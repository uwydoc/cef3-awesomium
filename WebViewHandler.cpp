/**
 * @file WebViewHandler.cpp
 *
 * @breif All callback wrapper classes impl.
 */
#include "WebViewHandler.h"

#include <Awesomium/WebViewListener.h>

#include "InternalHelper.h"

using namespace Awesomium;
using namespace Awesomium::WebViewHandler;

#define _TWS(cef_str) InternalHelper::ToWebString(cef_str)

/// View
void View::OnAddressChange(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           const CefString& url)
{
    listener_->OnChangeAddressBar(web_view_, _TWS(url));
}
void View::OnTitleChange(CefRefPtr<CefBrowser> browser,
                         const CefString& title)
{
    listener_->OnChangeTitle(web_view_, _TWS(title));
}
bool View::OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                            const CefString& message,
                            const CefString& source,
                            int line)
{
    return listener_->OnAddConsoleMessage(web_view_, _TWS(message), line,
            _TWS(source));
}
void View::OnCursorChange(CefRefPtr<CefBrowser> browser,
                          CefCursorHandler cursor)
{
    listener_->OnChangeCursor(web_view_, InternalHelper::ToOSMCursor(cursor));
}
void View::OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefDOMNode> node)
{
    listener_->OnChangeFocus(web_view_,
            InternalHelper::ToFocusedElementType(node->GetType()));
}
