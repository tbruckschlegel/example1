#include "gtest/gtest.h"
#include "util/performance_profiler.hpp"
#include "util/perf_macros.h"

#define GTEST_COUT std::cerr << "[          ] [ INFO ] "

std::shared_ptr<performance::PerformanceProfiler> test;

TEST(PerformanceProfiler, Start) {
  test = std::make_unique<performance::PerformanceProfiler>(
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

  { LOG_PERF(test, "unit test"); }

  EXPECT_TRUE(test.get() != nullptr);
}

TEST(PerformanceProfiler, End) {
  test.reset();
}
