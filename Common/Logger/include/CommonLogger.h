#pragma once

// 我们封装的引擎级头文件，外界只需 include 这个文件
#include <memory>
#include <string>

// 引入 spdlog 的核心头文件
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h> // 支持自定义类型的 {} 输出

namespace Magma::Common {

    class Logger {
    public:
        // 单例模式，保证全局唯一的日志大脑
        static void Init();
        static std::shared_ptr<spdlog::logger>& GetCoreLogger();

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };

}

// =========================================================================
// 【终极宏定义】：劫持编译器上下文，自动填入 文件名、行号、函数名
// 完美实现了你要求的 LOG_INFO("...", xxx) 语法！
// =========================================================================

// 获取 spdlog 的源码位置对象，__VA_ARGS__ 用于接收可变参数（即 {} 里的内容）
#define MAGMA_LOG_TRACE(...)    ::Magma::Common::Logger::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...)          ::Magma::Common::Logger::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)           ::Magma::Common::Logger::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...)           ::Magma::Common::Logger::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)          ::Magma::Common::Logger::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::err, __VA_ARGS__)
#define LOG_FATAL(...)          ::Magma::Common::Logger::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::critical, __VA_ARGS__)