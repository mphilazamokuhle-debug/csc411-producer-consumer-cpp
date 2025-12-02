#include "Buffer.h"

void Buffer::put(int item) {
    std::unique_lock<std::mutex> lock(mtx);
    notFull.wait(lock, [&]{ return q.size() < MAX_SIZE; });
    q.push(item);
    notEmpty.notify_one();
}
int Buffer::take() {
    std::unique_lock<std::mutex> lock(mtx);
    notEmpty.wait(lock, [&]{ return !q.empty(); });
    int item = q.front();
    q.pop();
    notFull.notify_one();
    return item;
}