/*
 * @Author: kafka.lxl 
 * @Date: 2023-02-16 19:45:15 
 * @Last Modified by: kafka.lxl
 * @Last Modified time: 2023-02-16 19:46:35
 */

#pragma once 

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
