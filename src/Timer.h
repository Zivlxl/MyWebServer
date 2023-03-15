/**
 * @file Timer.h
 * @brief
 * @author ziv
 * @email
 * @date 2023/3/15.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#pragma once

#include "base/MutexLock.h"
#include "base/noncopyable.h"
#include <functional>
#include <memory>
#include <queue>
#include <unistd.h>
#include <vector>

namespace kafka {

typedef std::function<void(void)> TimerCallback;

class EventLoop;

class Timer : public noncopyable {
public:
  typedef std::shared_ptr<Timer> TimerPtr;
  Timer(TimerCallback cb, unsigned long long expire, double interval)
      : cb_(cb), expire_(expire), interval_(interval), repeat_(interval > 0.0) {
  }
  ~Timer();

  unsigned long long expire() const { return expire_; }

  void run() const { cb_(); }

  void restart(unsigned long long expire);

private:
  TimerCallback cb_;
  unsigned long long expire_;
  double interval_;
  bool repeat_;
};

struct TimerCmp {
  bool operator()(Timer::TimerPtr &first, Timer::TimerPtr &second) const {
    return first->expire() > second->expire();
  }
};

class TimerManager : public noncopyable {
public:
  TimerManager();

  ~TimerManager();

  void addTimer(TimerCallback cb, unsigned long long expire, double interval);

  unsigned long long getNextTimer();

private:
  std::priority_queue<Timer::TimerPtr, std::deque<Timer::TimerPtr>, TimerCmp>
      timer_queue_;
};

} // namespace kafka
