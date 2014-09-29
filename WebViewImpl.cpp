/**
 * @file WebViewImpl.cpp
 *
 * @breif class WebViewImpl Impl.
 */
#include "WebViewImpl.h"

#include "InternalHelper.h"

#define _TWS(str) InternalHelper::ToWebString(str)

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

void WebViewImpl::GoBack()
{
    client_handler_->GoBack();
}

void WebViewImpl::GoForward()
{
    client_handler_->GoForward();
}

void WebViewImpl::GoToHistoryOffset(int offset)
{
    client_handler_->GoToHistoryOffset(int offset);
}

void WebViewImpl::Stop()
{
    client_handler_->StopLoad();
}

void WebViewImpl::Reload(bool ignore_cache)
{
    client_handler_->Reload(ignore_cache);
}

bool WebViewImpl::CanGoBack()
{
    return client_handler_->CanGoBack();
}

bool WebViewImpl::CanGoForward()
{
    return client_handler_->CanGoForward();
}

Surface* WebViewImpl::surface()
{
    //TODO
    if (type != kWebViewType_Offscreen)
        return NULL;
    SurfaceFactory* surface_factory = WebCore::instance()->surface_factory();
    if (surface_)
        surface_factory->DestroySurface(surface_);
    surface_ = surface_factory->CreateSurface(this, width_, height_);
    return surface_;
}

WebString WebViewImpl::title()
{
    return _TWS(client_handler_->title());
}

bool WebViewImpl::IsLoading()
{
    return client_handler_->IsLoading();
}

bool WebViewImpl::IsCrashed()
{
    return client_handler_->IsCrashed();
}

void WebViewImpl::Resize(int width, int height)
{
    client_handler_->Resize();
}

void WebViewImpl::PauseRendering()
{
    client_handler_->PauseRendering();
}

void WebViewImpl::ResumeRendering()
{
    client_handler_->ResumeRendering();
}

void WebViewImpl::Focus()
{
    client_handler_->Focus();
}

void WebViewImpl::Unfocus()
{
    client_handler_->Unfocus();
}

void WebViewImpl::ZoomIn()
{
    int zoom_percent = GetZoom() + 20;
    if (zoom_percent > 200)
        zoom_percent = 200;
    SetZoom(zoom_percent);
}

void WebViewImpl::ZoomOut()
{
    int zoom_percent = GetZoom() - 20;
    if (zoom_percent < 20)
        zoom_percent = 20;
    SetZoom(zoom_percent);
}

void WebViewImpl::SetZoom(int zoom_percent)
{
    client_handler_->SetZoomLevel(zoom_percent * 1.0 / 100);
}

void WebViewImpl::ResetZoom()
{
    SetZoom(100);
}

int WebViewImpl::GetZoom()
{
    return (int)(client_handler_->GetZoomLevel() * 100);
}
