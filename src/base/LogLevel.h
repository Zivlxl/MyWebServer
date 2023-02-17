/*
 * @Author: kafka.lxl
 * @Date: 2023-02-16 16:53:15
 * @Last Modified by: kafka.lxl
 * @Last Modified time: 2023-02-16 19:59:57
 */

#pragma once

namespace kafka {

class LogLevel {
 public:
  enum Level {
    UNKNOWN = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
  };
};

};  // namespace kafka