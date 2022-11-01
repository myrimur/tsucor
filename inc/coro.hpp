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

    void* pass(const Coro& to, void* arg = nullptr) const;
    void* operator()(void* arg = nullptr);
    static void* yield(void* arg = nullptr);

    static const Coro& first() { return *first_; };

private:
    inline static auto first_ = std::make_unique<Coro>();
    inline static auto running = first_.get();
    inline static std::stack<Coro*> idle{};

    u64* stack_top = nullptr;
    std::unique_ptr<u64[]> stack_alloc{};
};

extern "C" void* pass(const Coro* from, const Coro* to, void* arg);

#endif //TSUCOR_CORO_HPP