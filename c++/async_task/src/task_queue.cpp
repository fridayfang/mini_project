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

const std::shared_ptr<TaskQueue>& GlobalQueue::get_main_queue() {
    static std::shared_ptr<TaskQueue> g_main_queue = std::make_shared<TaskQueue>("main_queue");
    return g_main_queue;
}

std::shared_ptr<TaskQueue> GlobalQueue::get_queue_by_name(const std::string &queue_name){
    auto it = _queue_map.find(queue_name);
    if (it != _queue_map.end()) {
        return it->second;
    } else {
        return nullptr;
    }
}


// 获取默认的全局后台队列
std::shared_ptr<TaskQueue> GlobalQueue::get_background_queue() {
    static std::shared_ptr<TaskQueue> background_queue = std::make_shared<TaskQueue>("background");
    return background_queue;
}

void GlobalQueue::init_queues(const std::vector<std::string>& queue_names) {
    get_main_queue();
    get_background_queue();
    for (auto& name : queue_names) {
        init_queue(name);
    }
}

bool GlobalQueue::init_queue(const std::string& queue_name) {
    if(get_queue_by_name(queue_name) != nullptr) {
        return false;
    }
    auto queue = std::make_shared<TaskQueue>(queue_name);
    _queue_map[queue_name] = queue;
    return true;

}

}