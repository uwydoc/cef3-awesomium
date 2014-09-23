///
/// @file WebViewImpl.h
///
/// @brief The header for the WebViewImpl class.
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
#ifndef CEF3_AWESOMIUM_WEB_VIEW_IMPL_H_
#define CEF3_AWESOMIUM_WEB_VIEW_IMPL_H_
#pragma once

#include "Awesomium/WebView.h"

namespace Awesomium {

///
/// @brief A WebView is similar to a tab in a browser. You load pages into a
/// WebView, interact with it, and display it however you want.
///
/// Unless otherwise specified, all methods are asynchronous and must
/// be called from the main thread. Each WebView is rendered in its own
/// child-process and most API calls must be proxied through IPC messages.
///
/// @see WebCore::CreateWebView
///
class WebViewImpl: public WebView {
 public:
  ///
  /// Destroy this WebView immediately. If you never call this, the WebView
  /// will be destroyed upon calling WebCore::Shutdown.
  ///
  virtual void Destroy();

  ///
  /// The type of this WebView (declared at WebCore::CreateWebView). If this
  /// is an Offscreen WebView, you will need to display the Surface and pass
  /// all input yourself.
  ///
  virtual WebViewType type() { return type_; }

  ///
  /// Get the unique ID for the corresponding child-process hosting this
  /// WebView. May return 0 if the WebView has crashed or there is no
  /// process active.
  ///
  virtual int process_id() { return process_id_; }

  ///
  /// Get the unique routing ID within the child-process. Each process may
  /// have multiple WebViews, each with their own routing-id in said process.
  /// May return 0 if the WebView has crashed or there is no process active.
  ///
  virtual int routing_id();

  ///
  /// Increment and return the next routing ID, can be used to predict the
  /// routing ID of the next child WebView in the same process.
  ///
  virtual int next_routing_id();

  ///
  /// Get the handle for the corresponding child-process hosting this
  /// WebView. This may not be initialized until some time after the
  /// WebView is actually created (when we receive the first IPC message
  /// from the child-process).
  ///
  virtual ProcessHandle process_handle() { return process_handle_; }

  ///
  /// Set the parent window for this WebView. You should only call this
  /// for windowed WebViews (eg, those created with kWebViewType_Window)
  /// on the Windows platform.
  ///
  /// You should call this method immediately after calling CreateWebView,
  /// the window for this WebView will not be created until the first
  /// call to set_parent_window on the Windows platform.
  ///
  virtual void set_parent_window(NativeWindow parent)
  { parent_window_ = parent; }

  ///
  /// Get the parent window for this WebView.
  ///
  virtual NativeWindow parent_window() { return parent_window_; }

  ///
  /// Get the actual window handle that was created by this WebView. This is
  /// only valid for windowed WebViews.
  ///
  /// On the Mac OSX platform, you will need to retrieve this window (NSView)
  /// and add it to your application's view container to display it.
  ///
  virtual NativeWindow window() { return window_; }

  ///
  /// Register a listener to handle view-related events.
  ///
  /// @param  listener  The instance to register (you retain ownership).
  ///
  virtual void set_view_listener(WebViewListener::View* listener);

  ///
  /// Register a listener to handle page-load events.
  ///
  /// @param  listener  The instance to register (you retain ownership).
  ///
  virtual void set_load_listener(WebViewListener::Load* listener);

  ///
  /// Register a listener to handle process-related events (crash, hangs, etc).
  ///
  /// @param  listener  The instance to register (you retain ownership).
  ///
  virtual void set_process_listener(WebViewListener::Process* listener);

  ///
  /// Register a listener to handle the display of various menus.
  ///
  /// @param  listener  The instance to register (you retain ownership).
  ///
  virtual void set_menu_listener(WebViewListener::Menu* listener);

  ///
  /// Register a listener to handle the display of various dialogs.
  ///
  /// @param  listener  The instance to register (you retain ownership).
  ///
  virtual void set_dialog_listener(WebViewListener::Dialog* listener);

