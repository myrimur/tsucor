// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <coro.hpp>

Coro* main_co;
Coro* sub_co;

void sub(void* arg) {
    int i; // random local, to see stack pointer
    int a = 1;
    int b = 2;
    std::cout << "Inside sub-coroutine. Stack=" << &i << std::endl;
    pass(sub_co, main_co, &a); // back to main
    std::cout << "Back in sub. Stack=" << &i << std::endl;
    pass(sub_co, main_co, &b); // back to main
}

int main(int argc, char* argv[]) {
    int i;
    std::cout << "Making coroutines. Stack=" << &i << std::endl;
    main_co = new Coro{};
    sub_co = new Coro{sub, nullptr};
    std::cout << "Switching to coroutine" << std::endl;
    auto a = static_cast<int*>(pass(main_co, sub_co, nullptr));
    std::cout << *a << std::endl;
    std::cout << "Back in main from coroutine. Stack=" << &i << std::endl;
    auto b = static_cast<int*>(pass(main_co, sub_co, nullptr));
    std::cout << *b << std::endl;
    std::cout << "End" << std::endl;
    delete main_co;
    delete sub_co;
    return 0;
}
