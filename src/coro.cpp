#include "coro.hpp"

Coro::Coro(CoroFn fn, void* arg, int stack_size) {
    stack_alloc = std::make_unique<u64[]>(stack_size / sizeof(u64));
    stack_top = stack_alloc.get() + stack_size / sizeof(u64) - 1;

    // Used as return address to start coroutine when pass is called for the first time
    *(--stack_top) = reinterpret_cast<u64>(fn);
    *(--stack_top) = reinterpret_cast<u64>(arg);  // rdi
    stack_top -= 6;  // Space for rbp, rbx, r12, r13, r14, r15
}

void* Coro::pass(const Coro& to, void* arg) const {
    return ::pass(this, &to, arg);
}

void* Coro::operator()(void* arg) {
    idle.push(running);
    running = this;
    return ::pass(idle.top(), this, arg);
}

void* Coro::yield(void* arg) {
    auto from = running;
    running = idle.top();
    idle.pop();
    return ::pass(from, running, arg);
}
