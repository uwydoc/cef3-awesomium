/**
 * @file WebConfig.cpp
 *
 * @breif WebConfig Impl.
 *
 * @note CefSettings resembles WebConfig very much. Actually, they serve as the
 * same role in their respective framework.
 */
#include <Awesomium/WebConfig.h>
#include <Awesomium/STLHelpers.h>

namespace Awesomium {

OSM_EXPORT WebConfig::WebConfig()
    : log_level(kLogLevel_Normal)
    , package_path(WSLit("."))
    , plugin_path(WSLit("."))
    , log_path(WSLit("./awesomium.log"))
    , child_process_path(WSLit("./awesomium_process.exe"))
    , user_agent(WSLit("Mozilla/5.0 (Windows NT 6.1; WOW64) "
                       "AppleWebKit/535.19 (KHTML, like Gecko) "
                       "Chrome/18.0.1003.1 "
                       "Safari/535.19 "
                       "Awesomium/" OSM_VERSION))
    , remote_debugging_port(1337)
    , remote_debugging_host(WSLit("127.0.0.1"))
    , reduce_memory_usage_on_navigation(false)
    , user_script()
    , additional_options()
{}

}
