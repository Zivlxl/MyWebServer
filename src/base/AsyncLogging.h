/**
 * @file AsyncLogging.h
 * @brief
 * @author ziv
 * @email
 * @date 2023/3/7.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#pragma once

#include "noncopyable.h"
#include "Thread.h"
#include <string>

namespace kafka {

class AsyncLogging : noncopyable {
public:
  AsyncLogging(const std::string &basename, int interval = 2);

  ~AsyncLogging();

  void start() {
    running_ = true;
    thread_.start();
    latch_.wait();
  }

  void stop() {
    running_ = false;
    cond_.notify();
    thread_.join();
  }

private:
 void threadFunc();
  std::string basename_;
  int interval_;
  bool running_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_;
  CountDownLatch latch_;
};

} // namespace kafka