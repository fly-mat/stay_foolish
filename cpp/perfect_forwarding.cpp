/**
 * this file explains the concept of perfect forwarding in c++ 11
 * with easy-to-understand sample codes
 * 
 * perfect forwarding is especially useful when constructing some expensive objects
 */

#include <iostream>
#include <vector>

#define log(m) (std::cout << m << std::endl)

struct Expensive {
    Expensive(int _ii, std::vector<int> _vv) :
        ii(_ii),
        vv(_vv) {
        log("default");
    }
    Expensive(const Expensive& other) {
        ii = other.ii;
        vv = other.vv;
        log("copy");
    }
    Expensive(Expensive&& other) {
        ii = std::move(other.ii);
        vv = std::move(other.vv);
        log("move");
    }

    int ii;
    std::vector<int> vv;
};

/**
 * default way of construction
 */
struct Wrapper_PassByValue {
    Wrapper_PassByValue(int _i, Expensive _e) :
        i(_i),
        e(_e) {}

    int i;
    Expensive e;
};

/**
 * copy construct
 */
struct Wrapper_CopyConstruct {
    Wrapper_CopyConstruct(int _i, const Expensive& _e) :
        i(_i),
        e(_e) {}

    int i;
    Expensive e;
};

/**
 * move construct
 * but forget to call std::move()
 * making it works the same as the copy construct version
 */
struct Wrapper_MoveConstruct_Wrong {
    Wrapper_MoveConstruct_Wrong(int _i, Expensive&& _e) :
        i(_i),
        e(_e) {}

    int i;
    Expensive e;
};

/**
 * move construct
 */
struct Wrapper_MoveConstruct_Correct {
    Wrapper_MoveConstruct_Correct(int _i, Expensive&& _e) :
        i(_i),
        e(std::move(_e)) {}

    int i;
    Expensive e;
};

/**
 * perfect forwarding
 * where T&& is known as the forwarding reference
 * 
 * constructor call changes according to the argument type
 */
struct Wrapper_PerfectForwarding {
    template <typename T>
    Wrapper_PerfectForwarding(int _i, T&& _e) :
        i(_i),
        e(std::forward<T>(_e)) {}

    int i;
    Expensive e;
};

/**
 * construct in a vector::emplace_back() fashion
 * with the variadic template
 * 
 * probably not very useful though...
 */
struct Wrapper_EmplaceBackFashion {
    template <typename ... Args>  // variadic template
    Wrapper_EmplaceBackFashion(int _i, Args&& ... args) :
        i(_i),
        e(std::forward<Args>(args) ... ) {}

    int i;
    Expensive e;
};



int main(int argc, char const *argv[]) {
    log("\n---- pass by value -----------------------------------------------");
    Expensive e1(0, {1, 2, 3, 4, 5});
    Wrapper_PassByValue w1(1, e1);
    log("=> construct 3 times, with 2 copies");

    log("\n---- copy constructor --------------------------------------------");
    Expensive e2(0, {1, 2, 3, 4, 5});
    Wrapper_CopyConstruct w2(1, e2);
    log("=> construct 2 times, with 1 copy");

    log("\n---- move constructor: wrong implementation ----------------------");
    Expensive e3(0, {1, 2, 3, 4, 5});
    Wrapper_MoveConstruct_Wrong w3(1, std::move(e3));
    log("=> construct 2 times, with 1 copy");
    log("   note that it calls the move constructor, but still got one copy,");
    log("   meaning, the implementation of the move constructor was fallacious");

    log("\n---- move constructor: correct implementation --------------------");
    Expensive e4(0, {1, 2, 3, 4, 5});
    Wrapper_MoveConstruct_Correct w4(1, std::move(e4));
    log("=> construct 2 times, with 0 copies");
    log("   i.e. the implementation of the move constructor was correct");

    log("\n---- perfect forwarding with an lvalue argument ------------------");
    Expensive e5_1(0, {1, 2, 3, 4, 5});
    Wrapper_PerfectForwarding w5(1, e5_1);
    log("=> copy constructor was called when arguments are passed as lvalues");

    log("\n---- perfect forwarding with an rvalue argument ------------------");
    Expensive e5_2(0, {1, 2, 3, 4, 5});
    Wrapper_PerfectForwarding w5_2(1, std::move(e5_2));
    log("=> move constructor was called when arguments are passed as rvalues");

    log("\n---- perfect forwarding with emplace_back fashion ----------------");
    Wrapper_EmplaceBackFashion w6(1, 2, std::vector<int>{3, 4, 5});
    log("=> in place construction");
}