  ///
  /// Register a listener to handle printing-related events.
  ///
  /// @param  listener  The instance to register (you retain ownership).
  ///
  virtual void set_print_listener(WebViewListener::Print* listener);

  ///
  /// Register a listener to handle download-related events.
  ///
  /// @param  listener  The instance to register (you retain ownership).
  ///
  virtual void set_download_listener(WebViewListener::Download* listener);

  ///
  /// Register a listener to handle IME-related events.
  ///
  /// @param  listener  The instance to register (you retain ownership).
  ///
  virtual void set_input_method_editor_listener(
                             WebViewListener::InputMethodEditor* listener);

  /// Get the current view-event listener (may be NULL).
  virtual WebViewListener::View* view_listener() { return view_listener_; }

  /// Get the current load-event listener (may be NULL).
  virtual WebViewListener::Load* load_listener() { return load_listener_; }

  /// Get the current process-event listener (may be NULL).
  virtual WebViewListener::Process* process_listener() { return process_listener_; }

  /// Get the current menu-event listener (may be NULL).
  virtual WebViewListener::Menu* menu_listener() { return menu_listener_; }

  /// Get the current dialog-event listener (may be NULL).
  virtual WebViewListener::Dialog* dialog_listener() { return dialog_listener_; }

  /// Get the current print-event listener (may be NULL).
  virtual WebViewListener::Print* print_listener() { return print_listener_; }

  /// Get the current download-event listener (may be NULL).
  virtual WebViewListener::Download* download_listener()
  { return download_listener_; }

  /// Get the current download-event listener (may be NULL).
  virtual WebViewListener::InputMethodEditor* input_method_editor_listener()
  { return ime_listener_; }

  ///
  /// Begin loading a certain URL asynchronously.
  ///
  /// @param  url  The URL to begin loading.
  ///
  /// @note  The page is not guaranteed to be loaded after this method returns.
  ///        You should use WebViewListener::Load::OnFinishLoadingFrame to do
  ///        something after the page is loaded.
  ///
  virtual void LoadURL(const WebURL& url);

  ///
  /// Go back one page in history.
  ///
  virtual void GoBack();

  ///
  /// Go forward one page in history.
  ///
  virtual void GoForward();

  ///
  /// Go to a specific offset in history (for example, -1 would
  /// go back one page).
  ///
  /// @param  The offset to go to.
  ///
  virtual void GoToHistoryOffset(int offset);

  ///
  /// Stop all page loads.
  ///
  virtual void Stop();

  ///
  /// Reload the current page.
  ///
  /// @param  ignore_cache  Whether or not we force cached resources to
  ///                       to be reloaded as well.
  ///
  virtual void Reload(bool ignore_cache);

  ///
  /// Check whether or not we can go back in history.
  ///
  virtual bool CanGoBack();

  ///
  /// Check whether or not we can go forward in history.
  ///
  virtual bool CanGoForward();

  ///
  /// Get the current rendering Surface. May be NULL. This is only valid for
  /// offscreen WebViews (this will be NULL for windowed WebViews).
  ///
  /// @note: If you never call WebCore::SetSurfaceFactory, the returned
  ///        Surface will always be of type BitmapSurface.
  ///
  /// @see WebCore::set_surface_factory
  ///
  virtual Surface* surface();

  /// Get the current page URL.
  virtual WebURL url() { return url_; }

  /// Get the current page title.
  virtual WebString title() { return title_; }

  /// Get the session associated with this WebView.
  virtual WebSession* session() { return session_; }

  /// Check whether or not any page resources are loading.
  virtual bool IsLoading();

  /// Check whether or not the WebView process has crashed.
  virtual bool IsCrashed();

  ///
  /// Resize to certain pixel dimensions (will trigger a new Surface to
  /// be created). This operation is asynchronous and may not complete
  /// by the time this method returns.
  ///
  /// @param  width  The width in pixels.
  ///
  /// @param  height  The height in pixels.
  ///
  virtual void Resize(int width, int height);

