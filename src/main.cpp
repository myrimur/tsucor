// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <csetjmp>
#include <forward_list>
#include "options_parser.h"
#include "picoro.hpp"

class Corout {
public:
    bool resumable();
    void* resume(void *arg);
    void* yield(void* arg);

    void* operator()(void* arg) {
        std::cout << "Corout::operator()(" << arg << ")" << std::endl;
        return arg;
    }
private:
    inline static std::forward_list<std::jmp_buf> running{};
    inline static std::forward_list<std::jmp_buf> idle{};

    void* pass(void* arg);
    void start();
    void main(void*);

};

void* hello(void* arg) {
    for (int i = 0; i < 5; ++i) {
        yield((void*)&i);
    }
    return nullptr;
}

int main(int argc, char* argv[]) {
    auto c = coro_init(&hello);
    while (resumable(c)) {
        int* i = (int*)resume(c, nullptr);
        if (i == nullptr) {
            continue;
        }
        std::cout << *i << std::endl;
    }
    return 0;
}
