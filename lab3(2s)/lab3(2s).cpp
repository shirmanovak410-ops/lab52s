//
// Created by Ксения on 21.03.2026.
//
#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>
#include <utility>

template<class T>
class MyUnique {
private:
    T* ptr = nullptr;
    static inline std::unordered_set<T*> managed_ptrs;
public:
    explicit MyUnique(T* p = nullptr) : ptr(p) {
        if (ptr) {
            if (managed_ptrs.count(ptr) > 0) {
                throw std::runtime_error("pointer already managed");
            }
            managed_ptrs.insert(ptr);
        }
    }
    ~MyUnique() {
        if (ptr) {
            managed_ptrs.erase(ptr);
            delete ptr;
        }
    }
    MyUnique(const MyUnique&) = delete;
    MyUnique& operator=(const MyUnique&) = delete;
    MyUnique(MyUnique&& other) noexcept : ptr(other.ptr) {
        if (ptr) {
            managed_ptrs.erase(ptr);
            managed_ptrs.insert(ptr);
        }
        other.ptr = nullptr;
    }

    MyUnique& operator=(MyUnique&& other) noexcept {
        if (this != &other) {
            if (ptr) {
                managed_ptrs.erase(ptr);
                delete ptr;
            }
            ptr = other.ptr;
            if (ptr) {
                managed_ptrs.erase(ptr);
                managed_ptrs.insert(ptr);
            }
            other.ptr = nullptr;
        }
        return *this;
    }
    T* get() const noexcept {
        return ptr;
    }
    T& operator*() const {
        if (!ptr) throw std::runtime_error("null pointer dereference");
        return *ptr;
    }
    T* operator->() const {
        if (!ptr) throw std::runtime_error("null pointer access");
        return ptr;
    }
    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }
    T* release() noexcept {
        T* temp = ptr;
        if (ptr) {
            managed_ptrs.erase(ptr);
            ptr = nullptr;
        }
        return temp;
    }
    void reset(T* new_ptr = nullptr) {
        if (new_ptr && managed_ptrs.count(new_ptr) > 0) {
            throw std::runtime_error("pointer already managed");
        }
        if (ptr) {
            managed_ptrs.erase(ptr);
            delete ptr;
        }
        ptr = new_ptr;
        if (ptr) {
            managed_ptrs.insert(ptr);
        }
    }
};

template<class T>
class MyShared {
private:
    T* ptr = nullptr;
    static inline std::unordered_map<T*, int> ref_counters;
public:
    explicit MyShared(T* p = nullptr) : ptr(p) {
        if (ptr) {
            ref_counters[ptr]++;
        }
    }
    ~MyShared() {
        release();
    }
    MyShared(const MyShared& other) : ptr(other.ptr) {
        if (ptr) {
            ref_counters[ptr]++;
        }
    }
    MyShared(MyShared&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr;
    }

    MyShared& operator=(const MyShared& other) {
        if (this != &other) {
            release();
            ptr = other.ptr;
            if (ptr) {
                ref_counters[ptr]++;
            }
        }
        return *this;
    }

