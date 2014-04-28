/**
 * @file WebSessionImpl.cpp
 * 
 * @breif Impl of class WebSessionImpl
 */
#include "Awesomium/WebSessionImpl.h"
#include "Awesomium/CefTypeHelper.h"

using namespace Awesomium;

WebSessionImpl::WebSessionImpl(const WebString& path,
                               const WebPreferences& prefs)
        : path_(path), prefs_(prefs)
{
    settings_.javascript = to_cef_state_t(prefs.enable_javascript);
    settings_.plugins = to_cef_state_t(prefs.enable_plugins);
    settings_.local_storage = to_cef_state_t(prefs.enable_local_storage);
    settings_.databases = to_cef_state_t(prefs.enable_databases);
    settings_.application_cache = to_cef_state_t(prefs.enable_app_cache);
    settings_.webgl = to_cef_state_t(prefs.enable_web_gl);
    settings_.web_security = to_cef_state_t(prefs.enable_web_security);
    settings_.remote_fonts = to_cef_state_t(prefs.enable_remote_fonts);
    settings_.accelerated_compositing = to_cef_state_t(
            prefs.enable_gpu_acceleration);
    if (!prefs.user_stylesheet.IsEmpty()) {
        /// @todo Refer to http://magpcss.org/ceforum/apidocs3/projects/(default)/_cef_browser_settings_t.html#user_style_sheet_location
    }
    if (!prefs.default_encoding.IsEmpty())
        update_cef_string_t(&settings_.default_encoding,
                prefs.default_encoding);
    settings_.image_shrink_standalone_to_fit = to_cef_state_t(
            prefs.shrink_standalone_images_to_fit);
    settings_.image_loading = to_cef_state_t(prefs.load_images_automatically);
    settings_.javascript_open_windows = to_cef_state_t(
            prefs.allow_scripts_to_open_windows);
    settings_.javascript_close_windows = to_cef_state_t(
            prefs.allow_scripts_to_close_windows);
    settings_.javascript_access_clipboard = to_cef_state_t(
            prefs.allow_scripts_to_access_clipboard);
    settings_.universal_access_from_file_urls = to_cef_state_t(
            prefs.allow_universal_access_from_file_urls);
    settings_.file_access_from_file_urls = to_cef_state_t(
            prefs.allow_file_access_from_file_urls);
    /// @note unused variables in WebPreferences
    //  enable_dart, enable_web_audio, enable_smooth_scrolling, proxy_config,
    //  accept_language, accept_charset, default_encoding,
    //  allow_running_insecure_content
    
    /// Create cookie manager for use with this session
    WebCoreImpl* web_core = static_cast<WebCoreImpl*>(WebCore::instance());
    CefString cache_path = web_core->settings().cache_path;
    if (!path.IsEmpty())
        cache_path.FromString(ToString(path));
    cookie_manager_ = CefCookieManager::CreateManager(cache_path, false);
}

void WebSessionImpl::AddDataSource(const WebString& asset_host,
                           DataSource* source)
{}

void WebSessionImpl::SetCookie(const WebURL& url,
                       const WebString& cookie_string,
                       bool is_http_only,
                       bool force_session_cookie)
{}

void WebSessionImpl::ClearCookies()
{}

void WebSessionImpl::ClearCache()
{}

int WebSessionImpl::GetZoomForURL(const WebURL& url)
{}
