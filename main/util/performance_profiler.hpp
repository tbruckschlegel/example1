/*
 Performance profiler.

 Copyright (c) 2024 Thomas Bruckschlegel. All rights reserved.
*/

#pragma once

#include "util/timer.hpp"
#include <string>
#include <functional>
#include <unordered_map>
#include <mutex>

namespace performance {
using profiler_output_handler_t = std::function<
    void(const std::string&, double, const std::string& unit)>;
using timer_precision_t = util::Timer<std::chrono::milliseconds>;

namespace detail {
struct ProcessMemoryData {
  std::string process_name;
  size_t private_size{};
  size_t peak_working_size{};
};
}  // namespace detail

/// Units used by the profiler.
enum class ProfilerUnit {
  kMS,      /// milliseconds
  kMB,      /// megabytes
  kComment  /// comment
};

/**
 * Helper class to convert a ProfilerUnit value to std::string.
 */
struct ProfilerUnitString final {
  ProfilerUnitString() = delete;
  explicit ProfilerUnitString(ProfilerUnit v) : value(v){};

  operator std::string() const {
    static std::unordered_map<ProfilerUnit, std::string> strings{
      { ProfilerUnit::kMS, "ms" }, { ProfilerUnit::kMB, "MB" },
      { ProfilerUnit::kComment, "Comment" }
    };
    if (const auto&& it = std::find_if(strings.begin(), strings.end(),
            [&](auto&& i) { return value == i.first; });
        it != strings.end())
      return it->second;

    return "Undefined";
  }

  ProfilerUnit value{};
};

class PerformanceProfiler final {
public:
  PerformanceProfiler() = delete;
  /**
   * @brief Create a performance profiler instance that outputs results to the
   * given profiler_output_handler_t callback.
   * @param output_handler a profiler_output_handler_t callback function.
   */
  explicit PerformanceProfiler(profiler_output_handler_t output_handler);
  ~PerformanceProfiler() = default;

  /**
   * @brief Shutdown the performance profiler and outputs final memory usage
   */
  void Shutdown();

  /**
   * @brief Start tracking of a segment.
   * @param segment_name the segment name.
   */
  void Start(const std::string& segment_name);

  /**
   * @brief Stop tracking of a segment and outputs the time taken in the section
   * and also memory usage for all tracked processes.
   * @param segment_name the segment name.
   */
  void End(const std::string& segment_name);

  /**
   * @brief Start tracking memory usage of a given PID and process name.
   * @param pid the process ID.
   * @param process_name the process name (used to decorate the output)
   */
  void AddProcess(uint32_t pid, const std::string& process_name);

  /**
   * @brief Remove previously tracked processes that match the given process
   * name.
   * @param process_name the process name that will be removed from the track
   * list.
   */
  void RemoveProcesses(const std::string& process_name);

  /**
   * @brief Collect current memory usage of all tracked processes, if any.
   */
  void CollectMemoryUsage();

  /**
   * @brief Send a comment to the output handler.
   * @param comment the comment string.
   */
  void SendComment(const std::string& comment) const;

private:
  void OutputMemoryUsage(uint32_t pid, const detail::ProcessMemoryData& pmd);

  std::unordered_map<std::string, timer_precision_t> running_;
  std::unordered_map<uint32_t, detail::ProcessMemoryData> processes_;
  profiler_output_handler_t output_handler_;
  std::mutex mutex_;
};

class PerformanceObject {
public:
  PerformanceObject() = delete;
  /**
   * @brief Create a performance object that can be used to track code sections.
   * @param profiler the performance profiler associated with the performance
   * object instance.
   * @param segment_name the segment name.
   */
  explicit PerformanceObject(std::shared_ptr<PerformanceProfiler> profiler,
      const std::string& segment_name);
  ~PerformanceObject();

  /**
   * @brief Get the performance profiler instance associated with this
   * performance object.
   * @return the performance profiler.
   */
  std::shared_ptr<PerformanceProfiler> GetProfiler() {
    return profiler_;
  }

private:
  std::shared_ptr<PerformanceProfiler> profiler_;
  std::string segment_name_;
};
}  // namespace performance
