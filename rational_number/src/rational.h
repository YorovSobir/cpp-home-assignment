#pragma once
#include "lint.h"
#include <exception>

namespace apa{

    class wrong_format : public std::exception
    {

      virtual const char* what() const throw()
      {
        return "wrong format of input";
      }

    };

    struct rational{
        explicit rational();
        rational(const rational&);
        rational(rational&&);
        explicit rational(uint32_t numerator, uint32_t denumerator = 1);
        explicit rational(std::string numerator, std::string denumerator = "1");
        rational& operator=(rational other);
        rational& operator+=(const rational& other);
        rational& operator*=(const rational& other);
        rational& operator/=(const rational& other);
        bool operator==(const rational& other) const;
        bool operator!=(const rational& other) const;
        bool operator<(const rational& other) const;
        bool operator<=(const rational& other) const;
        bool operator>=(const rational& other) const;
        bool operator>(const rational& other) const;
        explicit operator bool() const;
        std::string str() const;
        friend std::ostream& operator<<(std::ostream& os, const rational& other);
        friend std::istream& operator>>(std::istream& os, rational& other);

    private:
        lint numerator_;
        lint denumerator_;

    };

    rational operator+(rational first, const rational& second);
    rational operator*(rational first, const rational& second);
    rational operator/(rational first, const rational& second);

} // namespace apa
