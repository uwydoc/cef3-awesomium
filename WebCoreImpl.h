///
/// @file WebCoreImpl.h
///
/// @brief The main header for the Awesomium C++ API. This header includes most
///        of the common API functions you will need.
///
/// @author
///
/// This file is a part of Awesomium, a Web UI bridge for native apps.
///
/// Website: <http://www.awesomium.com>
///
/// Copyright (C) 2013 Awesomium Technologies LLC. All rights reserved.
/// Awesomium is a trademark of Awesomium Technologies LLC.
///
#ifndef CEF3_AWESOMIUM_WEB_CORE_IMPL_H_
#define CEF3_AWESOMIUM_WEB_CORE_IMPL_H_
#pragma once

#include <Awesomium/WebCore.h>

#include <assert.h>

#include <include/cef_app.h>
#include <client_app.h>
#include <client_handler.h>

#include "WebSessionImpl.h"
//#include "WebViewImpl.h"
#include "CefTypeHelpers.h"

namespace Awesomium {

namespace { // internal

    /// Convert WebConfig to CefSettings
    ///
    CefSettings to_cef_settings(const WebConfig& config)
    {
        CefSettings settings;
        update_cef_string_t(&settings.log_file, "./awesomium.log");
        update_cef_string_t(&settings.browser_subprocess_path,
                "./awesomium_process.exe");
        settings.log_severity = to_cef_log_severity_t(config.log_level);
        settings.no_sandbox = true;  // default to not use sandbox
        if (!config.log_path.IsEmpty())
            update_cef_string_t(&settings.log_file, config.log_path);
        if (!config.child_process_path.IsEmpty())
            update_cef_string_t(&settings.browser_subprocess_path,
                    config.child_process_path);
        if (!config.package_path.IsEmpty())
            update_cef_string_t(&settings.resources_dir_path, config.package_path);
        settings.remote_debugging_port = config.remote_debugging_port;
        if (!config.user_agent.IsEmpty())
            update_cef_string_t(&settings.user_agent, config.user_agent);
        /// @note unused variables in WebConfig
        /// remote_debugging_host, plugin_path, reduce_memory_usage_on_navigation,
        /// additional_options
        return settings;
    }

}

/// Forward Decl.
class WebSession;
class WebView;
class WebViewImpl;
class SurfaceFactory;
class ResourceInterceptor;

class WebCoreImpl : public WebCore {
public:
    ///
    /// Create a WebSession which will be used to store all user-data (such as
    /// cookies, cache, certificates, local databases, etc).
    ///
    /// @param  path  The directory path to store the data (will create the path
    ///               if it doesn't exist, or load it if it already exists).
    ///               Specify an empty string to use an in-memory store.
    ///
    /// @return  Returns a new WebSession instance that you can use with
    ///          any number of WebViews. You are responsible for calling
    ///          WebSession::Release when you are done using the session.
    ///
    virtual WebSession* CreateWebSession(const WebString& path,
                                         const WebPreferences& prefs)
    {
        // Store the session for later cleaning in dtor
        WebSessionImpl* session = new WebSessionImpl(path, prefs);
        sessions_.push_back(session);
        return session;
    }
    ///
    /// Creates a new WebView.
    ///
    /// @param  width    The initial width, in pixels.
    /// @param  height   The initial height, in pixels.
    ///
    /// @param  session  The session to use for this WebView. Pass 0 to use
    ///                  a default, global, in-memory session.
    ///
    /// @param  type     The type of WebView to create. See WebViewType for more
    ///                  information.
    ///
    /// @return  Returns a pointer to a new WebView instance. You should call
    ///          WebView::Destroy when you are done with the instance.
    ///
    virtual WebView* CreateWebView(int width,
                                   int height,
                                   WebSession* session = 0,
                                   WebViewType type = kWebViewType_Offscreen)
    {
        //@note Only offscreen WebView support for the moment
        assert(type == kWebViewType_Offscreen);
        // Again, store the view for later cleaning in dtor
        //TODO
        //WebViewImpl* view = new WebViewImpl(width, height, session, type);
        //views_.push_back(view);
        //return view;
        return 0;
    }

