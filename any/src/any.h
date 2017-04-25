#pragma once

#include <typeinfo>
#include <type_traits>
#include <cstring>
#include <iostream>

namespace utils {

    struct any{
        any();
        any(const any& other);
        ~any();
        any& operator=(any rhs);
        const std::type_info & type() const;
        bool empty();

        template<typename value_type>
        any(value_type value)
            : content_(new holder<value_type>(value))
        {}

        template<typename value_type>
        any& operator=(const value_type value){
            any temp(value);
            temp.swap(*this);
            return *this;
        }

        any& swap(any& rhs);

        template<typename value_type>
        friend value_type * any_cast(any *other);

        struct place_holder{
            virtual ~place_holder()
            {}

            virtual const std::type_info& type() const = 0;
            virtual place_holder* clone() const = 0;
        };

        template<typename value_type>
        struct holder : place_holder{
            holder(const value_type& value)
                : value_(value)
            {}

            virtual const std::type_info& type() const{
                return typeid(value_type);
            }

            virtual place_holder* clone() const{
                return new holder(value_);
            }

            value_type value_;
        };

    private:
       place_holder *content_;

    };

    struct bad_any_cast : public std::bad_cast {

    public:
        bad_any_cast(const char *type1, const char *type2)
            : bad_cast()
        {
            const char *msg = "bad_any_cast: failed conversion from ";
            const size_t msg_len = strlen(msg);
            const size_t len1 = strlen(type1);
            const size_t len2 = strlen(type2);
            types = new char[msg_len + len1 + len2 + 5];
            memcpy(types, msg, msg_len);
            memcpy(types + msg_len, type1, len1);
            memcpy(types + msg_len + len1, " to ", 4);
            memcpy(types + msg_len + len1 + 4, type2, len2 + 1);

        }

        ~bad_any_cast(){
            delete[] types;
        }

        virtual const char* what() const throw() {
            return types;
        }

    private:
        char *types;
    };

    template<typename value_type>
    value_type * any_cast(any *other){
        return other && other->type() == typeid(value_type) ?
                    &(static_cast<any::holder<value_type> *>(other->content_)->value_)
                    : nullptr;
    }

    template<typename value_type>
    const value_type * any_cast(const any *other) {
        return any_cast<value_type> (const_cast<any *>(other));
    }

    template<typename value_type>
    value_type any_cast(any& other) {

        typedef typename std::remove_reference<value_type>::type nonref;

        nonref *result = any_cast<nonref> (&other);
        if (!result){
            throw (bad_any_cast(other.type().name(),
                                typeid(value_type).name())
                  );
        }

       return *result;

    }

    template<typename value_type>
    const value_type any_cast(const any& other){

        typedef typename std::remove_reference<value_type>::type nonref;

        const nonref *result = any_cast<nonref> (&other);
        if (!result){
            throw (bad_any_cast(other.type().name(),
                                typeid(value_type).name())
                  );
        }

       return *result;

    }

    void swap(any& lhs, any& rhs);

}
