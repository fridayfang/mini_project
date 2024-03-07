#ifndef ASYNC_TASK_H
#define ASYNC_TASK_H

#include <string>
#include <memory>
#include <functional>
#include <atomic>
#include "utils/utils_define.h"

namespace async_framework {
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

class task {
public:
};
 
}


#endif