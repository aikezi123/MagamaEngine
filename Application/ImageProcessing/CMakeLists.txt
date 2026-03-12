file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.h")

# 【幽灵补丁的妙用】：因为目前只有 .h 契约，没有 .cpp 文件。
# MSVC 如果发现没有代码文件，拒绝生成 STATIC 库。这个补丁完美解决了这个问题！
if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

# 打包成极其轻量级的专属静态库 (内城核心)
add_library(Magma_App_ImageProcessing STATIC ${SOURCES} ${HEADERS})

# 暴露自己专属的 include 目录给外部使用
target_include_directories(Magma_App_ImageProcessing PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 仅仅依赖底座的通用数据结构 (获取 cv::Mat 货币的使用权)
target_link_libraries(Magma_App_ImageProcessing PUBLIC Magma_Common)