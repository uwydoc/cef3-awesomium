/**
 * @file WebPreferences.cpp
 *
 * @breif WebPreferences Impl.
 *
 * @note CefBrowserSettings resembles WebPreferences very much.
 */
#include <Awesomium/WebPreferences.h>
#include <Awesomium/STLHelpers.h>

namespace Awesomium {

OSM_EXPORT WebPreferences::WebPreferences()
    : enable_javascript(true)
    , enable_dart(true)
    , enable_plugins(true)
    , enable_local_storage(true)
    , enable_databases(true)
    , enable_app_cache(true)
    , enable_web_audio(true)
    , enable_web_gl(false)
    , enable_web_security(true)
    , enable_remote_fonts(true)
    , enable_smooth_scrolling(false)
    , enable_gpu_acceleration(false)
    , user_stylesheet()
    , proxy_config(WSLit("auto"))
    , accept_language(WSLit("en-us,en"))
    , accept_charset(WSLit("iso-8859-1,*,utf-8"))
    , default_encoding(WSLit("iso-8859-1"))
    , shrink_standalone_images_to_fit(true)
    , load_images_automatically(true)
    , allow_scripts_to_open_windows(true)
    , allow_scripts_to_close_windows(true)
    , allow_scripts_to_access_clipboard(false)
    , allow_universal_access_from_file_url(false)
    , allow_file_access_from_file_url(false)
    , allow_running_insecure_content(true)
{}

}
