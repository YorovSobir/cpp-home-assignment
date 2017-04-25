#include <string>
#include <cstdint>
#include "lint.h"
#include <sstream>
#include <iostream>

namespace apa{

    lint::lint()
        : buf_(1)
    {
        uint32_t* temp = buf_.get_data();
        temp[0] = 0;
    }

    lint::lint(uint32_t number)
        : buf_(2)
    {
        uint32_t* temp = buf_.get_data();
        if (number / base_ == 0)
        {
            temp[0] = number;
            buf_.resize(1);
        }

        else
        {
            temp[0] = number % base_;
            temp[1] = number / base_;
        }
    }

    lint::lint(const std::string& src)
        : buf_((src.length() + digit_count_ - 1) / digit_count_)
    {
        uint32_t* temp = buf_.get_data();
        size_t str_len = src.length();
        size_t j = 0;
        size_t i = digit_count_;

        while (i <= str_len){

            temp[j] = std::stoul(src.substr(str_len - i, digit_count_));
            ++j;
            i += digit_count_;
        }

        if (i - digit_count_ < str_len)
        {
            temp[j] = std::stoul(src.substr(0, str_len - (i - digit_count_)));
        }

    }

    lint::lint(const lint& src)
        : buf_(src.buf_)
    {}

    lint::lint(lint&& src)
        : buf_(std::move(src.buf_))
    {}

    lint& lint::operator=(lint src)
    {
        std::swap(buf_, src.buf_);
        return *this;
    }

    lint& lint::operator+=(const lint& src)
    {
        uint32_t* l = this->buf_.get_data();
        const uint32_t* r = src.buf_.get_data();
        size_t lsize = this->buf_.get_size();
        size_t rsize = src.buf_.get_size();

        size_t max_size = std::max(lsize, rsize);
        shared_buffer new_buf(max_size + 1);

        uint32_t* res = new_buf.get_data();
        std::fill(res, res + max_size + 1, 0);

        for (size_t i = 0; i < max_size; ++i)
        {
            res[i + 1] = (res[i] + (i < lsize? l[i] : 0) + (i < rsize ? r[i] : 0)) / lint::base_;
            res[i] = (res[i] + (i < lsize? l[i] : 0) + (i < rsize ? r[i] : 0)) % lint::base_;
        }

        if (res[max_size] == 0)
        {
            new_buf.resize(1);
        }

        buf_ = new_buf;
        return *this;
    }

    lint& lint::operator*=(const uint32_t k)
    {
        uint32_t* l = this->buf_.get_data();
        size_t size = this->buf_.get_size();

        shared_buffer new_buf(size + 2);
        uint32_t* res = new_buf.get_data();
        std::fill(res, res + size + 2, 0);

        if (k == 0)
        {
            new_buf.resize(size + 1);
        }

        else
        {
            uint32_t carry = 0;
            size_t i;
            for (i = 0; i < size || carry; ++i)
            {
                uint64_t temp = (i < size ? l[i] : 0);
                temp = carry + temp * k;

                res[i] = static_cast<uint32_t>(temp % lint::base_);
                carry = static_cast<uint32_t>(temp / lint::base_);
            }

            new_buf.resize(size + 2 - i);

        }

        buf_ = new_buf;
        return *this;

    }

    lint operator*(lint src, const uint32_t k)
    {
        src *= k;
        return src;
    }

    lint& lint::operator*=(const lint& src)
    {
        uint32_t* l = this->buf_.get_data();
        const uint32_t* r = src.buf_.get_data();
        size_t lsize = this->buf_.get_size();
        size_t rsize = src.buf_.get_size();
        size_t new_size = lsize + rsize;

        shared_buffer new_buf(new_size);
        uint32_t* res = new_buf.get_data();

        std::fill(res, res + new_size, 0);

        for (size_t i = 0; i < lsize; ++i)
        {
           uint32_t carry = 0;
           for (size_t j = 0; j < rsize || carry > 0; ++j)
           {
               uint64_t temp = res[i + j] + static_cast<uint64_t>(l[i]) * (j < rsize ? r[j]: 0) + carry;
               res[i + j] = static_cast<uint32_t>(temp % lint::base_);
               carry = static_cast<uint32_t>(temp / lint::base_);

           }

        }
        size_t i = 0;

        while (res[new_size - i - 1] == 0 && (new_size - i) > 1)
        {
            ++i;
        }

        new_buf.resize(i);
        buf_ = new_buf;
        return *this;
    }

