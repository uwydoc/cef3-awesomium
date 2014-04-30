/**
 * @file WebSessionImpl.h
 * 
 * @breif WebSession instance class.
 */
#ifndef CEF3_AWESOMIUM_WEB_SESSION_IMPL_H_
#define CEF3_AWESOMIUM_WEB_SESSION_IMPL_H_
#pragma once

#include <Awesomium/WebSession.h>

namespace Awesomium {

class WebSessionImpl: public WebSession {
public:
    class RequestContextHandler: public CefRequestContextHandler {
    public:
        RequestContextHandler(CefRefPtr<CefCookieManager> manager)
                : cookie_manager_(manager) {}
        /// [Impl] CefRequestContextHandler
        virtual CefRefPtr<CefCookieManager> GetCookieManager() {
            return cookie_manager_;
        }
    private:
        CefRefPtr<CefCookieManager> cookie_manager_;
    }

public:
    WebSessionImpl(const WebString& path, const WebPreferences& prefs);
    virtual ~WebSessionImpl();
    CefRefPtr<CefRequestContext> request_context() const {
        return request_context_;
    }
    CefRefPtr<CefCookieManager> cookie_manager() const {
        return cookie_manager_;
    }
    ///
    /// Release the WebSession, you are responsible for calling this once you are
    /// done using the session. (Call this before shutting down WebCore).
    ///
    virtual void Release() const;

    ///
    /// Whether or not this session is being synchronized to disk (else, it's
    /// in-memory only and all data will be lost at exit).
    ///
    virtual bool IsOnDisk() const { return !path_.IsEmpty(); }

    ///
    /// The disk path of this session, if any.
    ///
    virtual WebString data_path() const { return path_; }

    ///
    /// The preferences for this session.
    ///
    virtual const WebPreferences& preferences() const { return prefs_; }

    ///
    /// Register a custom DataSource to handle "asset://" requests matching a
    /// certain hostname. This is useful for providing your own resource loader
    /// for local assets.
    ///
    /// @note  You should not add a single DataSource instance to multiple
    ///        WebSessions. The DataSource should outlive this WebSession.
    ///
    /// @param  asset_host  The asset hostname that this DataSource will be used
    ///                     for, (eg, asset://asset_host_goes_here/foobar.html).
    ///
    /// @param  source      The DataSource that will handle requests. You retain
    ///                     ownership of the DataSource. This instance should
    ///                     outlive all associated WebSessions and WebViews.
    ///
    virtual void AddDataSource(const WebString& asset_host,
                               DataSource* source);

    ///
    /// Sets a cookie for a certain URL asynchronously.
    ///
    /// @param  url  The URL to set the cookie on.
    ///
    /// @param  cookie_string  The cookie string, for example:
    ///                        <pre> "key1=value1; key2=value2" </pre>
    ///
    /// @param  is_http_only   Whether or not this cookie is HTTP-only.
    ///
    /// @param  force_session_cookie  Whether or not to force this as a session
    ///                               cookie. (Will not be saved to disk)
    ///
    virtual void SetCookie(const WebURL& url,
                           const WebString& cookie_string,
                           bool is_http_only,
                           bool force_session_cookie);

    ///
    /// Clears all cookies asynchronously.
    ///
    virtual void ClearCookies();

    ///
    /// Clears the cache asynchronously.
    ///
    virtual void ClearCache();

    ///
    /// Gets the saved zoom amount for a certain URL host (in percent).
    /// Zoom amounts are saved per-hostname.
    ///
    /// @see WebView::GetZoom
    /// @see WebView::SetZoom
    ///
    virtual int GetZoomForURL(const WebURL& url);

private:
    WebString path_;
    WebPreferences prefs_;
    CefBrowserSettings settings_;
    CefRefPtr<CefCookieManager> cookie_manager_;
    CefRefPtr<CefRequestContext> request_context_;
};

} // namespace Awesomium

#endif // CEF3_AWESOMIUM_WEB_SESSION_IMPL_H_
