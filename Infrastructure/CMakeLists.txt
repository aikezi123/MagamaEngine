file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

add_library(Magma_Infrastructure SHARED ${SOURCES} ${HEADERS})
target_include_directories(Magma_Infrastructure PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 【架构红线】苦力层依赖 Application（为了继承虚函数），并引入外部重型武器
target_link_libraries(Magma_Infrastructure PUBLIC 
    Magma_Application 
    opencv_imgcodecs
    opencv_imgproc
    OpenGL::GL
)