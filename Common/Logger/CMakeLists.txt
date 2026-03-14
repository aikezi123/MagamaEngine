# ==========================================
# Magma Engine - Common/Logger 模块
# ==========================================
# 1. 引入 CMake 的全自动包管理黑科技
include(FetchContent)

# 2. 自动下载并集成 C++ 工业界最强日志库 spdlog
# 【架构威力】：无论谁克隆了你的引擎代码，都不需要手动去配环境，
# CMake 会在第一次生成时自动去 Github 拉取源码并编译，实现真正的“开箱即用”！
message(STATUS "MagmaEngine: Fetching spdlog...")
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG        v1.13.0
  GIT_SHALLOW    TRUE    # 【关键】只下载当前版本，不下载历史，体积小很多
  GIT_PROGRESS   TRUE    # 【关键】在输出窗口显示百分比进度，不再死等
)
FetchContent_MakeAvailable(spdlog)

# 3. 递归搜索当前Logger目录下的代码
file(GLOB_RECURSE LOGGER_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE LOGGER_HEADERS CONFIGURE_DEPENDS "include/*.h")

# 4. 生成底层静态库
add_library(Magma_Common_Logger STATIC ${LOGGER_SOURCES} ${LOGGER_HEADERS})

# 5. 暴露include目录，给外部使用
target_include_directories(Magma_Common_Logger
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 6. 将spdlog绑定到Logger库上
target_link_libraries(Magma_Common_Logger 
    PUBLIC spdlog::spdlog
)