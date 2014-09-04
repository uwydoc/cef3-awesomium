/**
 * @file WebVector.h
 *
 * @breif Simple vector-like container for Awesomium.
 */
#ifndef AWESOMIUM_WEB_VECTOR_H_
#define AWESOMIUM_WEB_VECTOR_H_

#include <Awesomium/Platform.h>

#include <vector>

namespace Awesomium {

template<typename T>
class WebVector {
public:
    WebVector() {}
    WebVector(unsigned int n)
        : vector_(n)
    {}

    WebVector<T>& operator=(const WebVector<T>& rhs)
    {
        vector_ = rhs.vector_;
        return *this;
    }

    unsigned int size() const
    {
        return vector_.size();
    }
    T& at(unsigned int idx)
    {
        const WebVector<T>* const_this = this;
        return const_cast<T&>(const_this->at(idx));
    }
    const T& at(unsigned int idx) const
    {
        return vector_.at(idx);
    }
    T& operator[](unsigned int idx)
    {
        const WebVector<T>* const_this = this;
        return const_cast<T&>((*const_this)[idx]);
    }
    const T& operator[](unsigned int idx) const
    {
        return vector_[idx];
    }
    void push_back(const T& item)
    {
        vector_.push_back(item);
    }

private:
    std::vector<T> vector_;
};

}

#endif // AWESOMIUM_WEB_VECTOR_H_
