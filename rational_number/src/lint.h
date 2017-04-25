#pragma once
#include <cstring>
#include <ostream>
#include "shared_buffer.h"

namespace apa{

    struct lint{

        lint();
        lint(uint32_t);
        lint(const std::string&);
        lint(const lint&);
        lint(lint&& src);
        lint& operator=(lint src);
        lint& operator+=(const lint& src);
        lint& operator*=(const lint& src);
        lint& operator-=(const lint& src);
        lint& operator/=(const lint& src);
        lint& operator*=(const uint32_t);

        bool operator==(const lint& src) const;
        bool operator!=(const lint& src) const;
        bool operator<(const lint& src) const;
        bool operator<=(const lint& src) const;
        bool operator>=(const lint& src) const;
        bool operator>(const lint& src) const;

        std::string str() const;
        explicit operator bool() const;
        friend lint gcd(const lint& a, const lint& b);
        friend lint operator/(const lint& src, const uint32_t k);
        friend std::ostream& operator<<(std::ostream& os, const lint& src);

    private:
        shared_buffer buf_;
        static const uint32_t base_ = 1000000000;
        static const uint32_t digit_count_ = 9;

    };
    lint operator*(lint src, const uint32_t k);
    lint operator+(lint first, const lint& second);
    lint operator*(lint first, const lint& second);
    lint operator/(lint first, const lint& second);
    lint operator-(lint first, const lint& second);

}
