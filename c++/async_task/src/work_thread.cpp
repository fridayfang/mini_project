#include "work_thread.h"
#include "work_queue.h"
#include "task.h"

#include <iostream>

namespace async_framework
{
std::thread::id WorkThread::_s_main_thread_id;

void WorkThread::main_loop() {
    while (_running) {
        process_one_task(true);
    }
}

bool WorkThread::process_one_task(bool wait) {
        std::shared_ptr<Task> task;
        if (!wait) {
            //非阻塞
            task = _queue->try_deque( );
            if (task == nullptr) {
                return false;
            }
        } else {
            // 获取任务，如果没有任务，则阻塞当前线程
            task = _queue->dequeue();
        }

        if (task == nullptr) {
            return false;
        }
        bool immediate = true;
        task->execute(immediate);
        return true;
}

void WorkThread::start(bool new_os_thread) {
    _running .store(true);
    if (new_os_thread) {
        _thread = std::make_shared<std::thread>(&WorkThread::main_loop, this);
        auto native_thread = _thread->native_handle();
        pthread_setname_np(native_thread,  _name.c_str());
    } else {
        main_loop();
    }
}
    
} // namespace async_framework
