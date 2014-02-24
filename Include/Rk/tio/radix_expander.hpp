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

#ifndef RK_TEXTIO_API
#define RK_TEXTIO_API __declspec(dllimport)
#endif

namespace Rk
{
  namespace tio
  {
    template <typename int_t>
    class radix_expander
    {
      static_assert (std::is_unsigned <int_t>::value, "radix_expander may only use unsigned values");

      int_t value,
            radix,
            divisor;
      int   count;

    public:
      radix_expander () :
        value   (0),
        radix   (0),
        divisor (1),
        count   (0)
      { }

      radix_expander (int_t new_value, int_t new_radix) :
        value   (new_value),
        radix   (new_radix),
        divisor (1),
        count   (1)
      {
        if (radix < 2)
          throw std::invalid_argument ("Invalid radix");

        while (divisor < std::numeric_limits <int_t>::max () / radix)
        {
          auto next = divisor * radix;
          if (next > value)
            break;
          divisor = next;
          count++;
        }
      }
      
      radix_expander (const radix_expander&) = default;
      radix_expander& operator = (const radix_expander&) = default;

      int next_digit ()
      {
        auto digit = value / divisor;
        value %= divisor;
        divisor /= radix;
        count--;
        return int (digit);
      }

      int remaining () const
      {
        return count;
      }

      bool done () const
      {
        return count == 0;
      }

      bool significant () const
      {
        return value != 0;
      }

    };

    template <typename int_t>
    auto expand_radix (int_t magnitude, uint radix)
      -> radix_expander <std::make_unsigned_t <int_t>>
    {
      if (radix < 0)
        throw std::logic_error ("invalid radix");
      
      if (magnitude < 0)
        throw std::domain_error ("expand_radix cannot expand negative integers");

      return { magnitude, radix };
    }

  } // tio

} // Rk
