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

#include <Rk/types.hpp>

#include <type_traits>

#ifndef RK_API
#define RK_API
#endif

namespace Rk
{
  namespace tio
  {
    template <typename int_t, typename digit_map_t>
    class natural_speller
    {
      static_assert (std::is_unsigned <int_t>::value, "NaturalSpeller may only use unsigned values");

      int_t              magnitude,
                         radix,
                         divisor;
      const digit_map_t& digit_map;

      static const digit_map_t& digit_map_val ();

    public:
      typedef std::remove_reference_t <decltype (digit_map_val () [0])>
        char_t;

      natural_speller (int_t new_magnitude, int_t new_radix, digit_map_t& new_digit_map) :
        magnitude (new_magnitude),
        radix     (new_radix),
        divisor   (1),
        digit_map (new_digit_map)
      {
        size_t map_size = std::end (digit_map) - std::begin (digit_map);
        if (radix < 2 || radix > map_size)
          throw std::invalid_argument ("Invalid radix");

        while (divisor < std::numeric_limits <int_t>::max () / radix)
        {
          auto next = divisor * radix;
          if (next > magnitude)
            break;
          divisor = next;
        }
      }
      
      auto next_digit ()
        -> char_t
      {
        auto digit = digit_map [magnitude / divisor];
        magnitude %= divisor;
        divisor /= radix;
        return digit;
      }

      template <typename iter>
      auto get_digits (iter dest, iter limit, size_t count = 100)
        -> iter
      {
        while (!done () && dest != limit && count--)
          *dest++ = next_digit ();
        return dest;
      }

      template <typename Iter>
      auto get_digits (Iter dest, size_t capacity, size_t count = 100)
        -> Iter
      {
        return get_digits (dest, dest + capacity, count);
      }

      bool done () const
      {
        return divisor == 0;
      }

    };

    template <typename int_t, typename digit_map_t>
    auto spell_natural (int_t magnitude, uint radix, digit_map_t& digit_map)
      -> natural_speller <std::make_unsigned_t <int_t>, digit_map_t>
    {
      if (magnitude < 0)
        throw std::domain_error ("spell_natural cannot spell negative integers");

      return { magnitude, radix, digit_map };
    }

  } // tio

} // Rk
