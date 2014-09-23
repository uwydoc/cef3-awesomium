/**
 * @file WebSessionImpl.cpp
 * 
 * @breif Impl of class WebSessionImpl
 */
#include "WebSessionImpl.h"

#include <include/cef_cookie.h>
#include <include/cef_request_context.h>
#include <include/cef_task.h>
#include <include/cef_runnable.h>

#include "CefTypeHelpers.h"
#include "InternalHelper.h"

using namespace Awesomium;

namespace {

CefBrowserSettings to_cef_browser_settings(const WebPreferences& prefs)
{
    CefBrowserSettings browser_settings;
    browser_settings.javascript = to_cef_state_t(prefs.enable_javascript);
    browser_settings.plugins = to_cef_state_t(prefs.enable_plugins);
    browser_settings.local_storage = to_cef_state_t(prefs.enable_local_storage);
    browser_settings.databases = to_cef_state_t(prefs.enable_databases);
    browser_settings.application_cache = to_cef_state_t(prefs.enable_app_cache);
    browser_settings.webgl = to_cef_state_t(prefs.enable_web_gl);
    browser_settings.web_security = to_cef_state_t(prefs.enable_web_security);
    browser_settings.remote_fonts = to_cef_state_t(prefs.enable_remote_fonts);
    browser_settings.accelerated_compositing = to_cef_state_t(
            prefs.enable_gpu_acceleration);
    if (!prefs.user_stylesheet.IsEmpty()) {
        /// @todo Refer to http://magpcss.org/ceforum/apidocs3/projects/(default)/_cef_browser_browser_settingst.html#user_style_sheet_location
    }
    if (!prefs.default_encoding.IsEmpty()) {
        update_cef_string_t(&browser_settings.default_encoding,
                prefs.default_encoding);
    }
    browser_settings.image_shrink_standalone_to_fit = to_cef_state_t(
            prefs.shrink_standalone_images_to_fit);
    browser_settings.image_loading = to_cef_state_t(
            prefs.load_images_automatically);
    browser_settings.javascript_open_windows = to_cef_state_t(
            prefs.allow_scripts_to_open_windows);
    browser_settings.javascript_close_windows = to_cef_state_t(
            prefs.allow_scripts_to_close_windows);
    browser_settings.javascript_access_clipboard = to_cef_state_t(
            prefs.allow_scripts_to_access_clipboard);
    browser_settings.universal_access_from_file_urls = to_cef_state_t(
            prefs.allow_universal_access_from_file_url);
    browser_settings.file_access_from_file_urls = to_cef_state_t(
            prefs.allow_file_access_from_file_url);
    /// @note unused variables in WebPreferences
    //  enable_dart, enable_web_audio, enable_smooth_scrolling, proxy_config,
    //  accept_language, accept_charset, default_encoding,
    //  allow_running_insecure_content
    return browser_settings;
}

}

WebSessionImpl::WebSessionImpl(const WebString& path,
                               const WebPreferences& prefs)
        : path_(path), prefs_(prefs)
{
    settings_ = to_cef_browser_settings(prefs);
    /// Create cookie manager for use with this session
    //WebCoreImpl* web_core = dynamic_cast<WebCoreImpl*>(WebCore::instance());
    //CefString cache_path = web_core->settings().cache_path;
    CefString cache_path;
    if (!path.IsEmpty())
        cache_path.FromString(ToString(path));
    cookie_manager_ = CefCookieManager::CreateManager(cache_path, false);
    CefRefPtr<CefRequestContextHandler> handler = new RequestContextHandler(
            cookie_manager_);
    request_context_ = CefRequestContext::CreateContext(handler);
}

WebSessionImpl::~WebSessionImpl()
{}

void WebSessionImpl::Release() const
{
    cookie_manager_->FlushStore(0);
    //TODO
}

void WebSessionImpl::AddDataSource(const WebString& asset_host,
                                   DataSource* source)
{
    /// @todo
}

void WebSessionImpl::SetCookie(const WebURL& url,
                               const WebString& cookie_string,
                               bool is_http_only,
                               bool force_session_cookie)
{
    if (!CefCurrentlyOn(TID_IO)) {
        CefPostTask(TID_IO,
                NewCefRunnableMethod(this, &WebSessionImpl::SetCookie,
                    url, cookie_string, is_http_only, force_session_cookie));
        return;
    }
    CefCookie cookie;
    cookie.httponly = is_http_only;
    cookie.has_expires = !force_session_cookie;
    update_cef_string_t(&cookie.value, ToString(cookie_string));
    WebString url_str = InternalHelper::ToWebString(url);
    cookie_manager_->SetCookie(InternalHelper::ToCefString(url_str), cookie);
}

void WebSessionImpl::ClearCookies()
{
    if (!CefCurrentlyOn(TID_IO)) {
        CefPostTask(TID_IO,
                NewCefRunnableMethod(this, &WebSessionImpl::ClearCookies));
        return;
    }
    cookie_manager_->DeleteCookies("", "");
}

void WebSessionImpl::ClearCache()
{
    cookie_manager_->FlushStore(NULL);
}

int WebSessionImpl::GetZoomForURL(const WebURL& url)
{
    /// @todo
    return 100;
}
