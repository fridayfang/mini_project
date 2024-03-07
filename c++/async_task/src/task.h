#ifndef ASYNC_TASK_H
#define ASYNC_TASK_H

#include <string>
#include <memory>
#include <functional>
#include <atomic>
#include "utils/utils_define.h"

namespace async_framework {
class TaskQueue; // 前向聲明
/**
 * @brief 任务参数类
 *
 * 用于存储任务相关的参数。
 */
class TaskParam {
public:
    virtual ~TaskParam() = default;
};

class VoidTaskParam : public TaskParam {
public:
    VoidTaskParam() = default;
    virtual ~VoidTaskParam() = default;

    static VoidTaskParam& value() {
        return *pointer();
    }
    static std::shared_ptr< VoidTaskParam > pointer() {
        static std::shared_ptr< VoidTaskParam > s_void_param = std::make_shared< VoidTaskParam >();
        return s_void_param;
    }
private:
    DISALLOW_COPY_AND_ASSIGN(VoidTaskParam);

};

// 支持任意类型参数
template< typename T >
class TaskParamT : public TaskParam {
public:
    TaskParamT(const T& param) : _param(param) {}
    TaskParamT(T& param): _param(param) {}

    virtual ~TaskParamT() = default;

    T& get() { return _param;} 

private:
    T  _param;
};

class TaskResult {
public:
    virtual ~TaskResult() = default;
};

class VoidTaskResult : public TaskResult {
public:
    VoidTaskResult() = default;
    virtual ~VoidTaskResult() = default;

    static VoidTaskResult& value() {
        return *pointer();
    }
    static std::shared_ptr< VoidTaskResult > pointer() {
        static std::shared_ptr< VoidTaskResult > s_void_param = std::make_shared< VoidTaskResult >();
        return s_void_param;
    }
private:
    DISALLOW_COPY_AND_ASSIGN(VoidTaskResult);
};
template< typename T >
class TaskResultT : public TaskResult {
public:
    TaskResultT(const T& result) : _result(result) {}
    TaskResultT(T& result): _result(result) {}
    virtual ~TaskResultT() = default;
    T& get() { return _result;} 
private:
    T  _result;
};

enum TaskStatus {
    CREATED = 0, // 任务创建成功
    SCHEDULED = 1, // 任务运行中
    WORK_FINISHED = 2, // 任务运行结束,但尚未触发结果回调
    DONE = 3 //任务流程全部执行结束，回调也已执行
};

class Task : public std::enable_shared_from_this<Task> {
public:
    Task(const std::string& name) : _name(name) { }
    virtual ~Task() {};
    void set_param(const std::shared_ptr<TaskParam>& param) { _param = param; }
    std::shared_ptr<TaskParam> get_param() { return _param; }

    // 设置任务的执行队列,　用在延迟执行的场景(即: 创建任务时不主动派发到某个队列，而是加到时间线，然后等到固定时机再执行任务)
    void set_work_queue(const std::weak_ptr<TaskQueue>& queue) {
        _work_queue =  queue; 
    };
    std::weak_ptr<TaskQueue> get_work_queue() { return _work_queue; }

    // 设置任务结果的输出队列，如果不设置，则不会收到on_result的回调(添加到 时间线的task除外)，需要在任务提交到任务队列之前调用
    void set_result_callback_queue(const std::weak_ptr<TaskQueue>& queue) {
        _result_callback_queue = queue; 
    };
    std::weak_ptr<TaskQueue> get_result_callback_queue() { return _result_callback_queue; }

    // 调度到后台队列执行的任务, 会在被调度到的队列执行，一般是在后台线程)， 继承类一定要重写此方法以完成实际任务
    virtual std::shared_ptr<TaskResult> run(std::shared_ptr<TaskParam>) = 0;  

    // 通知任务结果, 在设置的任务回调队列中执行, 如果未设置，则会在工作线程回调
    // 特殊: 当任务被添加到 时间线，如果设置了回调时间的约束，则会按照相应的约束在主线程被回调
    virtual void on_result(std::shared_ptr<TaskResult> result)  {_status.store(TaskStatus::DONE);}

    // 读取当前任务的输出结果
    std::shared_ptr<TaskResult> get_result() {return _result; }

    TaskStatus get_status() const { return _status.load(); }

    // 读取当前任务的名称标识
    const std::string& get_name()  const { return _name; }

    // 执行任务, 如果立即执行，则不会派发到队列，而是马上执行
    // 否则会将任务派发到任务队列执行，前置条件: 需要提前设置好工作队列
    void execute(bool immediately = false);

private:
    // 设置任务的执行结果, 异步框架执行完会自动设置Result
    void set_result(std::shared_ptr<TaskResult> result) {  _result = result;  _status.store(TaskStatus::WORK_FINISHED); }

private:
    std::string  _name;
    std::shared_ptr<TaskParam> _param;
    std::shared_ptr<TaskResult> _result;
    std::weak_ptr<TaskQueue> _work_queue;
    std::weak_ptr<TaskQueue> _result_callback_queue;
    std::atomic<TaskStatus> _status { TaskStatus::CREATED };
    DISALLOW_COPY_AND_ASSIGN(Task);

};
 
}


#endif