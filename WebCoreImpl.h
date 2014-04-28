/**
 * @file WebCoreImpl.h
 * 
 * @breif WebCore instance class that provide default implementations for the
 * interfaces. For INTERNAL usage only.
 */
#ifndef CEF3_AWESOMIUM_WEB_CORE_IMPL_H_
#define CEF3_AWESOMIUM_WEB_CORE_IMPL_H_
#pragma once

#include "Awesomium/WebCore.h"

#include <cef_app.h>

namespace Awesomium {

/// Forward Decl.
class SurfaceFactory;
class ResourceInterceptor;

class WebCoreImpl : public WebCore {
public:
    WebCoreImpl(const WebConfig& config);
    virtual ~WebCoreImpl();
    const WebConfig& config() const { return config_; }
    const CefSettings& settings() const { return settings_; }
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
                                         const WebPreferences& prefs);
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
                                   WebViewType type = kWebViewType_Offscreen);

    ///
    /// Updates the WebCore, you must call this periodically within your
    /// application's main update loop. This method allows the WebCore to
    /// conduct operations such as updating the Surface of each WebView,
    /// destroying any WebViews that are queued for destruction, and dispatching
    /// any queued WebViewListener events.
    ///
    virtual void Update();

    ///
    /// Log a message to the log (print to the console and log file).
    ///
    virtual void Log(const WebString& message,
                     LogSeverity severity,
                     const WebString& file,
                     int line);


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
    virtual void set_surface_factory(SurfaceFactory* factory);

    ///
    /// Get the current SurfaceFactory instance.
    ///
    virtual SurfaceFactory* surface_factory() const;

    ///
    /// Set the ResourceInterceptor instance.
    ///
    /// @param  interceptor  The instance that will be used to intercept all
    ///                      resources. You are responsible for destroying this
    ///                      instance after you call Shutdown.
    ///
    virtual void set_resource_interceptor(ResourceInterceptor* interceptor);

    ///
    /// Get the current ResourceInterceptor instance.
    ///
    virtual ResourceInterceptor* resource_interceptor() const;

    ///
    /// Get the version for this build of Awesomium.
    ///
    virtual const char* version_string() const;

private:
    WebConfig config_;
    SurfaceFactory* surface_factory_;
    ResourceInterceptor* resource_interceptor_;
    /// @todo Extra CEF3 members
    CefSettings settings_;
    CefRefPtr<CefApp> app_;
};

} // namespace Awesomium

#endif // CEF3_AWESOMIUM_WEB_CORE_IMPL_H_