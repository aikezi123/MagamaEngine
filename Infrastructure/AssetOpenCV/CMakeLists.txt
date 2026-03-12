file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.h")

if(NOT SOURCES)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp "")
    list(APPEND SOURCES ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)
endif()

# 打包为独立的动态库插件 (外城重装甲)
add_library(Magma_Infra_AssetOpenCV SHARED ${SOURCES} ${HEADERS})

target_include_directories(Magma_Infra_AssetOpenCV PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 【核心架构防腐】：精确制导！只链接图像处理契约库，绝不污染其他业务！
# 同时引入 OpenCV 的重型算法引擎
target_link_libraries(Magma_Infra_AssetOpenCV PUBLIC 
    Magma_App_ImageProcessing 
    opencv_imgcodecs
    opencv_imgproc
)