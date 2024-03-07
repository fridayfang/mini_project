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

class GlobalQueue {
public:
// 获取主队列
static const std::shared_ptr<TaskQueue>& get_main_queue();

static bool is_main_queue(const std::shared_ptr<TaskQueue>& queue) { return queue == get_main_queue(); }

// 根据名称获取子队列，如果队列不存在，则返回nullptr
static std::shared_ptr<TaskQueue> get_queue_by_name(const std::string &queue_name);

// 获取默认的全局后台队列
static std::shared_ptr<TaskQueue> get_background_queue();

//  初始化所有队列
static void init_queues(const std::vector<std::string>& queue_names);
// 初始化某个队列
static bool init_queue(const std::string& queue_name);

private:
    GlobalQueue() {};
    ~GlobalQueue() {};
    static std::map<std::string, std::shared_ptr<TaskQueue>> _queue_map;
    DISALLOW_COPY_AND_ASSIGN(GlobalQueue);
};

}

#endif