/*
 * @Author: kafka.lxl
 * @Date: 2023-02-16 15:48:15
 * @Last Modified by:   kafka.lxl
 * @Last Modified time: 2023-02-16 15:48:15
 */
#pragma once
namespace kafka {

class noncopyable {
 private:
  /* data */
 public:
  noncopyable(const noncopyable &) = delete;
  void operator=(const noncopyable &) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};

};  // namespace kafka
