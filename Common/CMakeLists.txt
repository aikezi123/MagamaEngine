# 自动搜集文件，每次新增/删除文件自动触发 CMake 重载
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.h")

# 防止空目录报错的幽灵补丁
if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

# 【核心红线】：打成静态库，提供极致性能
add_library(Magma_Common STATIC ${SOURCES} ${HEADERS})
target_include_directories(Magma_Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 链接核心货币 (cv::Mat)
target_link_libraries(Magma_Common PUBLIC opencv_core)