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
__thread const char* t_threadName = "default";
}  // namespace CurrentThread

pid_t gettid() { return static_cast<pid_t>(::syscall(SYS_gettid)); }

void CurrentThread::cacheTid() {
  if (t_cachedTid == 0) {
    t_cachedTid = gettid();
    t_tidStringLength =
        snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
  }
}

}  // namespace kafka