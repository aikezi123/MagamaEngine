#pragma once

#include <memory>
#include <spdlog/spdlog.h>

// 严格遵守三段式命名空间：Magma(Root) :: Common(Layer) :: Logger(Feature)
namespace Magma::Common::Logger {

    class LogSystem {
    public:
        // 初始化整个引擎的日志大脑
        static void Initialize();
        // 获取核心日志器
        static std::shared_ptr<spdlog::logger>& GetCoreLogger();

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };

}

// =========================================================================
// 引擎级日志宏：完美实现你的 LOG_INFO("...", xxx) 诉求，并自动捕获黑匣子信息
// =========================================================================
#define LOG_TRACE(...)    ::Magma::Common::Logger::LogSystem::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::trace, __VA_ARGS__)
#define LOG_DEBUG(...)    ::Magma::Common::Logger::LogSystem::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::debug, __VA_ARGS__)
#define LOG_INFO(...)     ::Magma::Common::Logger::LogSystem::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::info, __VA_ARGS__)
#define LOG_WARN(...)     ::Magma::Common::Logger::LogSystem::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::warn, __VA_ARGS__)
#define LOG_ERROR(...)    ::Magma::Common::Logger::LogSystem::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::err, __VA_ARGS__)
#define LOG_FATAL(...)    ::Magma::Common::Logger::LogSystem::GetCoreLogger()->log(spdlog::source_loc{__FILE__, __LINE__, __func__}, spdlog::level::critical, __VA_ARGS__)