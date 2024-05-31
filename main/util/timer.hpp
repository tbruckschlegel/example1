/*
 High Resolution timer class to measure performance.

 Copyright (c) 2024 Thomas Bruckschlegel. All rights reserved.
*/

#pragma once

#include <atomic>
#include <string>
#include <unordered_map>
#include <chrono>
#include <mutex>
#include <functional>

namespace util {
typedef std::function<void(const std::string& label, double time_in_ms)>
    timer_callback_t;

/**
 * \brief A general timer object to measure things performance based on a
 * std::chrono time reference, such as std::chrono::milliseconds.
 **/
template<typename T>
class Timer {
public:
  Timer() {
    start_ = std::chrono::high_resolution_clock::now();
  }

  double ElapsedTime() const {
    const auto now = std::chrono::high_resolution_clock::now();
    const auto ms = std::chrono::duration_cast<T>(now - start_);
    return double(ms.count());
  }

protected:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_;
};

/**
 * \brief A scoped timer using std::chrono::microseconds to measure the time
 * taken in a given code block.
 **/
class ScopedTimer : public Timer<std::chrono::microseconds> {
public:
  ScopedTimer() = delete;

  /**
   * \brief Start keeping track of elapsed time on a given code block. The
   * elapsed time will stop being measured when the ScopedTimer destructor is
   * triggered (i.e. when the ScopeTimer instance goes out of scope)
   * \param[in] id a string that identifies the code block.
   * \param[in] callback a timer_callback_t function that is called when this
   * timer instance goes out of scope.
   **/
  explicit ScopedTimer(const std::string& id, timer_callback_t callback)
      : id_(id), callback_(callback) {
  }

  /**
   * \brief Stop keeping track of elapsed time on a given code block and save
   * the elapsed time for later dumping - see \ref DumpRuns (i.e. when the app
   * is terminating or whenever needed)
   **/
  ~ScopedTimer() {
    const auto elapsed_ms = ElapsedTime() / 1000.0;
    if (callback_ != nullptr)
      callback_(id_, elapsed_ms);
  }

  /**
   * \brief Get the elapsed time since the start of this timer.
   * \return the elapsed time in milliseconds. The timer goes on after this call
   * and will continue till the instance goes out of scope.
   **/
  double GetElapsedTimeInMs() const {
    return ElapsedTime() / 1000.0;
  }

  /**
   * \brief Return a pretty string formatted as 00h00m00s for a given elapsed
   * time.
   * \param[in] elapsed_time the elapsed time in ms (call GetElapsedTimeInMs()
   * to get the a ScopedTimer elapsed time.
   * \return a string with the elapsed time in pretty format.
   **/
  static std::string GetPrettyElapsedTime(double elapsed_time) {
    const auto execution_time = elapsed_time / 1000.0;
    const auto hours = (static_cast<int>(execution_time / 60 / 60)),
               minutes = (static_cast<int>(execution_time / 60) % 60),
               seconds = static_cast<int>(execution_time) % 60;
    const std::string time = std::format(
        "{:02d}h{:02d}m{:02d}s", hours, minutes, seconds);
    return time;
  }

private:
  std::string id_;
  timer_callback_t callback_;
};

class StopWatchTimer : public Timer<std::chrono::milliseconds> {
public:
  explicit StopWatchTimer(bool start = false) {
    if (start)
      Start();
  }

  void Start() {
    last_check_ = std::chrono::high_resolution_clock::now();
    started_ = true;
  }

  void Pause() {
    if (paused_)
      return;

    const auto now = std::chrono::high_resolution_clock::now();
    const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - last_check_);
    sum_ += ms;
    paused_ = true;
  }

  void Resume() {
    if (started_ && !paused_)
      return;

    Start();
    paused_ = false;
  }

  void Stop() {
    if (!started_)
      return;

    started_ = false;
    paused_ = false;
    sum_ = {};
  }

  double ElapsedTime() const {
    if (started_ && !paused_) {
      const auto now = std::chrono::high_resolution_clock::now();
      const auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
          now - last_check_);
      last_check_ = now;
      sum_ += ms;
    }

    return double(sum_.count());
  }

  bool IsPaused() const {
    return paused_;
  }

private:
  bool started_ = false;
  bool paused_ = false;
  mutable std::chrono::time_point<std::chrono::high_resolution_clock>
      last_check_;
  mutable std::chrono::duration<double, std::milli> sum_{};
};
}  // namespace util
