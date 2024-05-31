#include "gtest/gtest.h"
#include "interval_map.hpp"

#define GTEST_COUT std::cerr << "[          ] [ INFO ] "

// get test range output
std::string get_test_range(interval_map<int, char>& itm,
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

TEST(IntervalMap, MustPass) {
  interval_map<int, char> example_interval('X');

  example_interval.assign_list(
      { { 44, 'F' }, { 1, 'A' }, { 2, 'B' }, { 5, 'A' }, { 7, 'B' } });

  const auto res = get_test_range(example_interval, -5, 100);
  GTEST_COUT << "Result " << res << std::endl;

  EXPECT_STREQ(res.c_str(),
      "[-5] -> X\n[-4] -> X\n[-3] -> X\n[-2] -> X\n[-1] -> X\n[0] -> X\n[1] -> "
      "A\n[2] -> B\n[3] -> B\n[4] -> B\n[5] -> A\n[6] -> A\n[7] -> B\n[8] -> "
      "B\n[9] -> B\n[10] -> B\n[11] -> B\n[12] -> B\n[13] -> B\n[14] -> "
      "B\n[15] -> B\n[16] -> B\n[17] -> B\n[18] -> B\n[19] -> B\n[20] -> "
      "B\n[21] -> B\n[22] -> B\n[23] -> B\n[24] -> B\n[25] -> B\n[26] -> "
      "B\n[27] -> B\n[28] -> B\n[29] -> B\n[30] -> B\n[31] -> B\n[32] -> "
      "B\n[33] -> B\n[34] -> B\n[35] -> B\n[36] -> B\n[37] -> B\n[38] -> "
      "B\n[39] -> B\n[40] -> B\n[41] -> B\n[42] -> B\n[43] -> B\n[44] -> "
      "F\n[45] -> F\n[46] -> F\n[47] -> F\n[48] -> F\n[49] -> F\n[50] -> "
      "F\n[51] -> F\n[52] -> F\n[53] -> F\n[54] -> F\n[55] -> F\n[56] -> "
      "F\n[57] -> F\n[58] -> F\n[59] -> F\n[60] -> F\n[61] -> F\n[62] -> "
      "F\n[63] -> F\n[64] -> F\n[65] -> F\n[66] -> F\n[67] -> F\n[68] -> "
      "F\n[69] -> F\n[70] -> F\n[71] -> F\n[72] -> F\n[73] -> F\n[74] -> "
      "F\n[75] -> F\n[76] -> F\n[77] -> F\n[78] -> F\n[79] -> F\n[80] -> "
      "F\n[81] -> F\n[82] -> F\n[83] -> F\n[84] -> F\n[85] -> F\n[86] -> "
      "F\n[87] -> F\n[88] -> F\n[89] -> F\n[90] -> F\n[91] -> F\n[92] -> "
      "F\n[93] -> F\n[94] -> F\n[95] -> F\n[96] -> F\n[97] -> F\n[98] -> "
      "F\n[99] -> F\n");
}

TEST(IntervalMap,
    MustFireException_consecutive_map_entries_must_not_contain_the_same_value) {
  interval_map<int, char> example_interval('X');
  bool exception_fired{};
  try {
    example_interval.assign_list(
        { { 1, 'A' }, { 2, 'B' }, { 3, 'A' }, { 5, 'A' } });

    const auto res = get_test_range(example_interval, -5, 10);
    GTEST_COUT << "Result " << res << std::endl;

  } catch (std::exception e) {
    exception_fired = true;
    GTEST_COUT << "EXPECTED: Exception caught: " << e.what() << std::endl;
  }
  EXPECT_TRUE(exception_fired);
}

TEST(IntervalMap, MustFireException_first_value_mismatch) {
  interval_map<int, char> example_interval('A');
  bool exception_fired{};
  try {
    example_interval.assign_list({ { 1, 'A' }, { 2, 'B' } });

    const auto res = get_test_range(example_interval, -5, 10);
    GTEST_COUT << "Result " << res << std::endl;

  } catch (std::exception e) {
    exception_fired = true;
    GTEST_COUT << "EXPECTED: Exception caught: " << e.what() << std::endl;
  }
  EXPECT_TRUE(exception_fired);
}

TEST(IntervalMap, MustFireException_Error) {
  interval_map<int, char> example_interval('X');
  bool exception_fired{};
  try {
    example_interval.assign(1, 3, 'B');
    example_interval.assign(3, 4, 'A');
    example_interval.assign(-8, -4, 'F');

    const auto res = get_test_range(example_interval, -20, 10);
    GTEST_COUT << "Result " << res << std::endl;

  } catch (std::exception e) {
    exception_fired = true;
    GTEST_COUT << "EXPECTED: Exception caught: " << e.what() << std::endl;
  }
  EXPECT_FALSE(exception_fired);
}
