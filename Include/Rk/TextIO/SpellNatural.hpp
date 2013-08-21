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

#include <Rk/Types.hpp>

#include <type_traits>

#ifndef RK_API
#define RK_API
#endif

namespace Rk
{
  namespace TextIO
  {
    template <typename T, typename DigitMap>
    class NaturalSpeller
    {
      static_assert (std::is_unsigned <T>::value, "NaturalSpeller may only use unsigned values");

      T         magnitude,
                radix,
                divisor;
      DigitMap& digit_map;

      static DigitMap& digit_map_val ();

    public:
      typedef typename std::remove_reference <decltype (digit_map_val () [0])>::type
        Unit;

      NaturalSpeller (T new_magnitude, T new_radix, DigitMap& new_digit_map) :
        magnitude (new_magnitude),
        radix     (new_radix),
        divisor   (1),
        digit_map (new_digit_map)
      {
        uptr map_size = std::end (digit_map) - std::begin (digit_map);
        if (radix < 2 || radix > map_size)
          throw std::invalid_argument ("Invalid radix");

        while (divisor < std::numeric_limits <T>::max () / radix)
        {
          auto next = divisor * radix;
          if (next > magnitude)
            break;
          divisor = next;
        }
      }
      
      auto next_digit ()
        -> Unit
      {
        auto digit = digit_map [magnitude / divisor];
        magnitude %= divisor;
        divisor /= radix;
        return digit;
      }

      template <typename Iter>
      auto get_digits (Iter dest, Iter limit, uptr count = 100)
        -> Iter
      {
        while (!done () && dest != limit && count--)
          *dest++ = next_digit ();
        return dest;
      }

      template <typename Iter>
      auto get_digits (Iter dest, uptr capacity, uptr count = 100)
        -> Iter
      {
        return get_digits (dest, dest + capacity, count);
      }

      bool done () const
      {
        return divisor == 0;
      }

    };

    template <typename T, typename R, typename DigitMap>
    auto spell_natural (T magnitude, R radix, DigitMap& digit_map)
      -> NaturalSpeller <typename std::make_unsigned <T>::type, DigitMap>
    {
      if (magnitude < 0)
        throw std::domain_error ("spell_natural cannot spell negative integers");

      return NaturalSpeller <typename std::make_unsigned <T>::type, DigitMap> (
        magnitude, radix, digit_map
      );
    }

    /*uptr RK_API spell_natural (
      char* buffer,
      uptr  size,
      char  zero,
      u32   magnitude,
      uint  radix,
      char  group_seperator = 0,
      uint  group_length = 0
    );
    
    uptr RK_API spell_natural (
      char* buffer,
      uptr  size,
      char  zero,
      u64   magnitude,
      uint  radix,
      char  group_seperator = 0,
      uint  group_length = 0
    );*/

    

  }

}
