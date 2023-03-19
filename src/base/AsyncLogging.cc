/**
 * @file AsyncLogging.cc
 * @brief
 * @author ziv
 * @email
 * @date 2023/3/7.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#include "AsyncLogging.h"
#include "Util.h"
#include <algorithm>

namespace kafka {

AsyncLogging::AsyncLogging(const std::string &basename, int interval)
    : basename_(basename), interval_(interval), running_(false),
      thread_(std::bind(&AsyncLogging::threadFunc, this), "AsyncLogging"),
      mutex_(), cond_(mutex_), latch_(1) {
  cur_buf_.clear();
  bak_buf_.clear();
}

void AsyncLogging::threadFunc() {
  assert(running_ == true);
  latch_.countDown();

  while (running_) {
    {
      MutexLockGuard lock(mutex_);
      if (cur_buf_.empty() && bak_buf_.empty()) {
        cond_.waitForSeconds(interval_);
      }
//      std::copy(cur_buf_.begin(), cur_buf_.end(), bak_buf_.end());
      bak_buf_.insert(bak_buf_.end(), cur_buf_.begin(), cur_buf_.end());
      cur_buf_.clear();
    }

    if (!bak_buf_.empty()) {
      if (!ofs_.is_open()) {
        if (!reopen()) {
          std::cout << "open file error!" << std::endl;
        }

      }

      std::for_each(bak_buf_.begin(), bak_buf_.end(),
                    [&](const std::string &str) { ofs_ << str; });
      bak_buf_.clear();
    }
  }
}

void AsyncLogging::append(std::string &str) {
  MutexLockGuard lock(mutex_);

  cur_buf_.emplace_back(str);

  cond_.notify();
}

bool AsyncLogging::reopen() {
  MutexLockGuard lock(mutex_);

  if (ofs_.is_open()) {
    ofs_.close();
  }

  return FSUtil::OpenForWrite(ofs_, basename_, std::ios::app);
}

} // namespace kafka