///
/// @file WebStringImpl.h
///
/// @brief The header for the WebString class.
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
#ifndef AWESOMIUM_WEB_STRING_IMPL_H_
#define AWESOMIUM_WEB_STRING_IMPL_H_
#pragma once

#include <Awesomium/WebString.h>

#include <include/internal/cef_string.h>

namespace Awesomium {

template<class T>
class WebVector;

///
/// @brief  This class represents a UTF-16 String.
///
class WebStringImpl : public WebString {
 public:
  /// Create an empty string
  explicit WebStringImpl() {}

  /// Create a string by copying it from another substring
  explicit WebStringImpl(const WebStringImpl& src, unsigned int pos, unsigned int n) {
      const wchar16* src_str = src.utf16_.c_str();
      utf16_.FromString(src_str + pos, n, true);
  }

  /// Create a string by copying it from a UTF-16 buffer
  explicit WebStringImpl(const wchar16* data)
      : utf16_(data) {}

  /// Create a string by copying it from a UTF-16 buffer
  explicit WebStringImpl(const wchar16* data, unsigned int len)
      : utf16_(data, len, true) {}

  /// Create a string by copying it from a UTF-8 buffer
  // @note One redundant copy exists while constructing the 'std::string'
  //       instance.
  explicit WebStringImpl(const char* data, unsigned int len)
      : utf16_(std::string(data, len)) {}

  WebStringImpl(const WebStringImpl& src)
      : utf16_(src.utf16_) {}

  ~WebStringImpl() {}

  WebStringImpl& operator=(const WebStringImpl& rhs) {
      utf16_ = rhs.utf16_;
      return *this;
  }

  /// Create a WebString from a UTF8 buffer
  static WebStringImpl CreateFromUTF8(const char* data, unsigned int len) {
      return WebStringImpl(data, len);
  }

  /// Get the internal UTF-16 buffer
  const wchar16* data() const { return utf16_.c_str(); }

  /// Get the length of this string
  unsigned int length() const { return utf16_.length(); }

  /// Whether or not this string is empty.
  bool IsEmpty() const { return utf16_.empty(); }

  /// Compare this string with another string.
  int Compare(const WebStringImpl& src) const {
      return utf16_.compare(src.utf16_);
  }

  WebStringImpl& Assign(const WebStringImpl& src) {
      utf16_ = src.utf16_; 
      return *this;
  }
  WebStringImpl& Assign(const WebStringImpl& src, unsigned int pos, unsigned int n) {
      const wchar16* src_str = src.utf16_.c_str();
      utf16_.FromString(src_str + pos, n, true);
      return *this;
  }
  WebStringImpl& Assign(const wchar16* data) {
      utf16_ = data;
      return *this;
  }
  WebStringImpl& Assign(const wchar16* data, unsigned int len) {
      utf16_.FromString(data, len, true);
      return *this;
  }

  /// Append a string to the end of this string.
  WebStringImpl& Append(const WebStringImpl& src) {
      std::wstring total = utf16_.ToWString() + src.utf16_.ToWString();
      utf16_ = total;
      return *this;
  }

  /// Swap the contents of this string with another.
  void Swap(WebStringImpl& src) {
      utf16_.swap(src.utf16_);
  }

  /// Clear the contents of this string.
  void Clear() {
      utf16_.clear();
  }

  ///
  /// Convert this WebString to a UTF8 string. To determine the length of
  /// the destination buffer, you should call this with NULL dest and 0 len
  /// first. Returns the length of the final conversion.
  ///
  unsigned int ToUTF8(char* dest, unsigned int len) const {
      unsigned int size = utf16_.size() * sizeof(wchar16);
      if (!dest && !len) {
          return size;
      } else {
          unsigned int final_len = size > len ? len : size;
          const wchar16* str = utf16_.c_str();
          memcpy(dest, str, final_len);
          dest[final_len] = 0;  // append '\0' at the end
          return final_len;
      }
  }

  bool operator==(const WebStringImpl& other) const {
      return (utf16_ == other.utf16_);
  }
  bool operator!=(const WebStringImpl& other) const {
      return !(*this == other);
  }
  bool operator<(const WebStringImpl& other) const {
      return (utf16_ < other.utf16_);
  }

 private:
  explicit WebStringImpl(const CefStringUTF16& utf16)
      : utf16_(utf16) {}
  CefStringUTF16 utf16_;
  friend class InternalHelper;
};

}  // namespace Awesomium

#endif  // AWESOMIUM_WEB_STRING_IMPL_H_
