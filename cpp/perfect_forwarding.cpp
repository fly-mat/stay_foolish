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
        log("copy constructor");
    }
    Expensive(Expensive&& other) {
        ii = std::move(other.ii);
        vv = std::move(other.vv);
        log("move constructor");
    }
    // Expensive(int _ii, std::vector<int>&& _vv) :
    //     ii(_ii),
    //     vv(std::move(_vv)) {
    //     log("in-place constructor");
    // }

    int ii;
    std::vector<int> vv;
};

struct Wrapper_PassByValue {
    Wrapper_PassByValue(int _i, Expensive _e) :
        i(_i),
        e(_e) {}

    int i;
    Expensive e;
};

struct Wrapper_CopyConstruct {
    Wrapper_CopyConstruct(int _i, const Expensive& _e) :
        i(_i),
        e(_e) {}

    int i;
    Expensive e;
};

struct Wrapper_MoveConstruct_Wrong {
    Wrapper_MoveConstruct_Wrong(int _i, Expensive&& _e) :
        i(_i),
        e(_e) {}

    int i;
    Expensive e;
};

struct Wrapper_MoveConstruct_Correct {
    Wrapper_MoveConstruct_Correct(int _i, Expensive&& _e) :
        i(_i),
        e(std::move(_e)) {}

    int i;
    Expensive e;
};

struct Wrapper_PerfectForwarding {
    template <typename T>
    Wrapper_PerfectForwarding(int _i, T&& _e) :
        i(_i),
        e(std::forward<T>(_e)) {}

    int i;
    Expensive e;
};

struct Wrapper_EmplaceBackFashion {
    template <typename ... Args>
    Wrapper_EmplaceBackFashion(int _i, Args&& ... args) :
        i(_i),
        e(std::forward<Args>(args) ... ) {}

    int i;
    Expensive e;
};



int main(int argc, char const *argv[]) {
    log("\n---- Wrapper 1 ---------------------------------------------------");
    Expensive e1(0, {1, 2, 3, 4, 5});
    Wrapper_PassByValue w1(1, e1);

    log("\n---- Wrapper 2 ---------------------------------------------------");
    Expensive e2(0, {1, 2, 3, 4, 5});
    Wrapper_CopyConstruct w2(1, e2);

    log("\n---- Wrapper 3 ---------------------------------------------------");
    Expensive e3(0, {1, 2, 3, 4, 5});
    Wrapper_MoveConstruct_Wrong w3(1, std::move(e3));

    log("\n---- Wrapper 4 ---------------------------------------------------");
    Expensive e4(0, {1, 2, 3, 4, 5});
    Wrapper_MoveConstruct_Correct w4(1, std::move(e4));

    log("\n---- Wrapper 5 - call with lvalue ------------------------------- ");
    Expensive e5_1(0, {1, 2, 3, 4, 5});
    Wrapper_PerfectForwarding w5(1, e5_1);

    log("\n---- Wrapper 5 - call with rvalue --------------------------------");
    Expensive e5_2(0, {1, 2, 3, 4, 5});
    Wrapper_PerfectForwarding w5_2(1, std::move(e5_2));

    log("\n---- Wrapper 6 - emplace_back fashion ----------------------------");
    Wrapper_EmplaceBackFashion w6(1, 2, std::vector<int>{2, 2, 2});
}
