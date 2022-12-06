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

    void* pass(const BaseCoro& to, void* arg = nullptr) const;
    void* operator()(void* arg = nullptr);
    static void* yield(void* arg = nullptr);

    static const BaseCoro& first() { return *first_; };

private:
    inline static auto first_ = std::make_unique<BaseCoro>();
    inline static auto running = first_.get();
    inline static std::stack<BaseCoro*> idle{};

    u64* stack_top = nullptr;
    std::unique_ptr<u64[]> stack_alloc{};
};

template <typename T>
class TemplateCoro : public BaseCoro {
public:
    explicit TemplateCoro(CoroFn fn, int stack_size = 1 << 10) : BaseCoro(fn, nullptr, stack_size) {}
    explicit TemplateCoro(CoroFn fn, const T& arg, int stack_size = 1 << 10) : BaseCoro(fn, &arg, stack_size) {}
};

extern "C" void* pass(const BaseCoro* from, const BaseCoro* to, void* arg);

#endif //TSUCOR_CORO_HPP