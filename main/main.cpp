/*
 Main.cpp

 Copyright (c) 2024 Thomas Bruckschlegel. All rights reserved.
*/

#include "main.hpp"
#include "util/perf_macros.h"

using namespace std;

#define NAIVE_DISTANCE_IMPL 1

// comment this line out, to disable the naive impl.
#undef NAIVE_DISTANCE_IMPL

// iter distance() impl
template<typename It>
size_t my_distance(It first, It last) {
  using category = typename std::iterator_traits<It>::iterator_category;
#ifndef NAIVE_DISTANCE_IMPL
  if constexpr (std::is_same<std::random_access_iterator_tag,
                    category>::value) {
    return last - first;
  } else {
#endif
    size_t result = 0;
    for (; first != last; ++first, ++result) {
    }
    return result;
#ifndef NAIVE_DISTANCE_IMPL
  }
#endif
}

// main entry point
int main() {
  std::shared_ptr<performance::PerformanceProfiler> profiler;
  profiler = std::make_shared<performance::PerformanceProfiler>(
      [](const std::string& segment_name, double value,
          const std::string& unit) {
        static util::Timer<std::chrono::nanoseconds> elapsed_time;
        static bool first_time = true;
        static util::StopWatchTimer init_timer;
        if (first_time) {
          first_time = false;
          std::cout << std::right << std::setfill(' ') << std::setw(16)
                    << "Time offset (micro)" << std::right << std::setfill(' ')
                    << std::setw(64) << "Segment" << std::right
                    << std::setfill(' ') << std::setw(16) << "Value"
                    << std::right << std::setfill(' ') << std::setw(16)
                    << "Unit" << std::endl;
        }

        if (segment_name.empty())
          return;

        std::cout << std::right << std::setfill(' ') << std::setw(16)
                  << (elapsed_time.ElapsedTime() / 1000) << std::right
                  << std::setfill(' ') << std::setw(64) << segment_name
                  << std::right << std::setfill(' ') << std::setw(16) << value
                  << std::right << std::setfill(' ') << std::setw(16) << unit;

        std::cout << std::endl;
        elapsed_time.Reset();
      });

  LOG_MEM(profiler, GetCurrentProcessId(), "example1.exe");

  {
    LOG_PERF(profiler, "my_distance1");
    size_t x = 0;
    vector<string> names{ "Jerry", "John", "Frank", "Jerry", "John", "Frank",
      "Michael" };
    auto iter_Jerry = find(names.begin(), names.end(), "Jerry");
    auto iter_Michael = find(names.begin(), names.end(), "Michael");
    for (int i = 0; i < 100000000; ++i) {
      x += my_distance(iter_Jerry, iter_Michael) + i;
    }
    cout << x << "\n";
  }

  {
    LOG_PERF(profiler, "my_distance2");
    size_t x = 0;
    unordered_map<string, int> names{ { "Jerry", 1 }, { "John", 2 },
      { "Frank", 3 }, { "Frank2", 4 }, { "Frank3", 5 }, { "Frank4", 6 },
      { "Frank5", 7 }, { "Michael", 4 } };
    auto iter_Jerry = names.find("Jerry");
    auto iter_Michael = names.find("Michael");

    for (int i = 0; i < 100000000; ++i) {
      x += my_distance(iter_Jerry, iter_Michael) + i;
    }
    cout << x << "\n";
  }

  {
    LOG_PERF(profiler, "stl distance1");
    size_t x = 0;
    vector<string> names{ "Jerry", "John", "Frank", "Jerry", "John", "Frank",
      "Michael" };
    auto iter_Jerry = find(names.begin(), names.end(), "Jerry");
    auto iter_Michael = find(names.begin(), names.end(), "Michael");
    for (int i = 0; i < 100000000; ++i) {
      x += std::distance(iter_Jerry, iter_Michael) + i;
    }
    cout << x << "\n";
  }

  {
    LOG_PERF(profiler, "stl distance2");
    size_t x = 0;
    unordered_map<string, int> names{ { "Jerry", 1 }, { "John", 2 },
      { "Frank", 3 }, { "Frank2", 4 }, { "Frank3", 5 }, { "Frank4", 6 },
      { "Frank5", 7 }, { "Michael", 4 } };
    auto iter_Jerry = names.find("Jerry");
    auto iter_Michael = names.find("Michael");
    for (int i = 0; i < 100000000; ++i) {
      x += std::distance(iter_Jerry, iter_Michael) + i;
    }
    cout << x << "\n";
  }
}
