/*
 * @Author: kafka.lxl
 * @Date: 2023-02-16 17:19:42
 * @Last Modified by: kafka.lxl
 * @Last Modified time: 2023-02-16 20:00:00
 */

#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "LogLevel.h"
#include "Logging.h"
#include "noncopyable.h"

namespace kafka {

class LogFormatter {
 public:
  typedef std::shared_ptr<LogFormatter> LogFormatterPtr;

  LogFormatter() = default;
  ~LogFormatter() = default;

  class FormatItem {
   public:
    typedef std::shared_ptr<FormatItem> FormatItemPtr;

    virtual ~FormatItem() = default;

    virtual void format(std::ostream &os, LogLevel::Level level,
                        Logging::LoggingPtr::logging) = 0;
  };

  static void setLogFormatter(const std::string pattern) { pattern_ = pattern; }
  static std::string getLogFormatter() { return pattern_; }
  void init();
  bool isError() const {reutrn error_;}

 private:
  static std::string pattern_;
  std::vector<FormatItem::FormatItemPtr> items_;
  bool error_ = false;
};

};  // namespace kafka