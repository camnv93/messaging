#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace msghub::common {

  class Time {
   public:
    static uint64_t GetCurrentTimeMs() noexcept {
      auto now = std::chrono::steady_clock::now();
      return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
    }

    static std::string GetCurrentFormattedTime() {
      using namespace std::chrono;
      auto now = system_clock::now();
      auto time_now = system_clock::to_time_t(now);
      std::tm tm_now = *std::localtime(&time_now);
      auto duration = now.time_since_epoch();
      auto seconds = duration_cast<std::chrono::seconds>(duration);
      auto ms = duration_cast<milliseconds>(duration) - duration_cast<milliseconds>(seconds);
      std::ostringstream oss;
      oss << std::put_time(&tm_now, "%H:%M:%S") << "." << std::setfill('0') << std::setw(3) << ms.count();
      return oss.str();
    }

   public:
    Time(const Time&) = delete;
    Time& operator=(const Time&) = delete;
    Time(Time&&) = delete;
    Time& operator=(Time&&) = delete;
    Time() = delete;
  };

}  // namespace msghub::common