    MyShared& operator=(MyShared&& other) noexcept {
        if (this != &other) {
            release();
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
    T* get() const noexcept {
        return ptr;
    }
    T& operator*() const {
        if (!ptr) throw std::runtime_error("MyShared: null pointer dereference");
        return *ptr;
    }
    T* operator->() const {
        if (!ptr) throw std::runtime_error("MyShared: null pointer access");
        return ptr;
    }
    int count() const noexcept {
        if (!ptr) return 0;
        auto it = ref_counters.find(ptr);
        return (it != ref_counters.end()) ? it->second : 0;
    }
    bool unique() const noexcept {
        return count() == 1;
    }
    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }
    void reset(T* new_ptr = nullptr) {
        release();
        ptr = new_ptr;
        if (ptr) {
            ref_counters[ptr]++;
        }
    }

private:
    void release() noexcept {
        if (ptr) {
            auto it = ref_counters.find(ptr);
            if (it != ref_counters.end()) {
                it->second--;
                if (it->second == 0) {
                    delete ptr;
                    ref_counters.erase(it);
                }
            }
            ptr = nullptr;
        }
    }
};
class Test {
    int id;
public:
    Test(int i) : id(i) { std::cout << "Ctor: " << id << "\n"; }
    ~Test() { std::cout << "Dtor: " << id << "\n"; }
    void show() const { std::cout << "  Value: " << id << "\n"; }
    void set(int i) { id = i; }
    int get() const { return id; }
};

void test_unique() {
    std::cout << "\n[1] Basic operations:\n";
    MyUnique<Test> u1(new Test(10));
    std::cout << "  get(): " << u1.get() << "\n";
    std::cout << "  operator*: "; (*u1).show();
    std::cout << "  operator->: "; u1->show();

    std::cout << "\n[2] Uniqueness check:\n";
    Test* raw = new Test(20);
    MyUnique<Test> u2(raw);
    try {
        MyUnique<Test> u3(raw);
        std::cout << "  ERROR: No exception!\n";
    } catch (const std::runtime_error& e) {
        std::cout << "  OK: Caught: " << e.what() << "\n";
    }

    std::cout << "\n[3] Move constructor:\n";
    MyUnique<Test> u4(new Test(30));
    MyUnique<Test> u5(std::move(u4));
    std::cout << "  u5: "; u5->show();
    std::cout << "  u4.get(): " << u4.get() << " (nullptr)\n";

    std::cout << "\n[4] Move assignment:\n";
    MyUnique<Test> u6(new Test(40));
    MyUnique<Test> u7(new Test(50));
    u7 = std::move(u6);
    std::cout << "  u7: "; u7->show();
    std::cout << "  u6.get(): " << u6.get() << " (nullptr)\n";

    std::cout << "\n[5] release():\n";
    MyUnique<Test> u8(new Test(60));
    Test* released = u8.release();
    std::cout << "  u8.get(): " << u8.get() << " (nullptr)\n";
    std::cout << "  released: "; released->show();
    delete released;

    std::cout << "\n[6] reset():\n";
    MyUnique<Test> u9(new Test(70));
    u9.reset(new Test(80));
    std::cout << "  After reset: "; u9->show();

    std::cout << "\n[7] bool operator:\n";
    MyUnique<Test> u10(new Test(90));
    MyUnique<Test> u11;
    std::cout << "  u10: " << (u10 ? "valid" : "null") << "\n";
    std::cout << "  u11: " << (u11 ? "valid" : "null") << "\n";
}

void test_shared() {
    std::cout << "\n[1] Basic operations:\n";
    MyShared<Test> s1(new Test(100));
    std::cout << "  count: " << s1.count() << "\n";
    s1->show();

    std::cout << "\n[2] Copy constructor:\n";
    MyShared<Test> s2(s1);
    std::cout << "  s1.count: " << s1.count() << "\n";
    std::cout << "  s2.count: " << s2.count() << "\n";

    std::cout << "\n[3] Multiple copies:\n";
    MyShared<Test> s3 = s1;
    std::cout << "  count: " << s1.count() << "\n";

    std::cout << "\n[4] Modification:\n";
    s2->set(200);
    std::cout << "  s1: "; s1->show();
    std::cout << "  s2: "; s2->show();

    std::cout << "\n[5] Copy assignment:\n";
    MyShared<Test> s4;
    s4 = s1;
    std::cout << "  count after assignment: " << s1.count() << "\n";

    std::cout << "\n[6] Move constructor:\n";
    MyShared<Test> s5(new Test(300));
    MyShared<Test> s6(std::move(s5));
    std::cout << "  s6.count: " << s6.count() << "\n";
    std::cout << "  s5.get(): " << s5.get() << " (nullptr)\n";

    std::cout << "\n[7] Move assignment:\n";
    MyShared<Test> s7(new Test(400));
    MyShared<Test> s8(new Test(500));
    s8 = std::move(s7);
    std::cout << "  s8: "; s8->show();
    std::cout << "  s7.get(): " << s7.get() << " (nullptr)\n";

    std::cout << "\n[8] unique() method:\n";
    MyShared<Test> s9(new Test(600));
    std::cout << "  unique: " << (s9.unique() ? "true" : "false") << "\n";
    {
        MyShared<Test> s10(s9);
        std::cout << "  after copy, unique: " << (s9.unique() ? "false" : "true") << "\n";
        std::cout << "  count: " << s9.count() << "\n";
    }
    std::cout << "  after destroy, unique: " << (s9.unique() ? "true" : "false") << "\n";

    std::cout << "\n[9] reset():\n";
    MyShared<Test> s11(new Test(700));
    std::cout << "  before reset count: " << s11.count() << "\n";
    s11.reset(new Test(800));
    std::cout << "  after reset count: " << s11.count() << "\n";

    std::cout << "\n[10] Multiple references:\n";
    MyShared<Test> s12(new Test(900));
    {
        MyShared<Test> s13(s12);
        MyShared<Test> s14(s13);
        std::cout << "  inside block count: " << s12.count() << "\n";
    }
    std::cout << "  outside block count: " << s12.count() << "\n";

    std::cout << "\n[11] nullptr:\n";
    MyShared<Test> s15(nullptr);
    std::cout << "  count: " << s15.count() << "\n";
    std::cout << "  valid: " << (s15 ? "true" : "false") << "\n";

    std::cout << "\n[12] Chained assignment:\n";
    MyShared<Test> s16(new Test(1000));
    MyShared<Test> s17(new Test(2000));
    MyShared<Test> s18(new Test(3000));
    s18 = s17 = s16;
    std::cout << "  all count: " << s16.count() << " (should be 3)\n";
}
int main() {
    test_unique();
    test_shared();
    return 0;
}