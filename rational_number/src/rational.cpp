#include "rational.h"
#include <sstream>
#include <iostream>

namespace apa{

    rational::rational()
        : numerator_(0)
        , denumerator_(1)
    {}

    rational::rational(const rational& src)
        : numerator_(src.numerator_)
        , denumerator_(src.denumerator_)
    {}

    rational::rational(rational&& src)
        : numerator_(std::move(src.numerator_))
        , denumerator_(std::move(src.denumerator_))
    {}

    rational::rational(uint32_t numerator, uint32_t denumerator)
        : numerator_(numerator)
        , denumerator_(denumerator)
    {
        if (denumerator_ == lint(0))
        {
            int x = 0;
            int y = 1;
            y /= x;
        }

        lint temp = gcd(numerator_, denumerator_);
        if (temp != lint(1))
        {
            numerator_ /= temp;
            denumerator_ /= temp;
        }
    }

    rational::rational(std::string numerator, std::string denumerator)
    {
        size_t k = numerator.find_first_not_of("1234567890");

        if (k != std::string::npos)
        {
            throw wrong_format();
        }

        numerator_ = lint(numerator);

        k = denumerator.find_first_not_of("1234567890");

        if (k != std::string::npos)
        {
            throw wrong_format();
        }

        denumerator_ = lint(denumerator);


        if (denumerator_ == lint(0))
        {
            int x = 0;
            int y = 1;
            y /= x;
        }

        lint temp = gcd(numerator_, denumerator_);
        if (temp != lint(1))
        {
            numerator_ /= temp;
            denumerator_ /= temp;
        }
    }

    rational& rational::operator=(rational other)
    {
        std::swap(numerator_, other.numerator_);
        std::swap(denumerator_, other.denumerator_);
        return *this;
    }

    rational& rational::operator+=(const rational& other)
    {
        rational tmp;
        tmp.numerator_ = (numerator_ * other.denumerator_ + other.numerator_ * denumerator_);
        tmp.denumerator_ = denumerator_ * other.denumerator_;
        numerator_ = tmp.numerator_;
        denumerator_ = tmp.denumerator_;

        if (denumerator_ == lint(0))
        {
            int x = 0;
            int y = 1;
            y /= x;
        }

        lint temp = gcd(numerator_, denumerator_);
        if (temp != lint(1))
        {
            numerator_ /= temp;
            denumerator_ /= temp;
        }
        return *this;
    }

    rational& rational::operator*=(const rational& other)
    {
        rational tmp;
        tmp.numerator_ = numerator_ * other.numerator_;
        tmp.denumerator_ = denumerator_ * other.denumerator_;

        numerator_ = tmp.numerator_;
        denumerator_ = tmp.denumerator_;


        if (denumerator_ == lint(0))
        {
            int x = 0;
            int y = 1;
            y /= x;
        }

        lint temp = gcd(numerator_, denumerator_);
        if (temp != lint(1))
        {
            numerator_ /= temp;
            denumerator_ /= temp;
        }
        return *this;
    }

    rational& rational::operator/=(const rational& other)
    {
        rational tmp;
        tmp.numerator_ = numerator_ * other.denumerator_;
        tmp.denumerator_ = denumerator_ * other.numerator_;

        numerator_ = tmp.numerator_;
        denumerator_ = tmp.denumerator_;

        if (denumerator_ == lint(0))
        {
            int x = 0;
            int y = 1;
            y /= x;
        }

        lint temp = gcd(numerator_, denumerator_);
        if (temp != lint(1))
        {
            numerator_ /= temp;
            denumerator_ /= temp;
        }
        return *this;
    }

    bool rational::operator==(const rational& other) const
    {
        return (numerator_ == other.numerator_ && denumerator_ == other.denumerator_);
    }

    bool rational::operator!=(const rational& other) const
    {
        return !(*this == other);
    }

    bool rational::operator<(const rational& other) const
    {
        if (this->denumerator_ == other.denumerator_)
        {
            return (this->numerator_ < other.numerator_);
        }

        lint n1(this->numerator_);
        lint n2(other.numerator_);

        n1 *= other.denumerator_;
        n2 *= this->denumerator_;

        return (n1 < n2);

    }

    bool rational::operator<=(const rational& other) const
    {
        return (*this == other || *this < other);
    }

    bool rational::operator>=(const rational& other) const
    {
        return !(*this < other);
    }

    bool rational::operator>(const rational& other) const
    {
        return !(*this <= other);
    }
    rational::operator bool() const
    {
        return (this->numerator_ != lint(0));
    }

    std::string rational::str() const
    {
        std::string res;

        std::stringstream os;
        os << this->numerator_ << " / " << this->denumerator_;

        res = os.str();

        return res;
    }

    std::ostream& operator<<(std::ostream& os, const rational& other)
    {
        os << other.numerator_ << " / " << other.denumerator_;
        return os;
    }

    std::istream& operator>>(std::istream& os, rational& other)
    {
        std::string res;
        std::string numerator;
        std::string denumerator;

        os >> numerator;
        os >> res;
        os >> denumerator;

        if (res != "/")
        {
            throw wrong_format();
        }

        size_t n = numerator.find_first_not_of("1234567890");
        size_t d = denumerator.find_first_not_of("1234567890");
        if (n != std::string::npos
                || d != std::string::npos
                || numerator.size() == 0
                || denumerator.size() == 0)
        {
            throw wrong_format();
        }

        other.numerator_ = lint(numerator);
        other.denumerator_ = lint(denumerator);

        if (other.denumerator_ == lint(0))
        {
            int x = 0;
            int y = 1;
            y /= x;
        }

        lint temp = gcd(other.numerator_, other.denumerator_);
        if (temp != lint(1))
        {
            other.numerator_ /= temp;
            other.denumerator_ /= temp;
        }

        return os;
    }


    rational operator+(rational first, const rational& second)
    {
        first += second;
        return first;
    }

    rational operator*(rational first, const rational& second)
    {
        first *= second;
        return first;
    }

    rational operator/(rational first, const rational& second)
    {
        first /= second;
        return first;
    }

}

// TODO implementation
