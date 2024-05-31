/*
 Main.cpp

 Copyright (c) 2024 Thomas Bruckschlegel. All rights reserved.
*/

#include "main.hpp"
#include <windows.h>
#include "util/perf_macros.h"

#include <memory>

using interval_map_string_t = interval_map<int, std::string>;
interval_map_string_t example_interval("X");

// get test range output
std::string get_test_range(interval_map_string_t& itm,
    int min_range,
    int max_range) {
  std::string res;
  for (int i = min_range; i < max_range; ++i) {
    const auto mk = i;
    const auto mlv = itm[i];
    res += "[" + std::to_string(mk) + "] -> " + mlv + "\n";
  }
  return res;
}

// main entry point
int main() {
  std::shared_ptr<performance::PerformanceProfiler> profiler;
  profiler = std::make_shared<performance::PerformanceProfiler>(
      [](const std::string& segment_name, double value,
          const std::string& unit) {
        static util::Timer<std::chrono::milliseconds> elapsed_time;
        static bool first_time = true;
        static util::StopWatchTimer init_timer;
        if (first_time) {
          first_time = false;
          std::cout << std::right << std::setfill(' ') << std::setw(16)
                    << "Time offset (ms)" << std::right << std::setfill(' ')
                    << std::setw(64) << "Segment" << std::right
                    << std::setfill(' ') << std::setw(16) << "Value"
                    << std::right << std::setfill(' ') << std::setw(16)
                    << "Unit" << std::endl;
        }

        if (segment_name.empty())
          return;

        std::cout << std::right << std::setfill(' ') << std::setw(16)
                  << (elapsed_time.ElapsedTime() + init_timer.ElapsedTime())
                  << std::right << std::setfill(' ') << std::setw(64)
                  << segment_name << std::right << std::setfill(' ')
                  << std::setw(16) << value << std::right << std::setfill(' ')
                  << std::setw(16) << unit;

        std::cout << std::endl;
      });

  LOG_MEM(profiler, GetCurrentProcessId(), "example2.exe");

  try {
    {
      LOG_PERF(profiler, "interval map - assign_list");
      const auto kStepSize = 10;
      const auto kTotalRange = 1024 * 1024 * 32;
      for (int i = -2; i < kTotalRange; i += kStepSize) {
        example_interval.assign(i, i + kStepSize, "a" + std::to_string(i));
      }
    }

    // std::cout << get_test_range(example_interval, -10, kTotalRange);
  } catch (std::exception e) {
    std::cout << "error happend: " << e.what() << std::endl;
  }

  return getchar();
}
