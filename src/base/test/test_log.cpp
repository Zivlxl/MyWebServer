/**
 * @file test_log.cpp
 * @brief
 * @author ziv
 * @email
 * @date 23-3-2.
 * @copyright Copyright (c) 2023年 ziv All rights reserved.
 */

#include <iostream>

#include "../CurrentThread.h"
#include "../LogInclude.h"

using namespace kafka;

int main(int argc, char **argv) {
//  std::shared_ptr<Logging> g(new Logging(LogConfiguration::DEBUG, __FILE__,
//                                         __LINE__,
//                                         kafka::CurrentThread::tid(),
//                                         kafka::CurrentThread::threadName(),
//                                         0));

//  g->getSS() << "dddddd" << 111 << true;
      LOG << "dddddd" << 111 << true << std::endl;
//  std::cout << CurrentThread::tidString() << std::endl;
//  std::cout << CurrentThread::tidStringLength() << std::endl;
//  std::cout << CurrentThread::tid() << std::endl;
}