#pragma once

#include <exception>
#include <memory>
#include <string>
#include <format>
#include <utility>

// 严格遵守三段式命名空间: Magma(Root) :: Common(Layer) :: Exception(Feature)
namespace Magma::Common::Exception {

    class MagmaException : public std::exception {
    private:
        // 核心基石：使用 shared_ptr 管理堆区字符串
        // 保证在 catch 栈展开拷贝时，仅仅是引用计数 +1，绝对不会抛出 bad_alloc
        std::shared_ptr<std::string> message_;

    public:
        // ==========================================
        // 1. 基础构造函数
        // ==========================================

        // 接受 std::string
        explicit MagmaException(const std::string& message)
            : message_(std::make_shared<std::string>(message)) {
        }

        // 接受 C 风格字符串 (兜底 nullptr 防止崩溃)
        explicit MagmaException(const char* message)
            : message_(std::make_shared<std::string>(message ? message : "Unknown Magma Error")) {
        }

        // ==========================================
        // 2. 格式化模板构造函数 (C++20)
        // ==========================================

        // 允许类似: throw MagmaException("Port {} is used by {}", 8080, "System");
        template<typename... Args>
        explicit MagmaException(std::format_string<Args...> format_str, Args&&... args)
            : message_(std::make_shared<std::string>(
                std::format(format_str, std::forward<Args>(args)...)
            )) {
        }

        // ==========================================
        // 3. 生命周期管理 (Rule of Zero / Default)
        // ==========================================

        // 编译器会自动生成拷贝/移动构造函数。
        // 因为 shared_ptr 的拷贝/移动是 noexcept 的，所以我们的异常类也是绝对安全的。

        // ==========================================
        // 4. 标准接口重写
        // ==========================================

        // 必须被重写的 what() 函数，签名必须带有 noexcept
        [[nodiscard]] const char* what() const noexcept override {
            // 如果 message_ 有效，返回底层 C 字符串指针；否则返回兜底信息
            // 这个指针的生命周期与 shared_ptr 绑定，在 catch 块内绝对有效且安全
            return message_ ? message_->c_str() : "MagmaException: Unknown Error";
        }
    };

} // namespace magma