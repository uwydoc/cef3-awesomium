/**
 * @file WebViewHandler.h
 *
 * @breif Awesomium wrappers that encapsulate the backend CefBrowser handlers.
 */
#ifndef CEF3_AWESOMIUM_WEB_VIEW_HANDLER_H_
#define CEF3_AWESOMIUM_WEB_VIEW_HANDLER_H_
#pragma once

#include <client_handler.h>

namespace Awesomium {

namespace WebViewListener {
    class View;
    class Load;
    class Process;
    class Menu;
    class Dialog;
    class Print;
    class Download;
    class InputMethodEditor;
}


namespace WebViewHandler {

class View : public ClientHandler::View {
public:
    View(Awesomium::WebView* view, WebViewListener::View* listener)
        : web_view_(view), listener_(listener) {}

    /// CefDisplayHandler [CefClient]
    /// @note default to be of 'CefClient'
    virtual void OnAddressChange(CefRefPtr<CefBrowser> browser,
                                 CefRefPtr<CefFrame> frame,
                                 const CefString& url);
    virtual void OnTitleChange(CefRefPtr<CefBrowser> browser,
                               const CefString& title);
    virtual bool OnConsoleMessage(CefRefPtr<CefBrowser> browser,
                                  const CefString& message,
                                  const CefString& source,
                                  int line);
    /// CefRenderHandler
    virtual void OnCursorChange(CefRefPtr<CefBrowser> browser,
                                CefCursorHandle cursor);
    /// CefRenderProcessHandler [CefApp]
    virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefDOMNode> node);
    // TODO - the following interfaces of WebViewListener::View are not
    // implemented yet:
    // 'OnChangeTooltip', 'OnChangeTargetUrl', 'OnChangeFocus',
    // 'OnShowCreatedWebView'
private:
    Awesomium::WebView* web_view_;
    WebViewListener::View* listener_;

    IMPLEMENT_REFCOUNTING(View);
};

class Load : public ClientHandler::Load {
public:
    Load(Awesomium::WebView* view, WebViewListener::View* listener)
        : web_view_(view), listener_(listener) {}

    /// CefLoadHandler [CefApp][CefRenderProcessHandler]
    virtual void OnLoadStart(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame);
    virtual void OnLoadError(CefRefPtr<CefBrowser> browser,
                             CefRefPtr<CefFrame> frame,
                             CefLoadHandler::ErrorCode errorCode,
                             const CefString& errorText,
                             const CefString& failedUrl);
    virtual void OnLoadEnd(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           int httpStatusCode);
    /// CefRenderProcessHandler [CefApp]
    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefV8Context> context);
private:
    Awesomium::WebView* web_view_;
    WebViewListener::Load* listener_;

    IMPLEMENT_REFCOUNTING(Load);
};

class Process : public ClientHandler::Process {
public:
    Process(Awesomium::WebView* view, WebViewListener::View* listener)
        : web_view_(view), listener_(listener) {}

    /// CefRequestHandler
    virtual void OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                           CefRequestHandler::TerminationStatus status);
    // TODO - the following interfaces of WebViewListener::Process are not
    // implemented yet:
    // 'OnUnresponsive', 'OnResponsive'
private:
    Awesomium::WebView* web_view_;
    WebViewListener::Process* listener_;

    IMPLEMENT_REFCOUNTING(Process);
};

class Menu : public ClientHandler::Menu {
public:
    Menu(Awesomium::WebView* view, WebViewListener::View* listener)
        : web_view_(view), listener_(listener) {}

    /// CefContextMenuHandler
    virtual void OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefContextMenuParams> params,
                                     CefRefPtr<CefMenuModel> model);
    virtual void OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefContextMenuParams> params,
                                      int command_id,
                                      CefContextMenuHandler::EventFlags event_flags);
    // TODO - the following interfaces of WebViewListener::Menu are not
    // implemented yet:
    // 'OnShowPopupMenu'
private:
    Awesomium::WebView* web_view_;
    WebViewListener::Menu* listener_;

    IMPLEMENT_REFCOUNTING(Menu);
};

class Dialog : public ClientHandler::Dialog {
public:
    Dialog(Awesomium::WebView* view, WebViewListener::View* listener)
        : web_view_(view), listener_(listener) {}

    /// CefDialogHandler
    virtual void OnFileDialog(CefRefPtr<CefBrowser> browser,
                              CefDialogHandler::FileDialogMode mode,
                              const CefString& title,
                              const CefString& default_file_name,
                              const std::vector<CefString>& accept_types,
                              CefRefPtr<CefFileDialogCallback> callback);
    /// CefRequestHandler
    virtual bool GetAuthCrendentials(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     bool isProxy,
                                     const CefString& host,
                                     int port,
                                     const CefString& realm,
                                     const CefString& scheme,
                                     CefRefPtr<CefAuthCallback> callback);
    virtual bool OnCertificateError(cef_errorcode_t cert_error,
                                    const CefString& request_url,
                                    CefRefPtr<CefAllowCertificateErrorCallback> callback);
    // TODO - the following interfaces of WebViewListener::Dialog are not
    // implemented yet:
    // 'OnShowPageInfoDialog'
private:
    Awesomium::WebView* web_view_;
    WebViewListener::Dialog* listener_;

    IMPLEMENT_REFCOUNTING(Dialog);
};

class Print : public ClientHandler::Print {
public:
    Print(Awesomium::WebView* view, WebViewListener::View* listener)
        : web_view_(view), listener_(listener) {}

    // TODO - the following interfaces WebViewListener::Print are not
    // implemented yet:
    // 'OnRequestPrint', 'OnFailPrint', 'OnFinishPrint'
private:
    Awesomium::WebView* web_view_;
    WebViewListener::Print* listener_;

    IMPLEMENT_REFCOUNTING(Print);
};

class Download : public ClientHandler::Download {
public:
    Download(Awesomium::WebView* view, WebViewListener::View* listener)
        : web_view_(view), listener_(listener) {}

    /// CefDownloadHandler
    virtual void OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefDownloadItem> download_item,
                                  const CefString& suggested_name,
                                  CefRefPtr<CefBeforeDownloadCallback> callback);
    virtual void OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefDownloadItem> download_item,
                                   CefRefPtr<CefDownloadItemCallback> callback);
    // NOTE - 'OnFinisheDownload' can be implemented in 'OnDownloadUpdated'
private:
    Awesomium::WebView* web_view_;
    WebViewListener::Download* listener_;

    IMPLEMENT_REFCOUNTING(Download);
};

class InputMethodEditor : public ClientHandler::InputMethodEditor {
public:
    InputMethodEditor(Awesomium::WebView* view, WebViewListener::View* listener)
        : web_view_(view), listener_(listener) {}

    // TODO - the following interfaces WebViewListener::InputMethodEditor are
    // not implemented yet:
    // 'OnUpdateIME', 'OnCancelIME', 'OnChangeIMERange'
private:
    Awesomium::WebView* web_view_;
    WebViewListener::InputMethodEditor* listener_;

    IMPLEMENT_REFCOUNTING(InputMethodEditor);
};

}

}

#endif // CEF3_AWESOMIUM_WEB_VIEW_HANDLER_H_
