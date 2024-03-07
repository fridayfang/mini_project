cmake_minimum_required(VERSION 3.10)
project(async_task)

set(CMAKE_CXX_STANDARD 11)

# 添加src目录
add_subdirectory(src)

# 启用测试
# enable_testing()
# add_subdirectory(tests)