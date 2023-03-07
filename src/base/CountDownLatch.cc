/**
 * @file CountDownLatch.cc
 * @brief
 * @author ziv
 * @email
 * @date 2023/3/7.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#include "CountDownLatch.h"

namespace kafka {
CountDownLatch::CountDownLatch(int count)
    : mutex_(), condition_(mutex_), count_(count) {}

void CountDownLatch::countDown() {
  MutexLockGuard guard(mutex_);
  count_--;
  if (!count_) {
    condition_.notifyAll();
  }
}

void CountDownLatch::wait() {
  MutexLockGuard guard(mutex_);
  while (count_ > 0) condition_.wait();
}
} // namespace kafka