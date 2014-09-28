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

/// Load
void Load::OnLoadStart(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame)
{
    bool is_error_page = false;  //TODO
    listener_->OnBeginLoadingFrame(web_view_, frame->GetIdentifier(),
            frame->IsMain(), _TWS(frame->GetURL()), is_error_page);
}
void Load::OnLoadError(CefRefPtr<CefBrowser> browser,
                       CefRefPtr<CefFrame> frame,
                       CefLoadHandler::ErrorCode errorCode,
                       const CefString& errorText,
                       const CefString& failedUrl)
{
    wostringstream oss;
    oss << "failed to load " << failedUrl.ToWString()
        << " due to " << errorText.ToWString();
    listener_->OnFailLoadingFrame(web_view_, frame->GetIdentifier(),
            frame->IsMain(), _TWS(frame->GetURL()), (int)errorCode,
            oss.str().c_str());
}
void Load::OnLoadEnd(CefRefPtr<CefBrowser> browser,
                     CefRefPtr<CefFrame> frame,
                     int httpStatusCode)
{
    //NOTE - to fix, 'cause CEF3 does not ensure that the main frame finished
    //       loading only when all other frames are loaded
    listener_->OnFinishLoadingFrame(web_view_, frame->GetIdentifier(),
            frame->IsMain(), _TWS(frame->GetURL()));
}
void Load::OnContextCreate(CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefV8Context> context)
{
    //TODO - setup js context properly with 'context'
    listener_->OnDocumentReady(web_view_, _TWS(frame->GetURL()));
}

/// Process
void Process::OnRenderProcessTerminated(CefRefPtr<CefBrowser> browser,
                                        CefRequestHandler::TerminationStatus status)
{
    listener_->OnCrashed(web_view_, InternalHelper::ToTerminationStatus(status));
}

/// Menu
void Menu::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefFrame> frame,
                               CefRefPtr<CefContextMenuParams> params,
                               CefRefPtr<CefMenuModel> model)
{
    struct WebContextMenuInfo menu_info;
    menu_info.pos_x = params.GetXCoord();
    menu_info.pos_y = params.GetYCoord();
    menu_info.media_type = InternalHelper::ToMediaType(params->GetMediaType());
    menu_info.media_state = InternalHelper::ToMediaState(params->GetMediaStateFlags());
    menu_info.link_url = _TWS(params->GetLinkUrl());
    menu_info.src_url = _TWS(params->GetSourceUrl());
    menu_info.page_url = _TWS(params->GetPageUrl());
    //menu_info.frame_url = _TWS(params->GetFrameUrl());
    //menu_info.frame_id = 0; //NOTE - to be fixed
    menu_info.frame_url = _TWS(frame->GetURL());
    menu_info.frame_id = frame->GetIdentifier();
    menu_info.selection_text = _TWS(params->GetSelectionText());
    menu_info.is_editable = params->IsEditable();
    listener_->OnShowContextMenu(web_view_, menu_info);
}
void Menu::OnContextMenuCommand(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                CefRefPtr<CefContextMenuParams> params,
                                int command_id,
                                CefContextMenuHandler::EventFlags event_flags)
{
    //NOTE - not implemented yet
}

/// Dialog
void Dialog::OnFileDialog(CefRefPtr<CefBrowser> browser,
                          CefDialogHandler::FileDialogMode mode,
                          const CefString& title,
                          const CefString& default_file_name,
                          const std::vector<CefString>& accept_types,
                          CefRefPtr<CefFileDialogCallback> callback)
{
    struct WebFileChooseInfo chooser_info;
    chooser_info.mode = InternalHelper::ToWebFileChooserMode(mode);
    chooser_info.title = _TWS(title);
    chooser_info.default_file_name = _TWS(default_file_name);
    for (auto it = accept_types.begin(); it != accept_types.end(); ++it)
        choose_info.accept_types.Push(_TWS(*it));
    listener_->OnShowFileChooser(web_view_, chooser_info);
}
bool Dialog::GetAuthCredentials(CefRefPtr<CefBrowser> browser,
                                CefRefPtr<CefFrame> frame,
                                bool isProxy,
                                const CefString& host,
                                int port,
                                const CefString& realm,
                                const CefString& scheme,
                                CefRefPtr<CefAuthCallback> callback)
{
    WebLoginDialogInfo dialog_info;
    dialog_info.request_id = 0;  //TODO
    dialog_info.request_url = _TWS(frame->GetURL());
    dialog_info.is_proxy = isProxy;
    dialog_info.host = _TWS(host);
    dialog_info.port = (unsigned short)port;
    dialog_info.scheme = _TWS(scheme);
    dialog_info.realm = _TWS(realm);
    //TODO - pass callback to listener and call `callback->Continue` when necessary
    //     - refer to `WebView::DidLogin` and `WebView::DidCancelLogin` for details
    return listener_->OnShowLoginDialog(web_view_, dialog_info);
}
void Dialog::OnCertificateError(cef_errorcode_t cert_error,
                                const CefString& request_url,
                                CefRefPtr<CefAllowCertificateErrorCallback> callback)
{
    //TODO - handle callback properly, refer to
    //       `WebView::DidOverrideCertificateError` for details
    return listener_->OnShowCertificateErrorDialog(web_view_, is_overridable,
            _TWS(request_url), InternalHelper::ToCertError(cert_error));
}

/// Print

/// Download
void WebViewHandler::OnBeforeDownload(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefDownloadItem> download_item,
                                      const CefString& suggested_name,
                                      CefRefPtr<CefBeforeDownloadCallback> callback)
{
    //TODO - handle callback properly
    listener_->OnRequestDownload(web_view_, download_item->GetId(),
            _TWS(download_item->GetURL()), _TWS(suggested_name),
            _TWS(download_item->GetMimeType()));
}
void WebViewHandler::OnDownloadUpdated(CefRefPtr<CefBrowser> browser,
                               CefRefPtr<CefDownloadItem> download_item,
                               CefRefPtr<CefDownloadItemCallback> callback)
{
    //TODO - handle callback properly
    listener_->OnUpdateDownload(web_view_, download_item->GetId(),
            download_item->GetTotalBytes(), download_item->GetReceivedBytes(),
            download_item->GetCurrentSpeed());
    if (download_item->IsComplete())
        listener_->OnFinishDownload(web_view_, download_item->GetId(),
                _TWS(download_item->GetURL()),
                _TWS(download_item->GetFullPath()));
}

/// InputMethodEditor

