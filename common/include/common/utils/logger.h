#pragma once

#include <memory>
#include <string>
#include <vector>

#include "spdlog/async.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/spdlog.h"

#ifdef _WIN32
#include <process.h>
#define GET_PID() _getpid()
#else
#include <unistd.h>
#define GET_PID() getpid()
#endif

//---------------------------------------------------
// Default TAG
//---------------------------------------------------

#ifndef LOG_TAG
#define LOG_TAG "LOG"
#endif

//---------------------------------------------------
// Core format
//---------------------------------------------------

#ifndef __FILENAME__
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif
#define LOG_FMT(fmt, ...) "[{}][{}][{}::{}]" fmt, LOG_TAG, GET_PID(), __FILENAME__, __LINE__, ##__VA_ARGS__

namespace msghub::common {

  class Logger {
   public:
    static void Init(const std::string& log_file,
                     spdlog::level::level_enum console_level = spdlog::level::info,
                     spdlog::level::level_enum file_level = spdlog::level::debug) {
      constexpr size_t kQueueSize = 4;
      constexpr size_t kThreadCount = 1;

      spdlog::init_thread_pool(kQueueSize, kThreadCount);
      // Console sink
      auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      console_sink->set_level(console_level);
      console_sink->set_pattern("[%F][%Y-%m-%d %H:%M:%S,%e]%v");

      // Rotating file sink
      constexpr size_t kMaxFileSize = 5 * 1024 * 1024;  // 5MB
      constexpr size_t kMaxFiles = 10;
      auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(log_file, kMaxFileSize, kMaxFiles);
      file_sink->set_level(file_level);
      file_sink->set_pattern("[%F][%Y-%m-%d %H:%M:%S,%e] %v");

      std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};

      logger_ = std::make_shared<spdlog::async_logger>(
          LOG_TAG, sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);

      logger_->set_level(spdlog::level::trace);
      logger_->flush_on(spdlog::level::err);
      spdlog::register_logger(logger_);
    }

    static std::shared_ptr<spdlog::logger>& Get() {
      InitIfNeeded();
      return logger_;
    }

   private:
    inline static std::shared_ptr<spdlog::logger> logger_;

    static void InitIfNeeded() {
      static std::once_flag once;
      std::call_once(once, []() { Init("logs/default.log"); });
    }
  };

}  // namespace msghub::common

//---------------------------------------------------
// Public macros
//---------------------------------------------------

#define LOG_TRACE(fmt, ...) SPDLOG_LOGGER_TRACE(msghub::common::Logger::Get(), LOG_FMT(fmt, ##__VA_ARGS__))

#define LOG_DEBUG(fmt, ...) SPDLOG_LOGGER_DEBUG(msghub::common::Logger::Get(), LOG_FMT(fmt, ##__VA_ARGS__))

#define LOG_INFO(fmt, ...) SPDLOG_LOGGER_INFO(msghub::common::Logger::Get(), LOG_FMT(fmt, ##__VA_ARGS__))

#define LOG_WARN(fmt, ...) SPDLOG_LOGGER_WARN(msghub::common::Logger::Get(), LOG_FMT(fmt, ##__VA_ARGS__))

#define LOG_ERROR(fmt, ...) SPDLOG_LOGGER_ERROR(msghub::common::Logger::Get(), LOG_FMT(fmt, ##__VA_ARGS__))

#define LOG_CRITICAL(fmt, ...) SPDLOG_LOGGER_CRITICAL(msghub::common::Logger::Get(), LOG_FMT(fmt, ##__VA_ARGS__))
