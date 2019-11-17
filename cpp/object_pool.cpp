/**
 * Design Pattern - Object Pool
 * 
 * Usage:
 * 
 */

#include <vector>
#include <memory>

template<typename T>
struct Pool {
   public:
    using Storage = std::vector<std::unique_ptr<T>>;
    static std::unique_ptr<T> acquire() {
        return std::make_unique<T>();
    }

   private:
};

using IntList = std::vector<int>;

int main(int argc, char const* argv[]) {

    std::unique_ptr<IntList> int_list = Pool<IntList>::acquire();
}
