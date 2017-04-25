#pragma once

#include <cstddef>
#include <cstdint>

struct shared_buffer {
    explicit shared_buffer(size_t data_size);
    shared_buffer(const shared_buffer &src);
    shared_buffer& operator=(shared_buffer src);
    shared_buffer(shared_buffer &&src);
    ~shared_buffer();

    uint32_t* get_data();
    void resize(size_t);
    const uint32_t* get_data() const;
    size_t get_size() const;
private:
    struct shared;
    shared* shared_;
};
