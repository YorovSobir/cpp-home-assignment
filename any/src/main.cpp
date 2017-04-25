#include <cassert>
#include <string>
#include <iostream>
#include "any.h"

using utils::any;
using utils::any_cast;
using utils::bad_any_cast;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

void contruct_test(){

    any def;
    any copy_on_type(42);

    def = 3.14;

    def = string("2.71");

    any def_copy(def);

    def = copy_on_type;

    any e;

    assert(e.empty());
}

template<class T>
void check_cast(any& a, bool should_throw){

    bool thrown = false;

    try {
        T res = any_cast<T>(a);
        std::cout << res;
    }
    catch(bad_any_cast const& err) {

        thrown = true;
        std::cerr << err.what() << std::endl;

    }

    assert(should_throw == thrown);

}

void retrieve_value_test() {

    any ia(42);
    auto res = any_cast<double>(&ia);
    assert(res == nullptr);
    int& a_ref = any_cast<int&>(ia);

    assert(a_ref == 42);

    const any con_a(2);

    const int a_cons = any_cast<const int>(con_a);
    assert(a_cons == 2);

    const int* a_cons_ptr = any_cast<const int>(&con_a);
    assert(*a_cons_ptr == 2);

    const int& a_cons_ref = any_cast<const int&>(con_a);
    assert(a_cons_ref == 2);

    assert(&a_cons_ref == a_cons_ptr);

    const int* const a_cons_ptr_cons = any_cast<const int>(&con_a);

    assert(a_cons_ptr_cons == (&a_cons_ref));

    any empty;
    any str(std::string("foobar"));
    empty = str;
    assert(any_cast<string&>(empty) == any_cast<string&>(str));


//    code below mustn't compile
//    because we want to remove const from const any iac;
//    const any iac(ia);
//    int& rd = any_cast<int&>(iac);

    check_cast<double>(ia, true );

    check_cast<int> (ia, false);

}

void swap_test(any& a, any& b) {
    utils::swap(a, b);
}

int main() {

    contruct_test();

    retrieve_value_test();

    any a(5), b(string("6"));

    swap_test(a, b);

    return 0;

}
