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
#include <vector>
#include <fstream>

namespace kafka {

class AsyncLogging : noncopyable {
public:
  AsyncLogging(const std::string &basename, int interval = 2);

  ~AsyncLogging() = default;

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

  bool reopen();

  void append(std::string &str);

private:
 void threadFunc();
  std::string basename_;
  int interval_;
  bool running_;
  Thread thread_;
  MutexLock mutex_;
  Condition cond_;
  CountDownLatch latch_;
  std::vector<std::string> cur_buf_;
  std::vector<std::string> bak_buf_;
  std::ofstream ofs_;
};

} // namespace kafka