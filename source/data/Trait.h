/**
 *  @note This file is part of Empirical, https://github.com/devosoft/Empirical
 *  @copyright Copyright (C) Michigan State University, MIT Software license; see doc/LICENSE.md
 *  @date 2018
 *
 *  @file  Trait.h
 *  @brief Directly measure a target quality about a type of object.
 *
 *  These objects are able to measure a specific trait on another object.  They
 *  (will eventually) interface smoothly with DataNodes for data collection.
 */

#ifndef EMP_DATA_TRAIT_H
#define EMP_DATA_TRAIT_H

#include <string>

#include "../base/assert.h"
#include "../tools/Range.h"
#include "../tools/string_utils.h"

namespace emp {

  template <typename TARGET_T>
  class BaseTrait {
  public:
    virtual std::string EvalString(TARGET_T & target) = 0;
    virtual double EvalValue(TARGET_T & target) = 0;
  };

  template <typename TARGET_T, typename VALUE_T=double>
  class Trait : public BaseTrait<TARGET_T> {
  public:
    using target_t = TARGET_T;
    using value_t = VALUE_T;
    using fun_t = std::function<value_t(target_t &)>;
    using range_t = emp::Range<value_t>;

  protected:
    std::string name;   ///< Unique name for this trait.
    std::string desc;   ///< Full description explaining this trait.
    fun_t fun;          ///< Function to retrieve trait value from target.
    range_t range;      ///< Limits on legal values (defaults to extremes)

  public:
    Trait(const std::string & _n, const fun_t & _f)
      : name(_n), fun(_f), range() { ; }
    Trait(const std::string & _n, const fun_t & _f, value_t min, value_t max)
      : name(_n), fun(_f), range(min, max) { ; }
    Trait(const std::string & _n, const fun_t & _f, const range_t & _r)
      : name(_n), fun(_f), range(_r) { ; }

    const std::string & GetName() const { return name; }
    const std::string & GetDesc() const { return desc; }
    const fun_t & GetFun() const { return fun; }
    const range_t & GetRange() const { return range; }

    void SetName(const std::string & _in) { name = _in; }
    void SetDesc(const std::string & _in) { desc = _in; }
    void SetFun(const fun_t & _in) { fun = _in; }
    void SetRange(const range_t & _in) { range = _in; }
    void SetRange(value_t min, value_t max) { range.Set(min, max); }
    void SetMin(value_t min) { range.SetLower(min); }
    void SetMax(value_t max) { range.SetUpper(max); }

    value_t Eval(target_t & target) { return fun(target); }
    value_t EvalLimit(target_t & target) { return range.Limit(fun(target)); }
    std::string EvalString(target_t & target) { return std::to_string(EvalLimit(target)); }
    double EvalValue(target_t & target) { return (double) EvalLimit(target); }
  };


}

#endif
