#include "gtest/gtest.h"
#include "util/timer.hpp"
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

#define GTEST_COUT std::cerr << "[          ] [ INFO ] "

util::StopWatchTimer stop_watch;

TEST(Timer, MustBeGreaterThan2000ms) {
  util::ScopedTimer timer("test", [](auto id, auto ms) {
    GTEST_COUT << id << " took " << ms << "ms" << std::endl;
  });

  std::this_thread::sleep_for(2s);
  auto elapsed_time = timer.GetElapsedTimeInMs();
  EXPECT_TRUE(elapsed_time >= 2000);
}

TEST(Timer, PrettyPrintNoCallback) {
  util::ScopedTimer timer("test 2", nullptr);

  std::this_thread::sleep_for(2s);
  auto elapsed_time = timer.GetElapsedTimeInMs();
  auto pretty_time = timer.GetPrettyElapsedTime(elapsed_time);
  GTEST_COUT << pretty_time << std::endl;
  EXPECT_STREQ(pretty_time.c_str(), "00h00m02s");
}

TEST(Timer, StopWatchStart) {
  stop_watch.Start();
  std::this_thread::sleep_for(2s);
}

TEST(Timer, StopWatchPause) {
  stop_watch.Pause();
  std::this_thread::sleep_for(1s);
}

TEST(Timer, StopWatchCheckPaused) {
  EXPECT_TRUE(stop_watch.IsPaused());
}

TEST(Timer, StopWatchResume) {
  stop_watch.Resume();
  std::this_thread::sleep_for(1s);
}

TEST(Timer, StopWatchEnd) {
  const auto elapsed_time = stop_watch.ElapsedTime();
  GTEST_COUT << "Time elapsed: " << elapsed_time << std::endl;
  EXPECT_TRUE(elapsed_time >= 3000 && elapsed_time <= 3100);
}

TEST(Timer, Reset) {
  stop_watch.Stop();
  stop_watch.Start();
  std::this_thread::sleep_for(std::chrono::seconds(3));
}

TEST(Timer, GetElapsedTimeAfterReset) {
  const auto elapsed_time = stop_watch.ElapsedTime();
  GTEST_COUT << "Time elapsed: " << elapsed_time << std::endl;
  EXPECT_TRUE(elapsed_time >= 3000 && elapsed_time <= 3100);
  std::this_thread::sleep_for(std::chrono::seconds(1));
}

TEST(Timer, GetElapsedTimeAfterReset2) {
  const auto elapsed_time = stop_watch.ElapsedTime();
  GTEST_COUT << "Time elapsed: " << elapsed_time << std::endl;
  EXPECT_TRUE(elapsed_time >= 4000 && elapsed_time <= 4100);
}

TEST(Timer, GetElapsedTimeAfterPause) {
  stop_watch.Pause();
  std::this_thread::sleep_for(2s);
  stop_watch.Resume();

  // 2 seconds have been elapsed since the stop watch timer was paused but they
  // shouldn't be taken in account.
  const auto elapsed_time = stop_watch.ElapsedTime();
  GTEST_COUT << "Time elapsed: " << elapsed_time << std::endl;
  EXPECT_TRUE(elapsed_time >= 4000 && elapsed_time <= 4100);
}

TEST(Timer, ResetAndCheckFor5Secs) {
  stop_watch.Stop();
  stop_watch.Start();

  for (double i = 0; i < 5000; i += 1000) {
    const auto elapsed_time = stop_watch.ElapsedTime();
    GTEST_COUT << "Time elapsed: " << elapsed_time << std::endl;
    EXPECT_TRUE(elapsed_time >= i && elapsed_time <= i + 100);
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  const auto elapsed_time = stop_watch.ElapsedTime();
  GTEST_COUT << "Time elapsed: " << elapsed_time << std::endl;
  EXPECT_TRUE(elapsed_time >= 5000 && elapsed_time <= 5100);
}

TEST(Timer, StopWatchStop) {
  stop_watch.Stop();
}
