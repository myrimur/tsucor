#include "coro.hpp"

std::unique_ptr<Coro> Coro::running{new Coro{}};

Coro::Coro(CoroFn fn, void* arg, int stack_size) {
    stack_alloc = new u64[stack_size / sizeof(u64)];
    stack_top = stack_alloc + stack_size / sizeof(u64) - 1;

    // Used as return address to start coroutine when pass is called for the first time
    *(--stack_top) = reinterpret_cast<u64>(fn);
    *(--stack_top) = reinterpret_cast<u64>(arg);  // rdi
    stack_top -= 6;  // Space for rbp, rbx, r12, r13, r14, r15
}

void Coro::resume() {
    auto temp = Coro::running.get();
    Coro::running{this};
    pass(Coro::running.get(), this);
}