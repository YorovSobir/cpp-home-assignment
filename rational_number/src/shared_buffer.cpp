#include <string.h>
#include <utility>
#include <cstdlib>

#include "shared_buffer.h"

struct shared_buffer::shared {
    size_t ref_cntr_;
    size_t size_;
    uint32_t* data_;
};

shared_buffer::shared_buffer(size_t data_size)
    : shared_(nullptr)
{

    shared_ = new shared;
    shared_->data_ = new uint32_t[data_size];

    shared_->size_ = data_size;
    shared_->ref_cntr_ = 1;
}

shared_buffer::shared_buffer(const shared_buffer &src)
{
    shared_ = src.shared_;
    ++shared_->ref_cntr_;
}

shared_buffer::shared_buffer(shared_buffer &&src)
    : shared_(nullptr)
{
    std::swap(shared_, src.shared_);
}

shared_buffer& shared_buffer::operator=(shared_buffer src)
{
    std::swap(shared_, src.shared_);
    return *this;
}

shared_buffer::~shared_buffer()
{
    if (shared_ == nullptr)
        return;

    --shared_->ref_cntr_;
    if (shared_->ref_cntr_ != 0)
        return;

    delete [] shared_->data_;
    delete shared_;
}

uint32_t* shared_buffer::get_data()
{
    return shared_->data_;
}

void shared_buffer::resize(size_t k)
{
    shared_->size_ -= k;
}

const uint32_t* shared_buffer::get_data() const
{
    return shared_->data_;
}

size_t shared_buffer::get_size() const
{
    return shared_->size_;
}