#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <cmath>
#include <limits.h>
uint64_t is_prime(uint64_t a)
{
    uint64_t i;
    if (a == 2) {
        return 1;
    }
    if (a == 0 || a == 1 || a % 2 == 0) {
        return 0;
    }
    for (i = 3; i*i <= a && a % i; i += 2) { };
    return i * i > a;
}

void prime(uint64_t low, uint64_t high, int32_t count, std::vector<std::promise<uint64_t>> promises) {
    int32_t k = 0;
    for (uint64_t i = low; i <= high && count; i++) {
        if (is_prime(i)) {
            count--;
            promises[k++].set_value(i); 
        }
        if (i == ULLONG_MAX) {
            break;
        }
    }
    if (count) {
        promises[k].set_exception(std::make_exception_ptr(high));
    }
}
int main()
{
    uint64_t low, high;
    int32_t count;   
    std::cin >> low >> high >> count;
    std::vector<std::promise<uint64_t>> promises(count);
    std::vector<std::future<uint64_t>> futures(count);
    for (int32_t i = 0; i < count; i++) {
        futures[i] = promises[i].get_future();
    }
    std::thread(prime, low, high, count, std::move(promises)).detach();
    try {
        for (int32_t i = 0; i < count; i++) {
            futures[i].wait();
            std::cout << futures[i].get() << std::endl;
        }
    } catch(uint64_t t) {
        std::cout << t << std::endl;
    }
}