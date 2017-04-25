#include <iostream>
#include "any.h"
#include <typeinfo>

namespace utils {

    any::any()
        : content_(nullptr)
    {}

    any::any(const any& other)
        : content_(other.content_ ? other.content_->clone() : 0)
    {}

    any::~any(){
        delete content_;
    }

    any& any::operator=(any rhs){
        rhs.swap(*this);
        return *this;
    }

    bool any::empty(){
        return !content_;
    }

    any& any::swap(any& rhs){
        std::swap(content_, rhs.content_);
        return *this;
    }

    const std::type_info& any::type() const{
       return content_ ? content_->type() : typeid(void);
    }

    void swap(any& lhs, any& rhs){
        lhs.swap(rhs);
    }
}

