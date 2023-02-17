/*
 * @Author: kafka.lxl
 * @Date: 2023-02-16 15:42:01
 * @Last Modified by: kafka.lxl
 * @Last Modified time: 2023-02-16 20:00:02
 */

#pragma once
#include "noncopyable.h"
#include<memory>
#include<sstream>

namespace kafka {

class Logging : noncopyable {
 public:
 typedef std::shared_ptr<Logging> LoggingPtr;
  Logging();
  ~Logging();
  
  private:
  std::stringstream ss_;
  
};

};  // namespace kafka