# ==========================================
# Magma Engine - Common 层总代理
# ==========================================

# 引入下属的所有微模块
add_subdirectory(Logger)
add_subdirectory(Exceptions) # 等你建了 Exceptions 模块再取消注释

# （可选极客操作）：为了外部调用方便，我们可以建一个虚拟的“集合体”
# 这样外部只需要 target_link_libraries(Magma_UI PRIVATE Magma_Common) 即可，不用挨个写
add_library(Magma_Common INTERFACE)
target_link_libraries(Magma_Common INTERFACE Magma_Common_Logger)