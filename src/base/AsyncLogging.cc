/**
 * @file AsyncLogging.cc
 * @brief
 * @author ziv
 * @email
 * @date 2023/3/7.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#include "AsyncLogging.h"

namespace kafka {

AsyncLogging::AsyncLogging(const std::string &basename, int interval)
    : basename_(basename),
      interval_(interval),
      running_(false),
      thread_(std::bind(&AsyncLogging::threadFunc, this), "AsyncLogging"),
      mutex_(),
      cond_(mutex_),
      latch_(1) {

}

}  // namespace kafka