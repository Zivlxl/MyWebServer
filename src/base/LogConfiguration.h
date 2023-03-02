/*
 * @Author: kafka.lxl
 * @Date: 2023-02-16 16:53:15
 * @Last Modified by: kafka.lxl
 * @Last Modified time: 2023-02-16 19:59:57
 */

#pragma once

#include <string>

namespace kafka {

class LogConfiguration {
 public:
  enum Level {
    UNKNOWN = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
  };

  static void setLogFileName(std::string &filename) {kLogFileName = filename;}
  static void setLogLevel(Level level) {kLevel = level;}
  static Level getLogLevel() {return kLevel;}
  static std::string getLogFileName() {return kLogFileName;}

  static Level kLevel;
  static std::string kLogFileName;
};

};  // namespace kafka