/*
 * @Author: kafka.lxl
 * @Date: 2023-02-16 19:45:15
 * @Last Modified by: kafka.lxl
 * @Last Modified time: 2023-02-16 19:46:35
 */

#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace kafka {
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

class FSUtil {
public:
  static bool Mkdir(const std::string &dirname);
  static std::string Dirname(const std::string &filename);
  static std::string Basename(const std::string &filename);
  static bool OpenForRead(std::ifstream &ifs, const std::string &filename,
                          std::ios_base::openmode mode);

  static bool OpenForWrite(std::ofstream &ofs, const std::string &filename,
                           std::ios_base::openmode mode);
};

} // namespace kafka