  ///
  /// Set the background of the view to be transparent. You must call this
  /// if you intend to preserve the transparency of a page (eg, your
  /// body element has "background-color: transparent;" or some other
  /// semi-translucent background). Please note that the alpha channel is
  /// premultiplied.
  ///
  /// This is only compatible with Offscreen WebViews.
  ///
  /// If you never call this, the view will have an opaque, white background
  /// by default.
  ///
  /// @param  is_transparent  Whether or not the view should support
  ///                         transparency.
  ///
  virtual void SetTransparent(bool is_transparent)
  { transparent_ = is_transparent; }

  ///
  /// Whether or not the view supports transparency.
  ///
  virtual bool IsTransparent() { return transparent_; }

  ///
  /// Pause the renderer. All rendering is done asynchronously in a separate
  /// process so you should call this when your view is hidden to save
  /// some CPU cycles.
  ///
  virtual void PauseRendering();

  ///
  /// Resume the renderer (will force a full repaint).
  ///
  /// @see WebView::PauseRendering
  ///
  virtual void ResumeRendering();

  ///
  /// Give the appearance of input focus. You should call this whenever the
  /// view gains focus.
  ///
  /// @note If you fail to see a blinking caret when you select a textbox,
  ///       it's usually because you forgot to call this method.
  ///
  virtual void Focus();

  ///
  /// Remove the appearance of input focus. You should call this whenever the
  /// view loses focus.
  ///
  virtual void Unfocus();

  ///
  /// Get the type of the currently-focused element. This is useful for
  /// determining if the WebView should capture keyboard events. If no element
  /// is focused, this will return kFocusedElementType_None.
  ///
  /// @see FocusedElementType
  ///
  virtual FocusedElementType focused_element_type();

  ///
  /// Zooms into the page by 20%. (This is full-page zoom, increases size of
  /// text, CSS, and pictures similar to zoom in Chrome).
  ///
  virtual void ZoomIn();

  ///
  /// Zooms out the page by 20%. (This is full-page zoom, decreases size of
  /// text, CSS, and pictures similar to zoom in Chrome).
  ///
  virtual void ZoomOut();

  ///
  /// Similar to ZoomIn and ZoomOut except you can specify an arbitrary
  /// percentage between 25% and 500%.
  ///
  ///
  /// @param zoom_percent  The percent to scale the page by. For ex, '200' will
  ///                      zoom the page to 200% (effectively double in size).
  ///
  virtual void SetZoom(int zoom_percent);

  ///
  /// Reset the page zoom to 100%.
  ///
  virtual void ResetZoom();

  ///
  /// Get the current page zoom in percent. See also WebSession::GetZoomForURL.
  ///
  virtual int GetZoom();

  ///
  /// Passes a mouse-move event to the view.
  ///
  /// @param  x  The x-coordinate of the current mouse position
  ///
  /// @param  y  The y-coordinate of the current mouse position
  ///
  /// @note All coordinates should be localized to the view. All values are in
  ///       pixels, the origin (0,0) begins at the top-left corner of the view,
  ///       positive-y values are "down" and positive-x values are "right".
  ///
  virtual void InjectMouseMove(int x, int y);

  ///
  /// Passes a mouse-down event using the last mouse-move coordinates.
  ///
  /// @param  button  The button that was pressed.
  ///
  virtual void InjectMouseDown(MouseButton button);

  ///
  /// Passes a mouse-up event using the last mouse-move coordinates.
  ///
  /// @param  button  The button that was released.
  ///
  virtual void InjectMouseUp(MouseButton button);

  ///
  /// Passes a mouse-wheel event.
  ///
  /// @param  scroll_vert  The amount of pixels to scroll vertically by.
  ///
  /// @param  scroll_horz  The amount of pixels to scroll horizontally by.
  ///
  virtual void InjectMouseWheel(int scroll_vert, int scroll_horz);

