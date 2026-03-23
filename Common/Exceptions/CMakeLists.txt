# ==========================================
# Magma Engine - Common/Exceptions 模块
# ==========================================

# 1. 递归搜索当前 Exceptions 目录下的头文件 (.hpp / .h)
# 这一步主要是为了让 IDE（如 Visual Studio 或 CLion）的解决方案资源管理器里能显示这些文件
file(GLOB_RECURSE EXCEPTION_HEADERS CONFIGURE_DEPENDS "include/*.hpp" "include/*.h")

# 2. 生成 Header-only 接口库 (【关键架构节点】)
# 因为异常类全部由模板和内联函数实现，没有 .cpp 文件，所以绝对不能用 STATIC。
# INTERFACE 的作用是告诉 CMake：“不要尝试把这个模块编译成实体静态库，它只是一个接口契约。”
add_library(Magma_Common_Exceptions INTERFACE)

# 3. 暴露 include 目录，给外部使用
# 注意这里必须使用 INTERFACE 而不是 PUBLIC。
# 意思是：谁链接了我 (Magma_Common_Exceptions)，谁就会自动获得这个 include 搜索路径。
target_include_directories(Magma_Common_Exceptions
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 4. 将头文件附加到目标上 (为了极佳的 IDE 体验)
# 对于 INTERFACE 库，如果不做这一步，VS 的项目列表里可能找不到头文件。
# 将头文件作为 PUBLIC HEADER 附加，可以让代码结构在 IDE 中完美展现。
target_sources(Magma_Common_Exceptions 
    INTERFACE ${EXCEPTION_HEADERS}
)