file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE UI_FILES "*.ui") # 额外搜集 Qt 的界面文件

if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

add_library(Magma_UI SHARED ${SOURCES} ${HEADERS} ${UI_FILES})
target_include_directories(Magma_UI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 【架构红线】表现层向内依赖翻译官（或直接依赖大脑），并强依赖 Qt
target_link_libraries(Magma_UI PUBLIC 
    Magma_InterfaceAdapters 
    Qt6::Widgets
    Qt6::OpenGLWidgets
)