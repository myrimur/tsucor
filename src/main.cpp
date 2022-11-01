// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <coro.hpp>

void sub(void* arg);

Coro sub_co{sub};

void sub(void* arg) {
    std::cout << "Inside sub" << std::endl;
    sub_co.pass(Coro::first());
    std::cout << "Back in sub" << std::endl;
    sub_co.pass(Coro::first());
}

void sub2(void* arg) {
    std::cout << "Inside sub2" << std::endl;
    Coro::yield();
    std::cout << "Back in sub2" << std::endl;
    Coro::yield();
}

void sub1(void* arg) {
    std::cout << "Inside sub1" << std::endl;
    Coro sub2_co{sub2};
    sub2_co();
    Coro::yield();
    std::cout << "Back in sub1" << std::endl;
    sub2_co();
    Coro::yield();
}

void gen(void* arg) {
    for (int i = 0; i < *static_cast<int*>(arg); ++i) {
        Coro::yield(&i);
    }
    Coro::yield(nullptr);
}

int main() {
    std::cout << "Symmetric coroutines" << std::endl;
    std::cout << "Switching to sub" << std::endl;
    Coro::first().pass(sub_co);
    std::cout << "Back in main from sub" << std::endl;
    Coro::first().pass(sub_co);
    std::cout << "End in main\n" << std::endl;

    std::cout << "Asymmetric coroutines" << std::endl;
    Coro sub1_co{sub1};
    std::cout << "Switching to sub1" << std::endl;
    sub1_co();
    std::cout << "Back in main from sub1" << std::endl;
    sub1_co();
    std::cout << "End in main\n" << std::endl;

    std::cout << "Generator" << std::endl;
    int n = 5;
    Coro gen_co{gen, &n};
    while (true) {
        auto i = static_cast<int*>(gen_co());
        if (i == nullptr) {
            break;
        }
        std::cout << *i << std::endl;
    }
    return 0;
}