    ///
    /// Updates the WebCore, you must call this periodically within your
    /// application's main update loop. This method allows the WebCore to
    /// conduct operations such as updating the Surface of each WebView,
    /// destroying any WebViews that are queued for destruction, and dispatching
    /// any queued WebViewListener events.
    ///
    virtual void Update()
    {
        CefDoMessageLoopWork();
    }

    ///
    /// Log a message to the log (print to the console and log file).
    ///
    virtual void Log(const WebString& message,
                     LogSeverity severity,
                     const WebString& file,
                     int line)
    {
        //TODO
    }


    ///
    /// Set the SurfaceFactory to be used to create Surfaces for all offscreen
    /// WebViews from this point forward. If you call this, you are responsible
    /// for destroying the passed instance after you Shutdown the WebCore.
    ///
    /// @param  factory  The factory to be used to create all Surfaces. You are
    ///                  responsible for destroying this instance after you
    ///                  call Shutdown.
    ///
    /// @note: If you never call this, a default BitmapSurfaceFactory will be
    ///        used and all Surfaces will be of type 'BitmapSurface'.
    ///
    virtual void set_surface_factory(SurfaceFactory* factory)
    {
        surface_factory_ = factory;
    }

    ///
    /// Get the current SurfaceFactory instance.
    ///
    virtual SurfaceFactory* surface_factory() const
    {
        return surface_factory_;
    }

    ///
    /// Set the ResourceInterceptor instance.
    ///
    /// @param  interceptor  The instance that will be used to intercept all
    ///                      resources. You are responsible for destroying this
    ///                      instance after you call Shutdown.
    ///
    virtual void set_resource_interceptor(ResourceInterceptor* interceptor)
    {
        resource_interceptor_ = interceptor;
    }

    ///
    /// Get the current ResourceInterceptor instance.
    ///
    virtual ResourceInterceptor* resource_interceptor() const
    {
        return resource_interceptor_;
    }

    ///
    /// Get the version for this build of Awesomium.
    ///
    virtual const char* version_string() const
    {
        return OSM_VERSION;
    }

private:
    WebCoreImpl(const WebConfig& config)
        : config_(config),
          surface_factory_(0),
          resource_interceptor_(0),
          client_(0)
    {
        // Initialize CEF
        CefMainArgs args((HINSTANCE)GetModuleHandle(0));
        // Update 'settings' according to 'config'
        CefSettings settings = to_cef_settings(config);
        CefRefPtr<CefApp> app(new ClientApp);
        CefInitialize(args, settings, app.get(), NULL);
        // Initialize ClientHandler
        client_ = new ClientHandler();
    }
    virtual ~WebCoreImpl()
    {
        // Release ClientHandler
        client_ = 0;
        // Destroy existing views
        for (auto it = views_.begin(); it != views_.end(); ++it) {
            //TODO
            //WebViewImpl* view = dynamic_cast<WebViewImpl*>(*it);
            //*it = 0;
            //if (view) {
            //    view->Destroy();
            //    delete view;
            //}
        }
        // Release all sessions
        for (auto it = sessions_.begin(); it != sessions_.end(); ++it) {
            WebSessionImpl* session = *it;
            *it = 0;
            if (session) {
                session->Release();
                delete session;
            }
        }
        // Shutdown CEF
        CefShutdown();
    }

    friend class WebCore;

    WebConfig config_;
    SurfaceFactory* surface_factory_;
    ResourceInterceptor* resource_interceptor_;
    std::vector<WebSessionImpl*> sessions_;
    std::vector<WebViewImpl*> views_;
    // CEF-related members
    CefRefPtr<ClientHandler> client_;
};

} // namespace Awesomium

#endif // CEF3_AWESOMIUM_WEB_CORE_IMPL_H_
