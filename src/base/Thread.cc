/*
 * @Author: kafka.lxl
 * @Date: 2023-02-16 19:52:17
 * @Last Modified by: kafka.lxl
 * @Last Modified time: 2023-02-16 20:04:17
 */

#include "Thread.h"

#include <assert.h>
#include <errno.h>
#include <linux/unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <syscall.h>
#include <unistd.h>

#include <memory>

#include "CurrentThread.h"

namespace kafka {

namespace CurrentThread {
__thread int t_cachedTid = 0;
__thread char t_tidString[32];
__thread int t_tidStringLength = 6;
__thread const char *t_threadName = "default";
} // namespace CurrentThread

pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }

void CurrentThread::cacheTid() {
  if (t_cachedTid == 0) {
    t_cachedTid = gettid();
    t_tidStringLength =
        snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}

// In order to keep same structure of Thread
struct ThreadData {
  typedef Thread::ThreadFun ThreadFun;
  ThreadFun func_;
  std::string thread_name_;
  pid_t *tid_;
  CountDownLatch* latch_;

  ThreadData(const ThreadFun &func, const std::string &thread_name, pid_t *tid,
             CountDownLatch* latch)
      : func_(func), thread_name_(thread_name), tid_(tid), latch_(latch) {}

  void runInThread() {
    *tid_ = CurrentThread::tid();
    tid_ = NULL;
    latch_->countDown();
    latch_ = nullptr;

    CurrentThread::t_threadName =
        thread_name_.empty() ? "Thread" : thread_name_.c_str();

    // set thread name, can get it from "proc/xx(pid)/comm"
    prctl(PR_SET_NAME, CurrentThread::t_threadName);

    func_();

    CurrentThread::t_threadName = "finished";
  }
};

void *startThread(void *obj) {
  ThreadData *data = static_cast<ThreadData*>(obj);

  data->runInThread();

  delete data;
  data = nullptr;
  return nullptr;
}

Thread::Thread(const ThreadFun &thread_fun, const std::string &thread_name)
    : thread_fun_(thread_fun), started_(false), joined_(false), tid_(0),
      thread_name_(thread_name), pthread_id_(0), latch_(1) {
  setDefaultName();
}

Thread::~Thread() {
  if (started_ && !joined_) {
    pthread_detach(pthread_id_);
  }
}

void Thread::setDefaultName() {
  if (thread_name_.empty()) {
    thread_name_ = "Thread";
  }
}

void Thread::start() {
  assert(!started_);
  started_ = false;

  ThreadData *data = new ThreadData(thread_fun_, thread_name_, &tid_, &latch_);
  if (pthread_create(&pthread_id_, NULL, &startThread, data)) {
    started_ = false;
    delete data;
//    assert(false);
  }
  else {
    latch_.wait();
    assert(tid_ > 0);
  }
}

int Thread::join() {
  assert(started_ && (!joined_));
  joined_ = true;

  return pthread_join(pthread_id_, nullptr);
}

} // namespace kafka