  ///
  /// Passes a keyboard event.
  ///
  /// @param  key_event  The keyboard event.
  ///
  virtual void InjectKeyboardEvent(const WebKeyboardEvent& key_event);

  ///
  /// Passes a multi-touch event.
  ///
  /// @param  touch_event  The multi-touch event.
  ///
  virtual void InjectTouchEvent(const WebTouchEvent& touch_event);

  ///
  /// Call this method to let the WebView know you will be passing
  /// text input via IME and will need to be notified of any
  /// IME-related events (caret position, user unfocusing textbox, etc.)
  /// Please see WebViewListener::InputMethodEditor.
  ///
  /// Please note this is only compatible with Offscreen WebViews.
  ///
  /// @param  activate  Whether or not IME should be activated.
  ///
  virtual void ActivateIME(bool activate);

  ///
  /// Update the current IME text composition.
  ///
  /// @param  input_string  The string generated by your IME.
  /// @param  cursor_pos    The current cursor position in your IME composition.
  /// @param  target_start  The position of the beginning of the selection.
  /// @param  target_end    The position of the end of the selection.
  ///
  virtual void SetIMEComposition(const WebString& input_string,
                                 int cursor_pos,
                                 int target_start,
                                 int target_end);

  ///
  /// Confirm a current IME text composition.
  ///
  /// @param  input_string  The string generated by your IME.
  ///
  virtual void ConfirmIMEComposition(const WebString& input_string);

  ///
  /// Cancel a current IME text composition.
  ///
  virtual void CancelIMEComposition();

  ///
  /// Undo the last 'edit' operation. (Similar to CTRL+Z).
  ///
  virtual void Undo();

  ///
  /// Redo the last 'edit' operation. (Similar to CTRL+Y).
  ///
  virtual void Redo();

  ///
  /// Performs a 'cut' operation using the system clipboard.
  ///
  virtual void Cut();

  ///
  /// Performs a 'copy' operation using the system clipboard.
  ///
  virtual void Copy();

  ///
  /// Attempt to copy an image on the page to the system clipboard.
  /// This is meant to be used with Menu::OnShowContextMenu.
  ///
  /// @param  x  The x-coordinate.
  ///
  /// @param  y  The y-coordinate.
  ///
  /// @note All coordinates should be localized to the view. All values are in
  ///       pixels, the origin (0,0) begins at the top-left corner of the view,
  ///       positive-y values are "down" and positive-x values are "right".
  ///
  virtual void CopyImageAt(int x, int y);

  ///
  /// Performs a 'paste' operation using the system clipboard.
  ///
  virtual void Paste();

  ///
  /// Performs a 'paste' operation using the system clipboard while attempting
  /// to preserve any styles of the original text.
  ///
  virtual void PasteAndMatchStyle();

  ///
  /// Performs a 'select all' operation.
  ///
  virtual void SelectAll();

  ///
  /// Prints this WebView to a PDF file asynchronously.
  ///
  /// @param  output_directory  A writeable directory to write the file(s) to.
  ///
  /// @param  config  The configuration settings to use (you must specify
  ///                 a writable output_path or this operation will fail).
  ///
  /// @see WebView::set_print_listener
  ///
  /// @return  Returns a unique request ID that you can use later to identify
  ///          this specific request (see WebViewListener::Print). May return 0
  ///          if this method fails prematurely (eg, if the view has crashed).
  ///
  virtual int PrintToFile(const WebString& output_directory,
                          const PrintConfig& config);

  ///
  /// Check if an error occurred during the last synchronous API call.
  ///
  /// @see WebView::CreateGlobalJavascriptObject
  /// @see WebView::ExecuteJavascriptWithResult
  ///
  virtual Error last_error() const;

