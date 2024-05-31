/*
 Performance profiler - Windows specific code.

 Copyright (c) 2024 Thomas Bruckschlegel. All rights reserved.
*/

#include "util/performance_profiler.hpp"
#include <algorithm>
#include <Windows.h>
#include <psapi.h>

#pragma comment(lib, "psapi")

namespace performance {
void PerformanceProfiler::CollectMemoryUsage() {
  std::unique_lock lock(mutex_);
  constexpr auto get_mb = [](auto&& v) -> size_t { return v / 1024 / 1024; };
  std::for_each(processes_.begin(), processes_.end(), [&](auto& it) {
    const auto pid = it.first;
    auto& pmd = it.second;
    auto process = OpenProcess(
        PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);
    if (process == nullptr)
      return;

    PROCESS_MEMORY_COUNTERS_EX pmc{};
    pmc.cb = sizeof(pmc);
    if (GetProcessMemoryInfo(process,
            reinterpret_cast<PPROCESS_MEMORY_COUNTERS>(&pmc), sizeof(pmc))) {
      pmd.peak_working_size = (((pmd.peak_working_size) >
                                   (get_mb(pmc.PeakWorkingSetSize)))
                                   ? (pmd.peak_working_size)
                                   : (get_mb(pmc.PeakWorkingSetSize)));
      pmd.private_size = get_mb(pmc.PrivateUsage);
      pmd.peak_working_size = (((pmd.peak_working_size) > (pmd.private_size))
                                   ? (pmd.peak_working_size)
                                   : (pmd.private_size));
      OutputMemoryUsage(pid, pmd);
    } else {
      detail::ProcessMemoryData p;
      p.process_name = pmd.process_name;
      OutputMemoryUsage(pid, p);
    }

    CloseHandle(process);
  });
}
}  // namespace performance
