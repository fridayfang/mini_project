#include "task_queue.h"

#include <thread>
#include <iostream>

namespace async_framework {

void TaskQueue::enqueue(std::shared_ptr<Task> task) {
    std::unique_lock<std::mutex> lock(_queue_mutex);
    _queue.emplace(task);
    task->set_work_queue(this->shared_from_this());
    _queue_not_empty.notify_one();// 通知隊列不空
}

std::shared_ptr<Task> TaskQueue::dequeue() {
    std::unique_lock<std::mutex> lock(_queue_mutex);

    // while(_queue.empty()) {
    //     _queue_not_empty.wait(lock);
    // }

    _queue_not_empty.wait(lock, [this] {
        return !_queue.empty();
    });

    auto task = _queue.front();
    _queue.pop();
    return task;
}

std::shared_ptr<Task> TaskQueue::try_deque() {
    std::unique_lock<std::mutex> lock(_queue_mutex);
    if (_queue.empty()) {
        return nullptr;
    }
    
    auto task = _queue.front();
    _queue.pop();
    return task;
}

}