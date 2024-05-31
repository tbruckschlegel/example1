/*
 Performance profiler helper macros.

 Copyright (c) 2024 Thomas Bruckschlegel. All rights reserved.
*/

#pragma once
#include "util/performance_profiler.hpp"

#define LOG_PERF(p, x) performance::PerformanceObject pobj_##__LINE__(p, x)

#define LOG_MEM(p, pid, x)   \
  {                          \
    if (p != nullptr) {      \
      p->AddProcess(pid, x); \
    }                        \
  }
