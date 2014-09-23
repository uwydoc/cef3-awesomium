/**
 * @file WebViewImpl.cpp
 *
 * @breif class WebViewImpl Impl.
 */
#include "WebViewImpl.h"

#include "InternalHelper.h"

using namespace Awesomium;

WebViewImpl::WebViewImpl(int width,
                         int height,
                         WebSession* session,
                         WebViewType type)
    : width_(width), height_(height), session_(session), type_(type),
      parent_window_(0), window_(0),
      view_listener_(0), load_listener_(0), process_listener_(0),
      menu_listener_(0), dialog_listener_(0), print_listener_(0),
      download_listener_(0), input_method_editor_listener_(0),
      js_method_handler_(0), sync_message_timeout_(800),
      process_id_(0), process_handle_(0),
      transparent_(false), loading_(true), crashed_(false)
{
    WebCoreImpl* core_impl = dynamic_cast<WebCoreImpl*>(WebCore::instance());
    client_handler_ = core_impl->client_;
}

WebViewImpl::~WebViewImpl()
{}

void WebViewImpl::Destroy()
{}

void WebViewImpl::set_view_listener(WebViewListener::View* listener)
{
    view_listener_ = listener;
    client_handler_->set_view_handler(new WebViewHandler::View(listener));
}

void WebViewImpl::set_load_listener(WebViewListener::Load* listener)
{
    load_listener_ = listener;
    client_handler_->set_load_handler(new WebViewHandler::Load(listener));
}

void WebViewImpl::set_process_listener(WebViewListener::Process* listener)
{
    process_listener_ = listener;
    client_handler_->set_process_handler(new WebViewHandler::Process(listener));
}

void WebViewImpl::set_menu_listener(WebViewListener::Menu* listener)
{
    menu_listener_ = listener;
    client_handler_->set_menu_handler(new WebViewHandler::Menu(listener));
}

void WebViewImpl::set_dialog_listener(WebViewListener::Dialog* listener)
{
    dialog_listener_ = listener;
    client_handler_->set_dialog_handler(new WebViewHandler::Dialog(listener));
}

void WebViewImpl::set_print_listener(WebViewListener::Print* listener)
{
    print_listener_ = listener;
    client_handler_->set_print_handler(new WebViewHandler::Print(listener));
}

void WebViewImpl::set_download_listener(WebViewListener::Download* listener)
{
    download_listener_ = listener;
    client_handler_->set_download_handler(new WebViewHandler::Download(listener));
}

void WebViewImpl::set_input_method_editor_listener(
    WebViewListener::InputMethodEditor* listener)
{
    input_method_editor_listener_ = listener;
    client_handler_->set_input_method_editor_handler(
        new WebViewHandler::InputMethodEditor(listener));
}

void WebViewImpl::LoadURL(const WebURL& url)
{
    CefWindowInfo window_info;
    info.SetAsOffScreen(parent_window_);
    info.SetTransparentPainting(transparent_);
    WebSessionImpl* session_impl = dynamic_cast<WebSessionImpl*>(session_);
    CefBrowserHost::CreateBrowser(window_info, client_handler_,
        session_impl->settings_, InternalHelper::ToCefString(url), 0);
}
