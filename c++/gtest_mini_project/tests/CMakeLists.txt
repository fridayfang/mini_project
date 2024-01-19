# 下载和解压 Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.10.0.zip
)
FetchContent_MakeAvailable(googletest)

# 添加 Google Test 测试可执行文件
add_executable(test_demo test_demo.cpp)

# 链接测试执行文件与库
target_link_libraries(test_demo gtest_main my_even)

# 包含测试
include(GoogleTest)
gtest_discover_tests(test_demo)
