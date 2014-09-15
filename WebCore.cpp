/**
 * @file WebCore.cpp
 * 
 * @breif Impl. of the WebCore class.
 */
#include <Awesomium/WebCore.h>

#include "WebCoreImpl.h"

namespace Awesomium {

WebCore* OSM_EXPORT WebCore::instance_ = 0;

WebCore* OSM_EXPORT WebCore::Initialize(const WebConfig& config)
{
    if (!instance_)
        instance_ = new WebCoreImpl(config);
    return instance_;
}

void OSM_EXPORT WebCore::Shutdown()
{
    if (instance_) {
        delete instance_;
        instance_ = NULL;
    }
}

WebCore* OSM_EXPORT WebCore::instance()
{
    return instance_;
}

}
