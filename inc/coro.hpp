#ifndef TSUCOR_CORO_HPP
#define TSUCOR_CORO_HPP

#include <iostream>
#include <memory>

using CoroFn = void (*)(void* arg);
using u64 = std::uint64_t;

class Coro {
public:
    // Used to make the first coroutine
    Coro() = default;

    Coro(CoroFn fn, void* arg, int stack_size = 1024 * 1024);

    ~Coro() {
        delete[] stack_alloc;
    }

    void* resume(void* arg = nullptr);
    void* yield(void* arg = nullptr);

    inline static std::unique_ptr<Coro> first = std::make_unique<Coro>();
    inline static Coro* running = first.get();

private:
    u64* stack_top = nullptr;  // Top of coroutine's stack
    u64* stack_alloc = nullptr;  // Allocated memory for stack
    Coro* caller = nullptr;
};

extern "C" void* pass(Coro* from, Coro* to, void* arg);

#endif //TSUCOR_CORO_HPP