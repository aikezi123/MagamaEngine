file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

add_library(Magma_Application SHARED ${SOURCES} ${HEADERS})
target_include_directories(Magma_Application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 【架构红线】大脑只依赖内部的 Domain
target_link_libraries(Magma_Application PUBLIC Magma_Domain)