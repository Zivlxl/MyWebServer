/*
 * @Author: kafka.lxl
 * @Date: 2023-02-16 15:42:01
 * @Last Modified by: kafka.lxl
 * @Last Modified time: 2023-02-16 20:00:02
 */

#pragma once
#include <time.h>

#include <memory>
#include <sstream>

#include "LogConfiguration.h"
#include "LogFormatter.h"
#include "noncopyable.h"

namespace kafka {
class LogFormatter;

class Logging : noncopyable, public std::enable_shared_from_this<Logging> {
public:
  typedef std::shared_ptr<Logging> LoggingPtr;
  // friend class LogFormatter;
  Logging() = delete;
  Logging(LogConfiguration::Level level, const char *file, int line,
          int thread_id, const char *thread_name, int coroutine_id);
  ~Logging();
  std::stringstream &getSS() { return ss_; }
  std::string getContent() const { return ss_.str(); }
  LogConfiguration::Level getLevel() const { return level_; }
  int getThreadId() const { return thread_id_; }
  int getCoroutineId() const { return coroutine_id_; }
  const char *getFile() const { return file_; }
  const char *getThreadName() const { return thread_name_; }
  int getLine() const { return line_; }
  time_t getTime() const { return time_; }
  void log();

private:
  LogConfiguration::Level level_;
  std::stringstream ss_;
  const char *file_;
  int line_;
  int thread_id_;
  const char *thread_name_;
  int coroutine_id_;
  time_t time_;
  kafka::LogFormatter::LogFormatterPtr log_formatter_;
};

class LoggingWrap {
public:
  LoggingWrap(Logging::LoggingPtr logging) : logging_(logging) {}

  ~LoggingWrap() { logging_->log(); }

  Logging::LoggingPtr getLogging() const { return logging_; }

  std::stringstream &getSS() { return logging_->getSS(); }

private:
  Logging::LoggingPtr logging_;
};

#define LOG                                                                    \
  kafka::LoggingWrap(kafka::Logging::LoggingPtr(new kafka::Logging(            \
                         LogConfiguration::DEBUG, __FILE__, __LINE__,          \
                         kafka::CurrentThread::tid(),                          \
                         kafka::CurrentThread::threadName(), 0)))              \
      .getSS()

}; // namespace kafka