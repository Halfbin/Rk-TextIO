//
// Copyright (C) 2013 Rk
// Permission to use, copy, modify and distribute this file for any purpose is hereby granted without fee or other
// limitation, provided that
//  - this notice is reproduced verbatim in all copies and modifications of this file, and
//  - neither the names nor trademarks of the copyright holder are used to endorse any work which includes or is derived
//    from this file.
// No warranty applies to the use, either of this software, or any modification of it, and Rk disclaims all liabilities
// in relation to such use.
//

#pragma once

#include <Rk/string_ref.hpp>
#include <Rk/types.hpp>

#include <type_traits>

namespace Rk
{
  namespace tio
  {
    template <typename Int>
    struct soft_int
    {
      soft_int () = default;

      soft_int (Int mag, bool sig) :
        magnitude (mag),
        sign      (sig)
      { }
      
      Int  magnitude;
      bool sign;
    };

    typedef soft_int <u32> soft_int_32;
    typedef soft_int <u64> soft_int_64;

    template <typename Int, bool small = sizeof (Int) <= 4>
    struct soft_int_equiv;

    template <typename Int>
    struct soft_int_equiv <Int, true>
    {
      typedef soft_int_32 type;
    };

    template <typename Int>
    struct soft_int_equiv <Int, false>
    {
      typedef soft_int_64 type;
    };

    template <typename Int>
    using soft_int_t = typename soft_int_equiv <Int>::type;

    template <typename T>
    auto decompose (T value)
      -> typename std::enable_if <
          std::is_integral <T>::value,
          soft_int_t <T>
        >::type
    {
      if (std::is_signed <T>::value && value < 0)
        return soft_int_t <T> (-value, true);
      else
        return soft_int_t <T> (value, false);
    }

    template <typename sink>
    class speller
    {
    public:
      typedef typename sink::char_t char_t;

      virtual void apply (sink&, string_ref_base <char_t>) = 0;
      virtual void apply (sink&, char_t)                   = 0;
      virtual void apply (sink&, soft_int_32)              = 0;
      virtual void apply (sink&, soft_int_64)              = 0;
      virtual void apply (sink&, bool)                     = 0;
      virtual void apply (sink&, double)                   = 0;

    };

  }

}
