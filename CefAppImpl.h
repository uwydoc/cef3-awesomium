/**
 * @file CefAppImpl.h
 * 
 * @breif CefApp instance class for the Awesomium framework, inherits and
 * implements all necessary handlers. Currently inheirted interfaces are:
 * CefBrowserProcessHandler, CefRenderProcessHandler, CefResourceBundleHandler.
 */
#ifndef CEF3_AWESOMIUM_CEF_APP_IMPL_H_
#define CEF3_AWESOMIUM_CEF_APP_IMPL_H_
#pragma once

#include <cef_app.h>

namespace Awesomium {

class CefAppImpl: public CefApp,
                  public CefBrowserProcessHandler,
                  public CefRenderProcessHandler,
                  public CefResourceBundleHandler {
public:
    CefAppImpl();
    virtual ~CefAppImpl();
    /// [Impl] CefApp methods
    virtual void OnBeforeCommandLineProcessing(
        const CefString& process_type,
        CefRefPtr<CefCommandLine> command_line) {
    }
    virtual void OnRegisterCustomSchemes(
        CefRefPtr<CefSchemeRegistrar> registrar) {
    }
    /// [Important]
    virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler() {
        return this;
    }
    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler() {
        return this;
    }
    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() {
        return this;
    }

private:
    /// [Impl] CefBrowserProcessHandler methods
    virtual void OnContextInitialized() {}
    virtual void OnBeforeChildProcessLaunch(
        CefRefPtr<CefCommandLine> command_line) {}
    virtual void OnRenderProcessThreadCreated(
        CefRefPtr<CefListValue> extra_info) {}    

    /// [Impl] CefRenderProcessHandler methods
    virtual void OnRenderThreadCreated(CefRefPtr<CefListValue> extra_info) {}
    virtual void OnWebKitInitialized() {}
    virtual void OnBrowserCreated(CefRefPtr<CefBrowser> browser) {}
    virtual void OnBrowserDestroyed(CefRefPtr<CefBrowser> browser) {}
    virtual CefRefPtr<CefLoadHandler> GetLoadHandler() {
        return NULL;
    }
    virtual bool OnBeforeNavigation(CefRefPtr<CefBrowser> browser,
                                    CefRefPtr<CefFrame> frame,
                                    CefRefPtr<CefRequest> request,
                                    NavigationType navigation_type,
                                    bool is_redirect) {
        return false;
    }
    virtual void OnContextCreated(CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefRefPtr<CefV8Context> context) {}
    virtual void OnContextReleased(CefRefPtr<CefBrowser> browser,
                                   CefRefPtr<CefFrame> frame,
                                   CefRefPtr<CefV8Context> context) {}
    virtual void OnUncaughtException(CefRefPtr<CefBrowser> browser,
                                     CefRefPtr<CefFrame> frame,
                                     CefRefPtr<CefV8Context> context,
                                     CefRefPtr<CefV8Exception> exception,
                                     CefRefPtr<CefV8StackTrace> stackTrace) {}
    virtual void OnFocusedNodeChanged(CefRefPtr<CefBrowser> browser,
                                      CefRefPtr<CefFrame> frame,
                                      CefRefPtr<CefDOMNode> node) {}
    virtual bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser,
                                          CefProcessId source_process,
                                          CefRefPtr<CefProcessMessage> message) {
        return false;
    }

    /// [Impl] CefResourceBundleHandler
    virtual bool GetLocalizedString(int message_id,
                                    CefString& string) {
        return false;
    }
    virtual bool GetDataResource(int resource_id,
                                 void*& data,
                                 size_t& data_size) {
        return false;
    }

private:
    static CefAppImpl* instance_;
};

} // namespace Awesomium

#endif // CEF3_AWESOMIUM_CEF_APP_IMPL_H_