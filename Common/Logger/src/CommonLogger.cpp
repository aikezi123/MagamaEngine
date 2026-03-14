#include "CommonLogger.h"
#include <spdlog/sinks/stdout_color_sinks.h> // 控制台彩色输出
#include <spdlog/sinks/basic_file_sink.h>    // 输出到文件

namespace Magma::Common {

    std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;

    void Logger::Init() {
        // 1. 设置极其严谨的日志格式 (Pattern)
        // 格式说明：
        // [%T] 时间 (HH:MM:SS)
        // [%n] 工程名/模块名 (MagmaEngine)
        // [%^%l%$] 日志级别 (带颜色)
        // [%s:%#] 文件名:行号
        // [%!] 函数名
        // %v 真正的日志内容
        spdlog::set_pattern("[%T] [%n] [%^%l%$] [%s:%#] [%!] %v");

        // 2. 创建两个输出端：一个到控制台看，一个保存到文件留底
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>(); // mt代表多线程安全
        auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("MagmaEngine_Runtime.log", true);

        // 3. 组装引擎级 Logger
        s_CoreLogger = std::make_shared<spdlog::logger>("MagmaEngine", spdlog::sinks_init_list{ console_sink, file_sink });

        // 4. 设置全局最低触发级别 (开发阶段设为 Trace，发布时设为 Info)
        s_CoreLogger->set_level(spdlog::level::trace);
        s_CoreLogger->flush_on(spdlog::level::trace); // 一旦有日志立刻写入磁盘，防止崩溃时丢失最后一条记录

        spdlog::register_logger(s_CoreLogger);
    }

    std::shared_ptr<spdlog::logger>& Logger::GetCoreLogger() {
        return s_CoreLogger;
    }

}