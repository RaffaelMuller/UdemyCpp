#include <cstdint>
#include <iostream>
#include <mutex>
#include <thread>

#include "Timer.h"

constexpr std::uint32_t NUM_THREADS = 3;
constexpr std::uint32_t NUM_ITERATIONS = 100000;

std::int32_t global_counter = 0;
std::mutex mutex;

void function(const int input, int &output)
{
    printf("Argument: %d\n", input);

    for (std::uint32_t i = 0; i < NUM_ITERATIONS; ++i)
    {
        std::lock_guard<std::mutex> guard(mutex);
        ++global_counter;
    }

    output = input * 2;
}

int main()
{
    std::thread threads[NUM_THREADS];
    std::int32_t inputs[NUM_THREADS]{};
    std::int32_t outputs[NUM_THREADS]{};

    for (std::uint32_t i = 0; i < NUM_THREADS; ++i)
    {
        inputs[i] = i;
        outputs[i] = 0;
    }

    cpptiming::Timer timer;

    for (std::uint32_t i = 0; i < NUM_THREADS; ++i)
    {
        threads[i] = std::thread{function, inputs[i], std::ref(outputs[i])};
    }

    // ...

    for (std::uint32_t i = 0; i < NUM_THREADS; ++i)
    {
        threads[i].join();
    }

    std::cout << "Time: " << timer.elapsed_time<cpptiming::microsecs, double>() << "ms." << std::endl;

    for (std::uint32_t i = 0; i < NUM_THREADS; ++i)
    {
        printf("Result: %d\n", outputs[i]);
    }

    return 0;
}