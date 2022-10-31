// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com

#include <iostream>
#include <coro.hpp>

//std::unique_ptr<Coro> main_co;
std::unique_ptr<Coro> sub_co;

void sub(void* arg) {
    int i; // random local, to see stack pointer
    std::cout << "Inside sub-coroutine. Stack=" << &i << std::endl;
    sub_co->yield();
//    main_co->resume();
//    pass(sub_co.get(), main_co.get(), nullptr); // back to main
    std::cout << "Back in sub. Stack=" << &i << std::endl;
//    pass(sub_co.get(), main_co.get(), nullptr); // back to main
    //main_co->resume();
    sub_co->yield();
}

int main(int argc, char* argv[]) {
    int i;
    std::cout << "Making coroutines. Stack=" << &i << std::endl;
//    main_co = std::make_unique<Coro>();
    sub_co = std::make_unique<Coro>(sub, nullptr);
    std::cout << "Switching to coroutine" << std::endl;
    sub_co->resume();

//    pass(main_co.get(), sub_co.get(), nullptr);
    std::cout << "Back in main from coroutine. Stack=" << &i << std::endl;
    sub_co->resume();
//    pass(main_co.get(), sub_co.get(), nullptr);
    std::cout << "End" << std::endl;

    return 0;
}
