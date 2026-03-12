file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.h")

if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

# 【核心红线】：业务灵魂，必须是静态库，物理隐身
add_library(Magma_Domain STATIC ${SOURCES} ${HEADERS})
target_include_directories(Magma_Domain PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# 纯净依赖：只认识底座
target_link_libraries(Magma_Domain PUBLIC Magma_Common)