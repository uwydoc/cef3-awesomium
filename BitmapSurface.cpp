/**
 * @file BitmapSurface.cpp
 *
 * @brief BitmapSurface and BitmapSurfaceFactory Impl.
 */
#include <Awesomium/BitmapSurface.h>

#include <assert.h>

namespace Awesomium {

namespace {  // internal
    inline int min_int(int a, int b) { return (a < b ? a : b); }
    inline int max_int(int a, int b) { return (a < b ? b : a); }
}

/// class BitmapSurface
///
OSM_EXPORT BitmapSurface::BitmapSurface(int width, int height)
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

OSM_EXPORT BitmapSurface::~BitmapSurface()
{
    if (buffer_) {
        delete[] buffer_;
        buffer_ = 0;
    }
}

void OSM_EXPORT BitmapSurface::set_is_dirty(bool is_dirty)
{
    is_dirty_ = is_dirty;
}

void OSM_EXPORT BitmapSurface::CopyTo(unsigned char* dest_buffer,
        int dest_row_span,
        int dest_depth,
        bool convert_to_bgra,
        bool flip_y) const
{
    CopyBuffers(width_, height_, buffer_, row_span_, dest_buffer,
            dest_row_span, dest_depth, convert_to_bgra, flip_y);
}

bool OSM_EXPORT BitmapSurface::SaveToPNG(const WebString& file_path,
        bool preserve_transparency) const
{
    //TODO
    return false;
}

bool OSM_EXPORT BitmapSurface::SaveToJPEG(const WebString& file_path, int quality) const
{
    //TODO
    return false;
}

unsigned char OSM_EXPORT BitmapSurface::GetAlphaAtPoint(int x, int y) const
{
    if (x < 0 || x >= width_ || y < 0 || y >= height_)
        return 0;
    return buffer_[y * row_span_ + x * 4 + 3];
}

void OSM_EXPORT BitmapSurface::Paint(unsigned char* src_buffer,
        int src_row_span,
        const Rect& src_rect,
        const Rect& dest_rect)
{
    //@note Resize if needed

    // Source and dest region must be of the same dimension
    assert(src_rect.width == dest_rect.width
            && src_rect.height == dest_rect.height);
    //@note `src_buffer` may be incorrectly explained in the doc.
    //      It should be the pixel buffer for an entire view, if not resized,
    //      should be of the same size with `buffer_`.
    //      The following implementation is based on the above assumption

    // row span should be the same based on the above assumption
    assert(src_row_span == row_span_);
    unsigned char* src_row = src_buffer + src_rect.y * src_row_span +
        src_rect.x;
    unsigned char* dest_row = buffer_ + dest_rect.y * row_span_ +
        dest_rect.x;
    int row_size = src_rect.width * 4;
    for (int h = 0; h < src_rect.height; ++h) {
        memcpy(dest_row, src_row, row_size);
        src_row += src_row_span;
        dest_row += row_span_;
    }
    // mark as dirty
    set_is_dirty(true);
}

void OSM_EXPORT BitmapSurface::Scroll(int dx, int dy, const Rect& clip_rect)
{
    Rect dest_clip_rect(clip_rect.x + dx, clip_rect.y + dy,
            clip_rect.width, clip_rect.height);
    int x = clip_rect.x;
    int clip_width = clip_rect.width;
    int clip_height = clip_rect.height;
    // Carefully choosing the direction of pixel copies, to avoid possible
    // pixel overridden
    int y_begin = clip_rect.y;
    int y_end = clip_rect.y + clip_rect.height;
    int y_delta = 1;
    if (dy < 0) {
        y_begin = clip_rect.y + clip_rect.height - 1;
        y_end = clip_rect.y - 1;
        y_delta = -1;
    }
    // Scan and copy, instead of copy pixels one by on, `memcpy` is used to
    // speed up
    for (int y = y_begin; y != y_end; y += y_delta) {
        if (0 <= y + dy && y + dy < height_) {  // dest row not out of view
            int x_begin = x;
            int x_end = x + clip_rect.width - 1;
            if (x_begin + dx < width_ - 1 && x_end + dx >= 0) {  // dest row not
                                                                 // out of view
                x_begin = max_int(x_begin+dx, 0) - dx;
                x_end = min_int(x_end+dx, width_-1) - dx;
                unsigned char* src = buffer_ + y * row_span_ + x_begin;
                unsigned char* dest = src + dy * row_span_ + dx;
                memcpy(dest, src, (x_end - x_begin + 1)*4);
            }
        }
        // Clear source pixels
        memset(buffer_ + y * row_span_ + x, 0, clip_rect.height*4);
    }
    // mark as dirty
    set_is_dirty(true);
}


/// Helper functions
///
void OSM_EXPORT CopyBuffers(int width,
                            int height,
                            unsigned char* src,
                            int src_row_span,
                            unsigned char* dest,
                            int dest_row_span,
                            int dest_depth,
                            bool convert_to_rgba,
                            bool flip_y)
{
    // 3 for 'bgra', 4 for 'bgr'
    assert(dest_depth == 3 || dest_depth == 4);

    const int src_depth = src_row_span / width;
    assert(src_depth == 4);
    // dest is of the same format with src, and no pixel convert needed
    if (src_row_span == dest_row_span && dest_depth == src_depth &&
            !convert_to_rgba) {
        if (!flip_y) { // no flip_y, just copy whole pixels at once
            memcpy(dest, src, src_row_span * height);
        } else {  // flip_y, copy pixels row by row
            unsigned char* src_row = src;
            unsigned char* dest_row = dest + dest_row_span*(height-1);
            for (int h = 0; h < height; ++h) {
                memcpy(dest_row, src_row, src_row_span);
                // procceed to the next row
                src_row += src_row_span;
                dest_row -= dest_row_span;
            }
        }
        return;
    }
    // otherwise, pixel-by-pixel copy needed
    unsigned char* src_row = src;
    unsigned char* dest_row = flip_y ? dest + dest_row_span*(height-1) : dest;
    for (int h = 0; h < height; ++h) {
        unsigned char* src_pixel = src_row;
        unsigned char* dest_pixel = dest_row;
        for (int w = 0; w < width; ++w) {
            // flip 'r' and 'b' byte if necessary
            if (convert_to_rgba) {
                dest_pixel[0] = src_pixel[2];
                dest_pixel[2] = src_pixel[0];
            }
            dest_pixel[1] = src_pixel[1];
            // copy 'a' byte if necessary
            if (dest_depth == 4)
                dest_pixel[3] = src_pixel[3];
            // procceed to the next pixel in the current row
            src_pixel += src_depth;  // src_depth is 4
            dest_pixel += dest_depth;
        }
        // procceed to the next row
        src_row += src_row_span;
        dest_row += (flip_y ? -1 : 1) * dest_row_span;
    }
}


/// class BitmapSurfaceFactory
///
OSM_EXPORT BitmapSurfaceFactory::BitmapSurfaceFactory()
{}

OSM_EXPORT BitmapSurfaceFactory::~BitmapSurfaceFactory()
{}

Awesomium::Surface* OSM_EXPORT BitmapSurfaceFactory::CreateSurface(
        WebView* view, int width, int height)
{
    BitmapSurface* surface = new BitmapSurface(width, height);
    //TODO Bind the surface with the view
    return surface;
}

void OSM_EXPORT BitmapSurfaceFactory::DestroySurface(
        Awesomium::Surface* surface)
{
    BitmapSurface* bitmap_surface = dynamic_cast<BitmapSurface*>(surface);
    delete bitmap_surface;
}

}
