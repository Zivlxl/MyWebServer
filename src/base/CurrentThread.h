/*
 * @Author: kafka.lxl 
 * @Date: 2023-02-16 19:39:10 
 * @Last Modified by: kafka.lxl
 * @Last Modified time: 2023-02-16 20:03:35
 */

#pragma once

#include<stdint.h>
#include "Util.h"

namespace kafka {

namespace CurrentThread {

extern __thread int t_cacheTid;
extern __thread char t_tidString[32];
extern __thread int t_tidStringLength;
extern __thread const char *t_threadName;

void cacheTid();

inline int tid() {
    if (unlikely(t_cacheTid == 0)) {
        cacheTid();
    }

    return t_cacheTid;
}

inline const char *tidString() {
    return t_tidString;
}

inline int tidStringLength() {
    return t_tidStringLength;
}

inline const char* threadName() {
    return t_threadName;
}

};

};