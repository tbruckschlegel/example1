/*
 Interval Map.

 Copyright (c) 2024 Thomas Bruckschlegel. All rights reserved.
*/

#include <map>
#include <iostream>
#include <utility>
#include <string>

template<typename K, typename V>
class interval_map {
  V m_valBegin;
  std::map<K, V> m_map;

public:
  /**
   * @brief Constructor associates whole range of K with val.
   * @param val the beginning value.
   */
  interval_map(V const& val) : m_valBegin(val) {
  }

  /**
   * @brief Initialize a list of pairs.
   * @note Helper function not properly templated.
   * @param list of tupels to add to the map.
   */
  void assign_list(std::initializer_list<std::tuple<K, V>> list) {
    for (const auto& [key, value] : list) {
      assign(key, key + 1, value);
    }
  }

  /**
   * @brief Assign a range of values to the map.
   * @param key_begin start of the range.
   * @param key_end end of the range.
   * @param val value of range.
   */
  void assign(K const& key_begin, K const& key_end, V const& val) {

    if (!(key_begin < key_end)) {
      throw std::exception(
          "keyEnd & keyBegin are invalid");
    }

    // find the first element that is not less than keyBegin
    auto iterator_begin = m_map.lower_bound(key_begin);

    if (iterator_begin == m_map.begin() && val == m_valBegin) {
       throw std::exception(
          "first entry in m_map must not contain the same value as "
          "m_valBegin");
    }

    // if the element is not the first and its previous value is the same
    // as val, we bail out
    if (iterator_begin != m_map.begin() &&
        std::prev(iterator_begin)->second == val) {
       throw std::exception(
          "consecutive map entries must not contain the same value");
    }

    // find the first element that is greater than keyEnd
    auto iterator_end = m_map.upper_bound(key_end);

    // erase the elements in the range [iterator_begin, iterator_end)
    iterator_end = m_map.erase(iterator_begin,
        iterator_end);  // iterator_end is now valid or m_map.end()

    // insert the new interval
    auto hint = iterator_end;  // use iterator_end as a hint for the next insert
    m_map.emplace_hint(hint, key_begin, val);
    hint = m_map.upper_bound(key_begin);  // update hint after the first insert
    m_map.emplace_hint(hint, key_end, val);
  }

  /**
   * @brief Look-up of the value associated with key.
   * @param key to look-up from the map.
   */
  V const& operator[](K const& key) const {
    auto it = m_map.upper_bound(key);
    if (it == m_map.begin()) {
      return m_valBegin;
    } else {
      return (--it)->second;
    }
  }
};