  ///
  /// Create a JavaScript Object that will persist between all loaded pages.
  ///
  /// @note
  ///
  /// Global Objects can only contain the following JavaScript types as
  /// properties:
  ///
  /// - Number
  /// - String
  /// - Array
  /// - Other Global Objects
  /// - Null
  /// - Undefined
  ///
  /// Global Objects will retain any custom methods that are registered.
  ///
  /// You can only create objects on pages with an active DOM. (You should
  /// wait until the first DOMReady event before creating your objects).
  ///
  /// @param  name  The name of the object as it will appear in JavaScript.
  ///               To create a child global-object, you should specify the
  ///               the full name with dot-notation for example:
  ///                  parentobject.childobject
  ///
  ///               The parent object should exist before attempting to make
  ///               any children.
  ///
  /// @return The returned JSValue will be of 'Object' type if this call
  /// succeeds. You can check the reason why the call failed by calling
  /// WebView::last_error() after this method.
  ///
  virtual JSValue CreateGlobalJavascriptObject(const WebString& name);

  ///
  /// Executes some JavaScript asynchronously on the page.
  ///
  /// @param  script       The string of JavaScript to execute.
  ///
  /// @param  frame_xpath  The xpath of the frame to execute within; leave
  ///                      this blank to execute in the main frame.
  ///
  virtual void ExecuteJavascript(const WebString& script,
                                 const WebString& frame_xpath);

  ///
  /// Executes some JavaScript synchronously on the page and returns a result.
  ///
  /// @param  script       The string of JavaScript to execute.
  ///
  /// @param  frame_xpath  The xpath of the frame to execute within; leave
  ///                      this blank to execute in the main frame.
  ///
  /// @return  Returns the result (if any). Any JSObject returned from this
  ///          method will be a remote proxy for an object contained within
  ///          the WebView process. If this call fails, JSValue will have an
  ///          Undefined type. You can check WebView::last_error() for more
  ///          information about the failure.
  ///
  /// @note  You should never call this from within any of the following
  ///        callbacks:
  ///
  ///        - JSMethodHandler::OnMethodCall
  ///        - JSMethodHandler::OnMethodCallWithReturnValue
  ///        - DataSource::OnRequest
  ///
  virtual JSValue ExecuteJavascriptWithResult(const WebString& script,
                                              const WebString& frame_xpath);

  ///
  /// Register a handler for custom JSObject methods.
  ///
  /// @param  handler  The handler to register (you retain ownership).
  ///
  /// @see JSObject::SetCustomMethod
  ///
  virtual void set_js_method_handler(JSMethodHandler* handler);

  /// Get the handler for custom JSObject methods.
  virtual JSMethodHandler* js_method_handler();

  ///
  /// Set the maximum amount of time (in milliseconds) to wait for a response
  /// from a synchronous IPC message dispatched to the WebView's renderer
  /// process.
  ///
  /// You should increase this if you find that a lot of your synchronous
  /// method calls keep getting kError_TimedOut. (Your machine just may be
  /// slow at handling IPC calls).
  ///
  /// Default is 800 (ms). Set this to 0 to use no timeout.
  ///
  virtual void set_sync_message_timeout(int timeout_ms)
  { sync_message_timeout_ = timeout_ms; }

  ///
  /// Get the maximum timeout for synchronous IPC messages.
  ///
  virtual int sync_message_timeout() { return sync_message_timeout_; }

  ///
  /// This method should be called as the result of a user selecting an item
  /// in a popup (dropdown) menu.
  ///
  /// @param  item_index  The index of the item selected. Item index starts
  ///                     at 0. You can pass -1 as a shortcut for
  ///                     WebView::DidCancelPopupMenu.
  ///
  /// @see  WebViewListener::Menu::OnShowPopupMenu
  ///
  virtual void DidSelectPopupMenuItem(int item_index);

  ///
  /// This method should be called as the result of a user cancelling a popup
  /// menu.
  ///
  /// @see  WebViewListener::Menu::OnShowPopupMenu
  ///
  virtual void DidCancelPopupMenu();

