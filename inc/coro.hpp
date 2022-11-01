#ifndef TSUCOR_CORO_HPP
#define TSUCOR_CORO_HPP

#include <iostream>
#include <memory>
#include <stack>

using CoroFn = void (*)(void* arg);
using u64 = std::uint64_t;

class Coro {
public:
    Coro() = default;  // Used to make the first coroutine
    explicit Coro(CoroFn fn, void* arg = nullptr, int stack_size = 1 << 10);

    Coro(const Coro&) = delete;
    Coro& operator=(const Coro&) = delete;
    Coro(Coro&&) = delete;
    ~Coro() = default;

    void* pass(Coro* to, void* arg = nullptr);
    void* operator()(void* arg = nullptr);
    static void* yield(void* arg = nullptr);

    inline static std::unique_ptr<Coro> first = std::make_unique<Coro>();

private:
    inline static Coro* running = first.get();
    inline static std::stack<Coro*> idle{};

    u64* stack_top = nullptr;  // Top of coroutine's stack
    std::unique_ptr<u64[]> stack_alloc{};  // Allocated memory for stack
};

extern "C" void* pass(Coro* from, Coro* to, void* arg);

#endif //TSUCOR_CORO_HPP