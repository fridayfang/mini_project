#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <string>
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <map>

#include "utils/utils_define.h"
#include "task.h"

namespace async_framework {

class TaskQueue : public std::enable_shared_from_this<TaskQueue> {
public:
    TaskQueue(const std::string& queue_name) : _queue_name(queue_name) {}
    ~TaskQueue() = default;

    // 任务入队
    void enqueue(std::shared_ptr<Task> task);

    // 任务出队，如果没有任务则阻塞
    std::shared_ptr<Task> dequeue();

    // 非阻塞版本， 如果队列为空，则返回nullptr
    std::shared_ptr<Task> try_deque();

    const std::string& get_queue_name() const { return _queue_name; }
private:
    std::string _queue_name;

    std::queue<std::shared_ptr<Task>> _queue;

    std::mutex _queue_mutex;
    std::condition_variable _queue_not_empty;

    DISALLOW_COPY_AND_ASSIGN(TaskQueue);
};

}

#endif