# ==========================================
# Magma Engine - Domain/Entities 模块
# ==========================================

# 1. 递归搜索当前 Entities 目录下的代码
file(GLOB_RECURSE ENTITY_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE ENTITY_HEADERS CMAKE_CONFIGURE_DEPENDS "include/*.h")

# 2. 生成底层静态库
add_library(Magma_Domain_Entities STATIC ${ENTITY_SOURCES} ${ENTITY_HEADERS})

# 3. 暴露include目录，给外部使用
target_include_directories(Magma_Domain_Entities
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 4. 链接底座层 (Domain 层唯一允许依赖的外部模块)
# 假设你在最外层汇总了 Common，这里可以直接链接
# target_link_libraries(Magma_Domain_Entities PRIVATE Magma_Common)