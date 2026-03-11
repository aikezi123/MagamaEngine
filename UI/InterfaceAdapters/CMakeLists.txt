file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

add_library(Magma_InterfaceAdapters SHARED ${SOURCES} ${HEADERS})
target_include_directories(Magma_InterfaceAdapters PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 【架构红线】翻译官向内依赖 Application
target_link_libraries(Magma_InterfaceAdapters PUBLIC Magma_Application)