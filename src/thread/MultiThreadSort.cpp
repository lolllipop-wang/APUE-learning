#include <cstdio>
#include <mutex>
#include <thread>
#include <future>
#include <random>
#include <iostream>
#include <algorithm>
#include <condition_variable>

#define MAX_NUMBER_COUNT 8000000
#define MAX_THREAD_COUNT 8
#define MAX_NUMBER_PER_THREAD (MAX_NUMBER_COUNT / MAX_THREAD_COUNT)

int arr[MAX_NUMBER_COUNT];
int arrs[MAX_NUMBER_COUNT];
int indexs[MAX_THREAD_COUNT];
std::future<bool> futures[MAX_THREAD_COUNT];

bool Worker(int *begin, int *end)
{
    std::sort(begin, end);
    return true;
}

void Merge()
{
    int x = 0;
    for (int i = 0; i < MAX_THREAD_COUNT; ++i)
        indexs[i] = 0;
    
    for (int i = 0; i < MAX_NUMBER_COUNT; ++i) {
        int pos = -1;
        int minn = (1 << 30);
        for (int j = 0; j < MAX_THREAD_COUNT; ++j) {
            if (indexs[j] < MAX_NUMBER_PER_THREAD) {
                if (minn > arr[indexs[j] + j * MAX_NUMBER_PER_THREAD]) {
                    minn = arr[indexs[j] + j * MAX_NUMBER_PER_THREAD];
                    pos = j;
                }
            }
        }
        indexs[pos]++;
        arrs[x++] = minn;
    }
    memcpy(arr, arrs, sizeof(int) * MAX_NUMBER_COUNT);
}

void Print()
{
    for (int i = 0; i < MAX_NUMBER_COUNT; ++i) {
        printf("%-8d", arr[i]);

        if ((i + 1) % MAX_THREAD_COUNT == 0)
            std::cout << std::endl;
    }
}

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < MAX_NUMBER_COUNT; ++i)
        arr[i] = dis(gen);
    
    for (int i = 0; i < MAX_THREAD_COUNT; ++i) {
        futures[i] = std::async(
            Worker,
            arr + (i * MAX_NUMBER_PER_THREAD),
            arr + (i * MAX_NUMBER_PER_THREAD) + MAX_NUMBER_PER_THREAD
        );
    }

    for (int i = 0; i < MAX_THREAD_COUNT; ++i) {
        futures[i].get();
    }

    Merge();
    Print();
    return 0;
}
