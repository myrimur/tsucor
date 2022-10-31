#ifndef TSUCOR_CORO_HPP
#define TSUCOR_CORO_HPP

#include <iostream>
#include <memory>

using CoroFn = void (*)(void* arg);
using u64 = std::uint64_t;

class Coro {
public:
    static std::unique_ptr<Coro> running;
    // Used to make main into a coroutine
    Coro() = default;

    Coro(CoroFn fn, void* arg, int stack_size = 1024 * 1024);

    ~Coro() {
        delete[] stack_alloc;
    }

    void resume();

private:
    u64* stack_top = nullptr;  // Top of coroutine's stack
    u64* stack_alloc = nullptr;  // Allocated memory for stack
};

extern "C" void pass(Coro* from, Coro* to);

#endif //TSUCOR_CORO_HPP