    lint& lint::operator-=(const lint& src)
    {
        uint32_t* l = this->buf_.get_data();
        const uint32_t* r = src.buf_.get_data();
        size_t rsize = src.buf_.get_size();
        size_t lsize = this->buf_.get_size();

        uint32_t carry = 0;

        for (size_t i = 0; i < rsize || carry; ++i)
        {
            int temp = l[i] - (carry + (i < rsize? r[i]: 0));
            carry = temp < 0;
            if (carry)
            {
                l[i] = static_cast<uint32_t>(temp + lint::base_);
            }

            else
            {
                l[i] = static_cast<uint32_t>(temp);
            }
        }

        size_t i = 0;
        while(l[lsize - i - 1] == 0 && (lsize - i) > 1)
        {
            ++i;
        }

        this->buf_.resize(i);

        return *this;
    }


    lint operator-(lint first, const lint& second)
    {
        first -= second;
        return first;
    }

    lint gcd(const lint& a, const lint& b)
    {
        uint32_t k = 1;
        lint l = a * k;
        lint r = b * k;
        lint temp;

        while(r)
        {
            l = l - (l / r) * r;
            temp = l;
            l = r;
            r = temp;
        }

        return l;

    }

    bool lint::operator==(const lint& src) const
    {
        const uint32_t* l = this->buf_.get_data();
        const uint32_t* r = src.buf_.get_data();
        size_t lsize = this->buf_.get_size();
        size_t rsize = src.buf_.get_size();

        if (lsize != rsize)
        {
            return false;
        }

        size_t i = 0;

        while (i < lsize && l[i] == r[i])
        {
            ++i;
        }

        if (i == lsize) return true;

        return false;

    }

    bool lint::operator!=(const lint& src) const
    {
        return !(*this == src);
    }

    lint::operator bool() const
    {
        return (*this != lint(0));
    }

    bool lint::operator<(const lint& src) const
    {
        const uint32_t* l = this->buf_.get_data();
        const uint32_t* r = src.buf_.get_data();
        size_t lsize = this->buf_.get_size();
        size_t rsize = src.buf_.get_size();

        if (lsize > rsize)
        {
            return false;
        }

        if (rsize > lsize)
        {
            return true;
        }

        int i = lsize - 1;

        while (i >= 0 && l[i] == r[i])
        {
            --i;
        }

        if (i < 0) return false;

        if (l[i] < r[i]) return true;

        return false;

    }
    bool lint::operator<=(const lint& src) const
    {
        return (*this == src || *this < src);
    }

    bool lint::operator>=(const lint& src) const
    {
        return !(*this < src);
    }

    bool lint::operator>(const lint& src) const
    {
        return !(*this <= src);
    }

    lint operator/(const lint& src, const uint32_t k)
    {
        lint temp;
        size_t src_size = src.buf_.get_size();
        const uint32_t* src_buf = src.buf_.get_data();

        shared_buffer new_buffer(src_size);
        uint32_t* a = new_buffer.get_data();
        size_t size = src_size;


        for (size_t i = 0; i < src_size; ++i)
        {
            a[i] = src_buf[i];
        }

        uint32_t r = 0;

        for (int i = size - 1; i >= 0; --i)
        {
            uint64_t current = static_cast<uint64_t>(r) * lint::base_ + a[i];
            a[i] = static_cast<uint32_t>(current / k);
            r = static_cast<uint32_t>(current % k);
        }

        size_t i = 0;
        while ((size - i) > 1 && a[size - i - 1] == 0)
        {
            ++i;
        }

        new_buffer.resize(i);

        temp.buf_ = new_buffer;
        return temp;
    }


    lint& lint::operator/=(const lint& src)
    {
        lint start;
        lint end(*this);
        lint mid;
        uint32_t k = 2;

        if (*this < src)
        {
            *this = lint(0);
            return *this;
        }

        while (start < end)
        {
            mid = (start + end) / k;

            if (*this > (mid * src))
            {
                start = mid + lint(1);
            }

            else
            {
                if (*this == (mid * src))
                {
                    *this = mid;
                    return *this;
                }

                end = mid;
            }
        }
        if (start * src > *this)
        {
            *this = start - lint(1);
        }
        else
        {
            *this = start;
        }

        return *this;
    }


    lint operator+(lint first, const lint& second)
    {
        first += second;
        return first;
    }

    lint operator*(lint first, const lint& second)
    {
        first *= second;
        return first;
    }

    lint operator/(lint first, const lint& second)
    {
        first /= second;
        return first;
    }

    std::ostream& operator<<(std::ostream& os, const lint& src)
    {
        const uint32_t* temp = src.buf_.get_data();
        size_t size = src.buf_.get_size() - 1;

        os << temp[size];

        for (int i = size - 1; i >= 0; --i)
        {
            std::ostringstream ss;
            ss << temp[i];
            std::string num = ss.str();
            size_t j = lint::digit_count_;

            while (num.length() < j)
            {
                os << '0';
                --j;
            }

            os << temp[i];
        }
        return os;
    }

    std::string lint::str() const
    {
        std::string res;
        std::stringstream os;
        os << *this;
        res = os.str();

        return res;
    }
}
