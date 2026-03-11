file(GLOB_RECURSE SOURCES "*.cpp")
file(GLOB_RECURSE HEADERS "*.h")
if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

add_library(Magma_Domain SHARED ${SOURCES} ${HEADERS})
target_include_directories(Magma_Domain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 【架构红线】核心层极其纯净，只允许依赖 Common 底座
target_link_libraries(Magma_Domain PUBLIC Magma_Common)