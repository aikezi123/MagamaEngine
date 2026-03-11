# 自动搜集当前目录下所有的 .cpp 和 .h 文件
file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")

# 【黑科技补丁】如果没写代码，自动建一个假的空文件防止 CMake 报错
if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

# 编译为共享动态库 (DLL)
add_library(Magma_Common SHARED ${SOURCES} ${HEADERS})

# 向外暴露自己的头文件目录
target_include_directories(Magma_Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 链接全局“基础货币”
target_link_libraries(Magma_Common PUBLIC opencv_core)