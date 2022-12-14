#include "coro.hpp"

BaseCoro::BaseCoro(CoroFn fn, void* arg, int stack_size) {
    stack_alloc_ = new u64[stack_size / sizeof(u64)];
    stack_top_ = stack_alloc_ + stack_size / sizeof(u64) - 1;

    // Used as return address to start coroutine when pass is called for the first time
    *(--stack_top_) = reinterpret_cast<u64>(fn);
    *(--stack_top_) = reinterpret_cast<u64>(arg);  // rdi
    stack_top_ -= 6;  // Space for rbp, rbx, r12, r13, r14, r15
}

BaseCoro& BaseCoro::operator=(BaseCoro&& other) noexcept {
    stack_top_ = std::exchange(other.stack_top_, nullptr);
    stack_alloc_ = std::exchange(other.stack_alloc_, nullptr);
    return *this;
}

void* SymCoro::yield(void* arg) {
    auto from = running_;
    running_ = first_.get();
    return ::pass(from, running_, arg);
}

void* SymCoro::operator()(void* arg) {
    auto from = running_;
    running_ = this;
    return ::pass(from, running_, arg);
}

void* AsymCoro::yield(void* arg) {
    auto from = running_;
    running_ = idle_.top();
    idle_.pop();
    return ::pass(from, running_, arg);
}

void* AsymCoro::operator()(void* arg) {
    idle_.push(running_);
    running_ = this;
    return ::pass(idle_.top(), this, arg);
}
