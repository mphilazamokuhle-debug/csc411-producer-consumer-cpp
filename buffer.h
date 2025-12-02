#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

class Buffer {
    private:
    std::queue<int> q;
    const size_t MAX_SIZE = 10;
    std::mutex mtx;
    std::condition_variable notFull, notEmpty;

    public:
    void put(int value);
    int take();
};