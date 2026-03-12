# ==========================================
# Magma Engine - UI 表现层插件构建脚本 (终极工业版)
# 融合了 OpenCvProject 的全自动寻址黑科技 + 整洁架构规范
# ==========================================

find_package(Qt6 COMPONENTS Core Gui Widgets OpenGLWidgets REQUIRED)
# 1. 开启 Qt 自动化工具
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 2. 穿透性搜集所有的代码、UI 和资源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "Widgets/*.cpp" "InterfaceAdapters/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "Widgets/*.h" "InterfaceAdapters/*.h")
file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS "Widgets/*.ui")
file(GLOB_RECURSE RESOURCES CONFIGURE_DEPENDS "*.qrc")

# 【防报错补丁】防止目录为空时 MSVC 报错
if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "int magma_dummy_ui = 0;")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

# 3. 【核心物理法则】：生成 UI 动态库插件 (SHARED)
add_library(Magma_UI SHARED ${SOURCES} ${HEADERS} ${UI_FILES} ${RESOURCES})

# ==========================================
# 4. 【动态寻址黑科技】：自动解析所有 include 路径并暴露
# ==========================================
set(UI_INCLUDE_DIRS "")
foreach(_header_file ${HEADERS})
    get_filename_component(_dir ${_header_file} DIRECTORY)
    list(APPEND UI_INCLUDE_DIRS ${_dir})
endforeach()

if(UI_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES UI_INCLUDE_DIRS)
    target_include_directories(Magma_UI PUBLIC ${UI_INCLUDE_DIRS})
endif()

# ==========================================
# 5. 【UI 寻址黑科技】：自动定位所有的 .ui 文件给 AUTOUIC
# ==========================================
set(UI_FORM_DIRS "")
foreach(_ui_file ${UI_FILES})
    get_filename_component(_dir ${_ui_file} DIRECTORY)
    list(APPEND UI_FORM_DIRS ${_dir})
endforeach()

if(UI_FORM_DIRS)
    list(REMOVE_DUPLICATES UI_FORM_DIRS)
    set_property(TARGET Magma_UI APPEND PROPERTY AUTOUIC_SEARCH_PATHS ${UI_FORM_DIRS})
endif()

# ==========================================
# 6. 【架构防腐】：向内依赖具体的业务大脑，引入 Qt 巨兽
# ==========================================
target_link_libraries(Magma_UI PUBLIC 
    Magma_App_ImageProcessing   # <--- 【修复】：替换为最新的业务模块名称！
    Qt6::Widgets
    Qt6::OpenGLWidgets
)