/**
 * @file Logging.cpp
 * @brief
 * @author ziv
 * @email
 * @date 23-3-2.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#include "Logging.h"
#include "AsyncLogging.h"
#include <iostream>

#include "LogConfiguration.h"
namespace kafka {
LogConfiguration::Level LogConfiguration::kLevel = LogConfiguration::DEBUG;
std::string LogConfiguration::kLogFileName = "./log.txt";

static pthread_once_t once_control_ = PTHREAD_ONCE_INIT;
static AsyncLogging *asyncLogging_;
static std::unique_ptr<AsyncLogging> pAsyncLogging_;

void once_init() {
  pAsyncLogging_.reset(new AsyncLogging(LogConfiguration::kLogFileName));
  pAsyncLogging_->start();
}

void output(std::string &str) {
  pthread_once(&once_control_, once_init);
  pAsyncLogging_->append(str);
}

Logging::Logging(LogConfiguration::Level level, const char *file, int line,
                 int thread_id, const char *thread_name, int coroutine_id)
    : level_(level), file_(file), line_(line), thread_id_(thread_id),
      thread_name_(thread_name), coroutine_id_(coroutine_id) {
  time_ = time(nullptr);
  log_formatter_.reset(new LogFormatter);
  log_formatter_->init();
}

Logging::~Logging() {}

void Logging::log() {
  std::stringstream ss;
  if (level_ >= LogConfiguration::getLogLevel()) {
    if (!log_formatter_->isError()) {
      auto self = shared_from_this();
      log_formatter_->format(ss, self);
    } else {
      exit(1);
    }
  }
  //  std::cout << ss.str();
  std::string str = ss.str();
  std::cout << str;
  output(str);
  ss.clear();
}
} // namespace kafka