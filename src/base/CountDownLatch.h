/**
 * @file CountDownLatch.h
 * @brief
 * @author ziv
 * @email
 * @date 2023/3/7.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#pragma once
#include "MutexLock.h"
#include "Condition.h"
#include "noncopyable.h"

namespace kafka {

class CountDownLatch : noncopyable {
public:
  explicit CountDownLatch(int count);
  void wait();
  void countDown();

private:
  mutable MutexLock mutex_;
  Condition condition_;
  int count_;
};

}
