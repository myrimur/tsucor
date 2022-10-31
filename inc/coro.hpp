#ifndef TSUCOR_CORO_HPP
#define TSUCOR_CORO_HPP

#include <iostream>

using CoroFn = void (*)(void* arg);
using u64 = std::uint64_t;

class Coro {
public:
    // Used to make main into a coroutine
    Coro() = default;

    Coro(CoroFn fn, void* arg, int stack_size = 1024 * 1024) {
        stack_alloc = new u64[stack_size / sizeof(u64)];
        stack_top = stack_alloc + stack_size / sizeof(u64) - 1;

        // Used as return address to start coroutine when pass is called for the first time
        *(--stack_top) = reinterpret_cast<u64>(fn);
        *(--stack_top) = reinterpret_cast<u64>(arg);  // rdi
        stack_top -= 6;  // Space for rbp, rbx, r12, r13, r14, r15
    }

    ~Coro() {
        delete[] stack_alloc;
    }

private:
    u64* stack_top = nullptr;  // Top of coroutine's stack
    u64* stack_alloc = nullptr;  // Allocated memory for stack
};

extern "C" void* pass(Coro* from, Coro* to, void* arg);

#endif //TSUCOR_CORO_HPP