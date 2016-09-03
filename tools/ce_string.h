//  This file is part of Empirical, https://github.com/devosoft/Empirical
//  Copyright (C) Michigan State University, 2016.
//  Released under the MIT Software license; see doc/LICENSE
//
//  ce_string defines a limited string object for use within a constexpr class or function.
//
//  Developer note:
//   @CAO: Not ready from prime time-- for some reason I can't get Set to work as constexpr
//         (when compiled with C++14).

#ifndef EMP_CE_STRING_H
#define EMP_CE_STRING_H

#include <string>
#include <type_traits>

#include "functions.h"

namespace emp {

  class ce_string {
  public:
    using size_t = std::size_t;

  private:
    const char * m_str;
    size_t m_size;

    constexpr bool IsEqual(const ce_string & in, int first=0) const {
      return (m_size == in.m_size) &&
             ((m_size == first) || (m_str[first] == in.m_str[first] && IsEqual(in, first+1)));
    }
    constexpr bool IsLess(const ce_string & in, int first=0) const {
      return (first == in.m_size) ? false :
             ((first == m_size) || (m_str[first] < in.m_str[first] || IsLess(in, first+1)));
    }
  public:
    template<size_t N>
    constexpr ce_string(const char (&in)[N]) : m_str(in), m_size(N-1) { ; }
    constexpr ce_string(const ce_string & in) = default;

    constexpr bool operator==(const ce_string & in) const {
      return IsEqual(in);
    }
    constexpr bool operator!=(const ce_string & in) const { return !(*this == in); }
    constexpr bool operator<(const ce_string & in) const {
      return IsLess(in);
    }
    constexpr bool operator>(const ce_string & in) const { return (in < *this); }
    constexpr bool operator<=(const ce_string & in) const { return !(in < *this); }
    constexpr bool operator>=(const ce_string & in) const { return !(*this < in); }

    constexpr size_t size() const { return m_size; }

    constexpr char operator[](const size_t pos) const {
      // static_assert(pos < m_size, "ce_array index out of range.");
      return m_str[pos];
    }

    operator std::string() const { return std::string(m_str); }
  };

  // @CAO Grrr...  not sure why this operator breaks everything....
  // std::ostream & operator<<(std::ostream & out, const emp::ce_string & str) {
  //   out << (std::string) str;
  //   return out;
  // }

}

#endif
