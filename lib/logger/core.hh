#pragma once

#include <fmt/format.h>

#include <memory>
#include <string_view>
#include <string>
#include <syslog.h>

namespace logger {
  void open(std::string_view name) {
    openlog(name.data(), LOG_CONS | LOG_PERROR | LOG_PID, LOG_USER);
  }

  void log(int level, std::string_view format, auto&&... args) {
    const std::string message = fmt::format(fmt::runtime(format), std::forward<decltype(args)>(args)...);
    syslog(level, "%s", message.c_str());
  }
  
  void logInfo(std::string_view format, auto&&... args) {
    log(LOG_INFO, format, std::forward<decltype(args)>(args)...);
  }

  void logWarning(std::string_view format, auto&&... args) {
    log(LOG_WARNING, format, std::forward<decltype(args)>(args)...);
  }

  void logError(std::string_view format, auto&&... args) {
    log(LOG_ERR, format, std::forward<decltype(args)>(args)...);
  }

  void logDebug(std::string_view format, auto&&... args) {
    log(LOG_DEBUG, format, std::forward<decltype(args)>(args)...);
  };
}
