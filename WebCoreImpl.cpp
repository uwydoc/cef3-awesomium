/**
 * @file WebCoreImpl.cpp
 * 
 * @breif Impl. of the internal class WebCoreImpl
 */
#include "Awesomium/WebCoreImpl.h"
#include "Awesomium/WebSessionImpl.h"
#include "Awesomium/CefAppImpl.h"
#include "Awesomium/CefTypeHelper.h"

using namespace Awesomium;

WebCoreImpl::WebCoreImpl(const WebConfig& config)
        : config_(config)
{
    /// Setup settings
    update_cef_string_t(&settings_.log_file, "./awesomium.log");
    update_cef_string_t(&settings_.browser_subprocess_path,
            "./awesomium_process.exe");
    settings_.log_severity = to_cef_log_severity(config.log_level);
    settings_.no_sandbox = true;  // default to not use sandbox
    if (!config.log_path.IsEmpty())
        update_cef_string_t(&settings_.log_file, config.log_path);
    if (!config.child_process_path.IsEmpty())
        update_cef_string_t(&settings_.browser_subprocess_path,
                config.child_process_path);
    if (!config.package_path.IsEmpty())
        update_cef_string_t(&settings_.resources_dir_path, config.package_path);
    settings_.remote_debugging_port = config.remote_debugging_port;
    if (!config.user_agent.IsEmpty())
        update_cef_string_t(&settings_.user_agent, config.user_agent);
    /// @note unused variables in WebConfig
    /// remote_debugging_host, plugin_path, reduce_memory_usage_on_navigation,
    /// additional_options
    //
    /// Create CefApp instance
    app_ = new CefAppImpl();
    /// Initialize CEF
    CefMainArgs args((HINSTANCE) GetModuleHandler(0));
    CefInitialize(args, settings_, app_.get(), NULL);
}

WebCoreImpl::~WebCoreImpl()
{
    CefShutdown();
}

WebSession* WebCoreImpl::CreateWebSession(const WebString& path,
                                          const WebPreferences& prefs)
{
    /// @todo
    return new WebSessionImpl(path, prefs);
}

WebView* WebCoreImpl::CreateWebView(int width,
                                    int height,
                                    WebSession* session = 0,
                                    WebViewType type = kWebViewType_Offscreen)
{
    /// @todo
    return NULL;
}

void WebCoreImpl::Update()
{
    /// @todo
    CefDoMessageLoopWork();
}

void WebCoreImpl::Log(const WebString& message,
                      LogSeverity severity,
                      const WebString& file,
                      int line)
{
    /// @todo
}

const char* WebCoreImpl::version_string()
{
    /// @todo
    return NULL;
}
