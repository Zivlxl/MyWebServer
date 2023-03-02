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

#include "LogConfiguration.h"
#include "noncopyable.h"

namespace kafka {
class Logging;

class LogFormatter {
 public:
  typedef std::shared_ptr<LogFormatter> LogFormatterPtr;

  friend class Logging;

  LogFormatter() = default;
  ~LogFormatter() = default;

  class FormatItem {
   public:
    typedef std::shared_ptr<FormatItem> FormatItemPtr;

    virtual ~FormatItem() = default;

    virtual void format(std::ostream &os, std::shared_ptr<Logging> logging) = 0;
  };

  static void setLogFormatter(const std::string &pattern) {
    kPattern = pattern;
  }
  static std::string getLogFormatter() { return kPattern; }
  void init();
  bool isError() const { return error_; }
  std::ostream &format(std::ostream &os,
                       std::shared_ptr<kafka::Logging> logging);

 private:
  static std::string kPattern;
  std::vector<FormatItem::FormatItemPtr> items_;
  bool error_ = false;
};

};  // namespace kafka