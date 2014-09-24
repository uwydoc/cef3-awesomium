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
    switch(hCursor) {
        case IDC_APPSTARTING: return kCursor_Progress;
        case IDC_ARROW: return kCursor_Pointer;
        case IDC_CROSS: return kCursor_Cross;
        case IDC_HAND: return kCursor_Hand;
        case IDC_HELP: return kCursor_Help;
        case IDC_IBEAM: return kCursor_IBeam;
        case IDC_ICON: return kCursor_Pointer;
        case IDC_NO: return kCursor_NotAllowed;
        case IDC_SIZE:
        case IDC_SIZEALL: return kCursor_Move;
        case IDC_SIZENESW: return kCursor_NorthEastSouthWestResize;
        case IDC_SIZENS: return kCursor_NorthSouthResize;
        case IDC_SIZENWSE: return kCursor_NorthWestSouthEastResize;
        case IDC_SIZEWE: return kCursor_WestEastResize;
        case IDC_UPARROW: return kCursor_NorthResize;
        case IDC_WAIT: return kCursor_Wait;
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
    if (tag_name == "input")
        if(node->GetElementAttribute("type") == "text")
            return kFocusedElementType_TextInput;
        else
            return kFocusedElementType_Input;
    if (node->IsEditable())
        return kFocusedElementType_EditableContent;
    if (tag_name == "embed" || tag_name == "object")
        return kFocusedElementType_Plugin;
    return kFocusedElementType_Other;
}
