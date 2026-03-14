#include "CommonLogger.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <vector>

namespace Magma::Common::Logger {

    std::shared_ptr<spdlog::logger> LogSystem::s_CoreLogger;

    void LogSystem::Initialize() {
        // 防止重复初始化
        if (s_CoreLogger) return;

        // 设置格式：[时间] [模块] [级别] [文件:行号] [函数名] 内容
        spdlog::set_pattern("[%T] [%n] [%^%l%$] [%s:%#] [%!] %v");

        // 双通道输出：控制台带颜色，文件落盘留档
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("MagmaEngine_Runtime.log", true));

        s_CoreLogger = std::make_shared<spdlog::logger>("MagmaEngine", begin(logSinks), end(logSinks));
        spdlog::register_logger(s_CoreLogger);

        // 设置最低拦截级别
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace); // 实时刷入硬盘，极其重要
    }

    std::shared_ptr<spdlog::logger>& LogSystem::GetCoreLogger() {
        return s_CoreLogger;
    }

}