  ///
  /// This method should be called as the result of a user selecting files in
  /// a file-chooser dialog.
  ///
  /// @param  files  An array of file-paths that the user selected. If the
  ///                user cancelled the dialog, you should pass an empty array.
  ///
  /// @param  should_write_files  Whether or not this was a Save File dialog.
  ///
  /// @see  WebViewListener::Dialog::OnShowFileChooser
  ///
  virtual void DidChooseFiles(const WebStringArray& files,
                              bool should_write_files);

  ///
  /// This method should be called as the result of a user supplying
  /// their credentials in a login dialog.
  ///
  /// @param  request_id  The id of the request that was handled (see
  ///                     WebLoginDialogInfo::request_id).
  ///
  /// @param  username  The username supplied.
  ///
  /// @param  password  The password supplied.
  ///
  /// @see  WebViewListener::Dialog::OnShowLoginDialog
  ///
  virtual void DidLogin(int request_id,
                        const WebString& username,
                        const WebString& password);

  ///
  /// This method should be called as the result of a user cancelling
  /// a login dialog.
  ///
  /// @param  request_id  The id of the request that was handled (see
  ///                     WebLoginDialogInfo::request_id).
  ///
  /// @see  WebViewListener::Dialog::OnShowLoginDialog
  ///
  virtual void DidCancelLogin(int request_id);

  ///
  /// This method should be called as the result of a user selecting
  /// a path to download a file. The file will only begin downloading
  /// after this call is made.
  ///
  /// @param  download_id  The id of the download.
  ///
  /// @param  path  The full path (including filename) to write the download to.
  ///
  /// @see  WebViewListener::Download::OnRequestDownload
  ///
  virtual void DidChooseDownloadPath(int download_id,
                                     const WebString& path);

  ///
  /// This method should be called as the result of a user cancelling a
  /// download.
  ///
  /// @param  download_id  The id of the download.
  ///
  /// @see  WebViewListener::Download::OnRequestDownload
  ///
  virtual void DidCancelDownload(int download_id);

  ///
  /// This method should be called as the result of a user choosing to
  /// ignore an SSL error and proceed loading the page anyways.
  ///
  /// @see  WebViewListener::Dialog::OnShowCertificateErrorDialog
  ///
  virtual void DidOverrideCertificateError();

  ///
  /// Request additional page info (such as SSL security status) asynchronously.
  ///
  /// @see  WebViewListener::Dialog::OnShowPageInfoDialog
  ///
  virtual void RequestPageInfo();

  ///
  /// Forces V8 to release as much memory as possible (collects garbage, dumps
  /// cached structures, etc) and also clears WebKit cache. This helps to
  /// reduce memory accumulated within the process associated with this view.
  ///
  virtual void ReduceMemoryUsage();

  WebViewImpl(int width,
              int height,
              WebSession* session,
              WebViewType type=kWebViewType_Offscreen);

  virtual ~WebViewImpl();

 private:
    int width_;
    int height_;
    WebSession* session_;
    WebViewType type_;
    WebURL url_;
    NativeWindow parent_window_;
    NativeWindow window_;
    WebViewListener::View* view_listener_;
    WebViewListener::Load* load_listener_;
    WebViewListener::Process* process_listener_;
    WebViewListener::Menu* menu_listener_;
    WebViewListener::Dialog* dialog_listener_;
    WebViewListener::Print* print_listener_;
    WebViewListener::Download* download_listener_;
    WebViewListener::InputMethodEditor* input_method_editor_listener_;
    JSMethodHandler* js_method_handler_;
    int sync_message_timeout_;  // ms
    int process_id_;
    ProcessHandle process_handle_;
    bool transparent_;
    bool loading_;
    bool crashed_;
    // CFE3-related members
    ClientHandler* client_handler_;
    CefRefPtr<CefBrowser> browser_;
};

}  // namespace Awesomium

#endif  // CEF3_AWESOMIUM_WEB_VIEW_IMPL_H_
