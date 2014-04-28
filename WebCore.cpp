/**
 * @file WebCore.cpp
 * 
 * @breif Impl. of the WebCore class.
 */
#include "WebCore.h"
#include "WebCoreImpl.h"

using namespace Awesomium;

WebCore* WebCore::Initialize(const WebConfig& config)
{
    if (!instance_)
        instance_ = new WebCoreImpl(config);
    return instance_;
}

void WebCore::Shutdown()
{
    if (!instance) return;
    delete instance_;
    instance_ = NULL;
}

WebCore* WebCore::instance()
{
    return instance_;
}
