/**
 * @file BitmapSurface.cpp
 *
 * @brief BitmapSurface and BitmapSurfaceFactory Impl.
 */
#include <Awesomium/BitmapSurface.h>

namespace Awesomium {

BitmapSurface::BitmapSurface(int width, int height)
    : width_(width), height_(height)
    , row_span_(width * 4)  // 32-bit BGRA format, 4 bpp
    , is_dirty_(true)
{
    if (width_ < 0)
        width_ = 0;
    if (height_ < 0)
        height_ = 0;
    buffer_ = new unsigned char[height_ * row_span_];
}

BitmapSurface::~BitmapSurface()
{
    if (buffer_) {
        delete[] buffer_;
        buffer_ = 0;
    }
}

void BitmapSurface::set_is_dirty(bool is_dirty)
{
    is_dirty_ = is_dirty;
}

void BitmapSurface::CopyTo(unsigned char* dest_buffer,
        int dest_row_span,
        int dest_depth,
        bool convert_to_bgra,
        bool flip_y) const
{
    CopyBuffers(width_, height_, buffer_, row_span_, dest_buffer,
            dest_row_span, dest_depth, convert_to_bgra, flip_y);
    is_dirty_ = false;
}

bool BitmapSurface::SaveToPNG(const WebString& file_path,
        bool preserve_transparency) const
{
    //TODO
    return true;
}

bool BitmapSurface::SaveTOJPEG(const WebString& file_path, int quality) const
{
    //TODO
    return true;
}

unsigned char BitmapSurface::GetAlphaAtPoint(int x, int y) const
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return 0;
    return buffer_[y * row_span_ + x * 4 + 3];
}

void BitmapSurface::Paint(unsigned char* src_buffer,
        int src_row_span,
        const Rect& src_rect,
        const Rect& dest_rect)
{
    assert(src_rect.width == dest_rect.width
            && src_rect.height == dest_rect.height);
    //TODO
}

void BitmapSurface::Scroll(int dx, int dy, const Rect& clip_rect)
{
    Rect dest_clip_rect(clip_rect.x + dx, clip_rect.y + dy,
            clip_rect.width, clip_rect.height);
    int x = clip_rect.x;
    for (int y = clip_rect.y; y < clip_rect.y + clip_rect.height; ++y) {
        int x_begin = x;
        //TODO
        int line_width = clip_rect.width;
    }
}

}
