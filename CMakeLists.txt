cmake_minimum_required(VERSION 3.20)

# 定义引擎名称与版本 
project(MagmaEngine VERSION 0.1.0 LANGUAGES CXX)

# ==========================================
# 1. 全局编译规范 (融合了你的防乱码大法)
# ==========================================
# 彻底消灭 MSVC 下的 C4828 中文乱码警告，并让界面正确显示中文
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ==========================================
# 2. 动态库 (DLL) 全局统一策略 (完美借鉴)
# ==========================================
# 强行导出 Windows DLL 的所有符号，保持领域层代码的绝对干净，无需手写 dllexport
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# 将所有的 .exe (RUNTIME) 和 .dll (RUNTIME/LIBRARY) 统一输出到 build/bin 目录下
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ==========================================
# 3. 全局工具配置 (Qt 专属自动化)
# ==========================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# ==========================================
# 4. 寻找外部重型武器
# ==========================================
# Qt6 (根据你的环境，确保 CMAKE_PREFIX_PATH 已经配置或使用了环境变量)
find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenGL REQUIRED)

# ==========================================
# 5. 分封诸侯 (严格遵循整洁架构的依赖倒置顺序)
# 注意：这里未来我们会把它们都配置成 SHARED（动态库）
# ==========================================
add_subdirectory(Common)              # 第0层：基础货币
add_subdirectory(Domain)              # 第1层：领域内核
add_subdirectory(Application)         # 第2层：调度大脑
add_subdirectory(InterfaceAdapters)   # 第3层：翻译官 (如果暂时没代码可以先注释掉)
add_subdirectory(Infrastructure)      # 第4层：底层设施
add_subdirectory(UI)                  # 第5层：表现层插件

# ==========================================
# 6. 上帝之手：组装车间与生成 .exe
# ==========================================
add_executable(${PROJECT_NAME} WIN32 main.cpp)

# 目前只链接 Qt，等子模块的微型 CMake 补齐后，我们再把它们链进来
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets          
    Magma_UI              # <--- 取消注释！链接最外层的 UI
    Magma_Infrastructure  # <--- 取消注释！链接最外层的底层实现
)