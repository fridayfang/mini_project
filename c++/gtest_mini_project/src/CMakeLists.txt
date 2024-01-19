# 创建一个库
add_library(my_even my_even.cpp)

# 指定这个库的头文件路径
target_include_directories(my_even PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
