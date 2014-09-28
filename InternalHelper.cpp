/**
 * @file InternalHelper.cpp
 *
 * @breif \class\ InternalHelper Impl.
 */
#include "InternalHelper.h"

#include "WebURLImpl.h"
#include "WebStringImpl.h"

using namespace Awesomium;

WebString InternalHelper::ToWebString(const WebURL& url)
{
    WebURLImpl* instance = static_cast<WebURLImpl*>(url.instance_);
    return instance->url;
}

WebString InternalHelper::ToWebString(const CefString& str)
{
    WebStringImpl* internal_instance = new WebStringImpl(str);
    return WebString((const void*)internal_instance);
}

CefString InternalHelper::ToCefString(const WebString& web_str)
{
    WebStringImpl* instance = static_cast<WebStringImpl*>(web_str.instance_);
    return instance->utf16_;
}

CefString InternalHelper::ToCefString(const WebURL& url)
{
    return ToCefString(ToWebString(url));
}

Cursor InternalHelper::ToOSMCursor(CefCursorHandle cursor)
{
#ifdef WIN32
    HCURSOR hCursor = (HCURSOR)cursor;
    unsigned int cur_int = (unsigned int)hCursor;
    switch (cur_int) {
        case (unsigned int)IDC_APPSTARTING: return kCursor_Progress;
        case (unsigned int)IDC_ARROW: return kCursor_Pointer;
        case (unsigned int)IDC_CROSS: return kCursor_Cross;
        case (unsigned int)IDC_HAND: return kCursor_Hand;
        case (unsigned int)IDC_HELP: return kCursor_Help;
        case (unsigned int)IDC_IBEAM: return kCursor_IBeam;
        case (unsigned int)IDC_ICON: return kCursor_Pointer;
        case (unsigned int)IDC_NO: return kCursor_NotAllowed;
        case (unsigned int)IDC_SIZE:
        case (unsigned int)IDC_SIZEALL: return kCursor_Move;
        case (unsigned int)IDC_SIZENESW: return kCursor_NorthEastSouthWestResize;
        case (unsigned int)IDC_SIZENS: return kCursor_NorthSouthResize;
        case (unsigned int)IDC_SIZENWSE: return kCursor_NorthWestSouthEastResize;
        case (unsigned int)IDC_SIZEWE: return kCursor_EastWestResize;
        case (unsigned int)IDC_UPARROW: return kCursor_NorthResize;
        case (unsigned int)IDC_WAIT: return kCursor_Wait;
        default: return kCursor_Pointer;
    };
#endif // WIN32
}

FocusedElementType InternalHelper::ToFocusedElementType(
    CefRefPtr<CefDOMNode> node)
{
    CefString tag_name = node->GetElementTagName();
    if (tag_name == "a")
        return kFocusedElementType_Link;
    if (node->IsText())
        return kFocusedElementType_Text;
    if (tag_name == "input") {
        if(node->GetElementAttribute("type") == "text")
            return kFocusedElementType_TextInput;
        else
            return kFocusedElementType_Input;
    }
    if (node->IsEditable())
        return kFocusedElementType_EditableContent;
    if (tag_name == "embed" || tag_name == "object")
        return kFocusedElementType_Plugin;
    return kFocusedElementType_Other;
}

TerminationStatus InternalHelper::ToTerminationStatus(CefRequestHandler::TerminationStatus status)
{
    // @note kTerminationStatus_StillRunning NOT referenced
    switch (status) {
        case TS_ABNORMAL_TERMINATION: return kTerminationStatus_Abnormal;
        case TS_PROCESS_WAS_KILLED: return kTerminationStatus_Killed;
        case TS_PROCESS_CRASHED: return kTerminationStatus_Crashed;
        default: return kTerminationStatus_Normal;
    }
}

MediaType InternalHelper::ToMediaType(CefContextMenuParams::MediaType media_type)
{
    switch (media_type) {
        case CM_MEDIATYPE_NONE: return kMediaType_None;
        case CM_MEDIATYPE_IMAGE: return kMediaType_Image;
        case CM_MEDIATYPE_VIDEO: return kMediaType_Video;
        case CM_MEDIATYPE_AUDIO: return kMediaType_Audio;
        case CM_MEDIATYPE_FILE: return kMediaType_File;
        case CM_MEDIATYPE_PLUGIN: return kMediaType_Plugin;
        default: return kMediaType_None;
    }
}

int InternalHelper::ToMediaState(CefContextMenuParams::MediaStateFlags state_flags)
{
    int media_state = 0;
    if (state_flags & CM_MEDIAFLAG_NONE)
        media_state |= kMediaState_None;
    if (state_flags & CM_MEDIAFLAG_ERROR)
        media_state |= kMediaState_Error;
    if (state_flags & CM_MEDIAFLAG_PAUSED)
        media_state |= kMediaState_Paused;
    if (state_flags & CM_MEDIAFLAG_MUTED)
        media_state |= kMediaState_Muted;
    if (state_flags & CM_MEDIAFLAG_LOOP)
        media_state |= kMediaState_Loop;
    if (state_flags & CM_MEDIAFLAG_CAN_SAVE)
        media_state |= kMediaState_CanSave;
    if (state_flags & CM_MEDIAFLAG_HAS_AUDIO)
        media_state |= kMediaState_HasAudio;
    if (state_flags & CM_MEDIAFLAG_HAS_VIDEO)
        media_state |= kMediaState_HasVideo;
    //TODO - handle CM_MEDIAFLAG_CAN_CONTROL_ROOT_ELEMENT, CM_MEDIAFLAG_CAN_PRINT
    //       and CM_MEDIAFLAG_CAN_ROTATE
    return media_state;
}
