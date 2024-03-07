file(GLOB_RECURSE SOURCE_FILES "*.cpp")

add_library(async_task SHARED ${SOURCE_FILES})

# 指定这个库的头文件路径
target_include_directories(async_task PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})