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

#include <Rk/type_traits.hpp>
#include <Rk/types.hpp>

namespace Rk
{
  namespace tio
  {
    struct soft_float
    {
      u64 significand;
      i32 exponent;

      soft_float () = default;

      soft_float (u64 new_significand, i32 new_exponent) :
        significand (new_significand),
        exponent    (new_exponent)
      { }
      
      void normalize ()
      {
        // Faster when normalizing IEEE 754 doubles
        while ((significand & (0x3ffull << 54)) == 0)
        {
          significand <<= 10;
          exponent -= 10;
        }

        while ((significand & (1ull << 63)) == 0)
        {
          significand <<= 1;
          exponent--;
        }
      }

      soft_float& operator *= (soft_float rhs)
      {
        const u64 
          mask32 = (1ull << 32) - 1,
          a = significand >> 32,
          b = significand & mask32,
          c = rhs.significand >> 32,
          d = rhs.significand & mask32,
          ac = a * c,
          bc = b * c,
          ad = a * d,
          bd = b * d,
          t = (bd >> 32) + (ad & mask32) + (bc & mask32) + (1ull << 31);

        significand = ac + (ad >> 32) + (bc >> 32) + (t >> 32);
        exponent += rhs.exponent + 64;

        return *this;
      }

      friend soft_float operator * (soft_float lhs, soft_float rhs)
      {
        lhs *= rhs;
        return lhs;
      }

    };

  }

}
