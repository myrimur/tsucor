#include "coro.hpp"

Coro::Coro(CoroFn fn, void* arg, int stack_size) {
    caller = running;

    stack_alloc = new u64[stack_size / sizeof(u64)];
    stack_top = stack_alloc + stack_size / sizeof(u64) - 1;

    // Used as return address to start coroutine when pass is called for the first time
    *(--stack_top) = reinterpret_cast<u64>(fn);
    *(--stack_top) = reinterpret_cast<u64>(arg);  // rdi
    stack_top -= 6;  // Space for rbp, rbx, r12, r13, r14, r15
}

void* Coro::resume(void* arg) {
    running = this;
    return pass(caller, this, arg);
}

void* Coro::yield(void* arg) {
    running = caller;
    return pass(this, caller, arg);
}
