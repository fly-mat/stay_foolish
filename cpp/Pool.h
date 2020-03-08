#pragma once
#include <list>
#include <memory>
#include <unordered_map>

template <typename T>
struct Pool {
    // if necessary put custom specialization next to the definition of T
    static void initialize(T*) {}
    static void deinitialize(T*) {}

    struct PoolDeleter {
        void operator()(T* ptr) const {
            Pool::release(ptr);
        }
    };

    using Pointer = std::unique_ptr<T, PoolDeleter>;
    using Storage = std::vector<std::unique_ptr<T>>;

    static Pointer acquire() {
        T* ptr;
        if (storage.empty()) {
            ptr = new T;
        } else {
            ptr = storage.back().release();
            storage.pop_back();
        }

        initialize(ptr);
        return Pointer(ptr);
    }

   private:
    static void release(T* ptr) {
        deinitialize(ptr);
        storage.push_back(std::unique_ptr<T>(ptr));
    }

    // put storage initializer into corresponding .cpp file for type T
    static Storage storage;
};


// in CASObject.h
// template<>
// inline void Pool<CASObjectList>::deinitialize(CASObjectList* l) 
// {
// 	l->Clear();
// }

// in CASObject.cpp
// template <>
// Pool<CASObjectList>::Storage Pool<CASObjectList>::storage;


// in .h
// using Token_record_ptr = Pool<Token_record>::Pointer;

// template <>
// inline void Pool<Token_record>::initialize(Token_record* tr) {
//     tr->initTokenRecord();
// }

// in .cpp
// template <>
// std::vector<unique_ptr<Token_record>> Pool<Token_record>::storage;