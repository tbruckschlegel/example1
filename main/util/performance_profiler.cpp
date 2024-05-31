/*
 Performance profiler.

 Copyright (c) 2024 Thomas Bruckschlegel. All rights reserved.
*/

#include "performance_profiler.hpp"
#include <algorithm>
#include <Windows.h>

namespace performance {
PerformanceProfiler::PerformanceProfiler(
    profiler_output_handler_t output_handler)
    : output_handler_(output_handler) {
}

void PerformanceProfiler::Shutdown() {
  std::lock_guard lock(mutex_);
  for (auto&& [segment_name, timer] : running_)
    output_handler_(segment_name, timer.ElapsedTime(),
        ProfilerUnitString(ProfilerUnit::kMS));

  for (auto&& [pid, pmd] : processes_)
    OutputMemoryUsage(pid, pmd);
}

void PerformanceProfiler::Start(const std::string& segment_name) {
  std::lock_guard lock(mutex_);
  SendComment("Starting " + segment_name);
  running_.insert(std::make_pair(segment_name, timer_precision_t{}));
}

void PerformanceProfiler::End(const std::string& segment_name) {
  std::unique_lock lock(mutex_);
  const auto&& it = running_.find(segment_name);
  if (it == running_.end())
    return;

  const auto& timer = it->second;
  output_handler_(
      segment_name, timer.ElapsedTime(), ProfilerUnitString(ProfilerUnit::kMS));
  running_.erase(it);

  lock.unlock();
  CollectMemoryUsage();
}

void PerformanceProfiler::AddProcess(uint32_t pid,
    const std::string& process_name) {
  std::lock_guard lock(mutex_);
  if (std::find_if(processes_.begin(), processes_.end(),
          [pid](auto&& it) { return it.first == pid; }) != processes_.end())
    return;

  detail::ProcessMemoryData pmd{};
  pmd.process_name = process_name;
  processes_.insert(std::make_pair(pid, pmd));
}

void PerformanceProfiler::RemoveProcesses(const std::string& process_name) {
  std::lock_guard lock(mutex_);
  for (auto it = processes_.begin(); it != processes_.end();) {
    const auto pid = it->first;
    const auto& pmd = it->second;
    if (it->second.process_name == process_name) {
      OutputMemoryUsage(pid, pmd);
      processes_.erase(it++);
    } else {
      ++it;
    }
  }
}

void PerformanceProfiler::SendComment(const std::string& comment) const {
  output_handler_(
      "# " + comment, 0, ProfilerUnitString(ProfilerUnit::kComment));
}

void PerformanceProfiler::OutputMemoryUsage(uint32_t pid,
    const detail::ProcessMemoryData& pmd) {
  constexpr auto get_label = [](auto&& pid, auto&& pname, auto&& suffix) {
    return pname + ":" + std::to_string(pid) + " " + suffix;
  };
  output_handler_(get_label(pid, pmd.process_name, "(current)"),
      static_cast<double>(pmd.private_size),
      ProfilerUnitString(ProfilerUnit::kMB));
  output_handler_(get_label(pid, pmd.process_name, " (peak)"),
      static_cast<double>(pmd.peak_working_size),
      ProfilerUnitString(ProfilerUnit::kMB));
}

PerformanceObject::PerformanceObject(
    std::shared_ptr<PerformanceProfiler> profiler,
    const std::string& segment_name)
    : profiler_(profiler), segment_name_(segment_name) {
  if (profiler_ != nullptr)
    profiler_->Start(segment_name_);
}

PerformanceObject::~PerformanceObject() {
  if (profiler_ != nullptr)
    profiler_->End(segment_name_);
}
}  // namespace performance
