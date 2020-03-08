/**
 * Design Pattern - Object Pool
 */

// TODO: 
// - Storage will be constantly pop_back() and push_back(), is std::vector the best choice?
// - make it thread safe -> thread_local

#include <iostream>
#include <vector>
#include <memory>

template<typename T>
struct Pool {
   public:
    using Storage = std::vector<std::unique_ptr<T>>;

    static std::unique_ptr<T> acquire() {
        if (storage.empty()) {
            std::cout << "Create a new one" << std::endl;
            return std::make_unique<T>();
        } else {
            std::cout << "Take from existing" << std::endl;
            std::unique_ptr<T> res = std::move(storage.back());
            storage.pop_back();
            return std::move(res);
        }
    }

    static void release(std::unique_ptr<T> obj) {
        storage.emplace_back(std::move(obj));
    }

    static size_t size() {
        return storage.size();
    }

   private:
    static Storage storage;
};

template <typename T>
typename Pool<T>::Storage Pool<T>::storage;


using IntList = std::vector<int>;

int main(int argc, char const* argv[]) {
    std::cout << "Start" << std::endl;
    std::unique_ptr<IntList> int_list = Pool<IntList>::acquire();
    std::cout << Pool<IntList>::size() << std::endl;
    Pool<IntList>::release(std::move(int_list));
    std::cout << Pool<IntList>::size() << std::endl;
    std::unique_ptr<IntList> int_list2 = Pool<IntList>::acquire();
    std::cout << Pool<IntList>::size() << std::endl;
    std::cout << "End" << std::endl;
}
