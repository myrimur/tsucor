#ifndef TSUCOR_CORO_HPP
#define TSUCOR_CORO_HPP

#include <iostream>
#include <memory>
#include <stack>

using CoroFn = void (*)(void* arg);
using u64 = std::uint64_t;

class BaseCoro {
public:
    BaseCoro() = default;  // Used to make the first coroutine
    explicit BaseCoro(CoroFn fn, void* arg, int stack_size);

    BaseCoro(const BaseCoro&) = delete;
    BaseCoro& operator=(const BaseCoro&) = delete;
    BaseCoro(BaseCoro&&) = delete;
    ~BaseCoro() = default;

protected:
    inline static auto first_ = std::make_unique<BaseCoro>();
    inline static auto running_ = first_.get();

private:
    u64* stack_top_ = nullptr;
    std::unique_ptr<u64[]> stack_alloc_{};
};

class SymCoro: public BaseCoro {
public:
    explicit SymCoro(CoroFn fn, void* arg = nullptr, int stack_size = 1 << 10): BaseCoro(fn, arg, stack_size) {};

    // Yields to main
    static void* yield(void* arg = nullptr);

    void* operator()(void* arg = nullptr);
};

class AsymCoro: public BaseCoro {
public:
    explicit AsymCoro(CoroFn fn, void* arg = nullptr, int stack_size = 1 << 10): BaseCoro(fn, arg, stack_size) {};

    // Yields to caller
    static void* yield(void* arg = nullptr);

    void* operator()(void* arg = nullptr);

private:
    inline static std::stack<BaseCoro*> idle_{};
};

extern "C" void* pass(const BaseCoro* from, const BaseCoro* to, void* arg);

#endif //TSUCOR_CORO_HPP