#ifndef WORK_THREAD_H
#define WORK_THREAD_H

#include <string>
#include <memory>
#include <atomic>
#include <thread>

#include "utils/utils_define.h"
#include "task.h"
#include "task_queue.h"

namespace async_framework {

class WorkThread {
public:
    WorkThread(const std::string &name, const std::shared_ptr<TaskQueue>& queue)
    : _name(name)
    , _queue(queue){};

    virtual ~WorkThread();
    void start(bool new_os_thread = true);

    // 判断当前正在执行的线程是否是主线程
    static bool is_main_thread() { 
        return _s_main_thread_id == std::this_thread::get_id(); 
    }

    static void set_main_thread_id(const std::thread::id& main_thread_id) {
        _s_main_thread_id = main_thread_id;
    }
protected:
    // 从队列中获取一个任务并处理，返回是否获取到任务，wait表示如果没有任务是否阻塞当前线程
    virtual bool process_one_task(bool wait  = false);
    
    // 线程的主循环, 默认实现为如果队列为空，则阻塞，否则处理任务 
    virtual void main_loop();
protected:
    std::string  _name;
    std::shared_ptr<TaskQueue> _queue;
    std::atomic<bool> _running;
    std::shared_ptr<std::thread> _thread; //
    static std::thread::id _s_main_thread_id; // _s_main_thread_id 所有对象共享，记录主线程id
private:
    DISALLOW_COPY_AND_ASSIGN(WorkThread);
};

}

#endif