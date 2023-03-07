/**
 * @file Thread.h
 * @brief
 * @author ziv
 * @email
 * @date 23-3-2.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#pragma once
#include "noncopyable.h"
#include <functional>
#include <string>
#include "CountDownLatch.h"

namespace kafka {

class Thread : noncopyable {
public:
  typedef std::function<void(void)> ThreadFun;
  explicit Thread(const ThreadFun &,
                  const std::string &thread_name = std::string());

  ~Thread();

  void start();

  int join();

  bool started() const {return started_;}

  pid_t tid() const {return tid_;}

  const std::string& thread_name() const {return thread_name_;}

private:
  void setDefaultName();
  ThreadFun thread_fun_;
  bool started_;
  bool joined_;
  pid_t tid_;
  std::string thread_name_;
  pthread_t pthread_id_;
  CountDownLatch latch_;
};

} // namespace kafka
