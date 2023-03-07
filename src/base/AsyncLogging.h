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
#include <string>

namespace kafka {

class AsyncLogging : noncopyable {
public:
  AsyncLogging(const std::string &basename, int interval = 2)
      : basename_(basename), interval_(interval) {}

  ~AsyncLogging();

private:
  std::string basename_;
  int interval_;
};

} // namespace kafka