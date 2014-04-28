/**
 * @file CefTypeHelper.h
 * 
 * @breif Helper functions/macros that deals with misc. CEF types.
 */
#ifndef CEF3_AWESOMIUM_CEF_TYPE_HELPER_H
#define CEF3_AWESOMIUM_CEF_TYPE_HELPER_H
#pragma once

#include "Awesomium/WebString.h"
#include "Awesomium/STLHelpers.h"

#include <cef_base.h>

namespace Awesomium {

/**
 * Convert Awesomium::LogLevel/LogSeverity to cef_log_severity_t
 */
static inline cef_log_severity_t to_cef_log_severity_t(LogLevel log_level)
{
    switch (log_level) {
    case kLogLevel_None: return LOGSEVERITY_DISABLED;
    case kLogLevel_Normal: return LOGSEVERITY_DEFAULT;
    case kLogLevel_Verbose: return LOGSEVERITY_VERBOSE;
    default: return LOGSEVERITY_DEFAULT;
    }
}
static inline cef_log_severity_t to_cef_log_severity_t(LogSeverity log_severity)
{
    switch (log_severity) {
    case kLogSeverity_Info: return LOGSEVERITY_INFO;
    case kLogSeverity_Warning: return LOGSEVERITY_WARNING;
    case kLogSeverity_Error: return LOGSEVERITY_ERROR;
    case kLogSeverity_Fatal: return LOGSEVERITY_ERROR_REPORT;
    default: return LOGSEVERITY_DEFAULT;
    }
}

/**
 * Convert bool to cef_state_t
 */
static inline cef_state_t to_cef_state_t(bool state)
{
    return state ? STATE_ENABLED : STATE_DISABLED;
}

/**
 * Update an existing cef_string_t object
 */
static inline void update_cef_string_t(cef_string_t* cef_str,
                                       const std::string& str)
{
    CefString(cef_str).FromString(str);
}
static inline void update_cef_string_t(cef_string_t* cef_str,
                                       const Awesomium::WebString& web_str)
{
    CefString(cef_str).FromString(Awesomium::ToString(web_str));
}

} // namespace Awesomium

#endif // CEF3_AWESOMIUM_CEF_TYPE_HELPER_H