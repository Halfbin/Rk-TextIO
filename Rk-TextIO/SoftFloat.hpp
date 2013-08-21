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

namespace Rk
{
  namespace TextIO
  {
    u64 as_bits (f64 value)
    {
      union U { f64 f; u64 i; } u = { value };
      return u.i;
    }

    struct SoftFloat
    {
      u64 significand;
      i32 exponent;

      SoftFloat () { }

      SoftFloat (u64 new_significand, i32 new_exponent) :
        significand (new_significand),
        exponent    (new_exponent)
      { }
      
      SoftFloat (f64 value)
      {
        significand = as_bits (value) & ((1ull << 52) - 1);

        auto raw_exp = u32 (as_bits (value) >> 52) & 0x7ff;
        if (raw_exp != 0)
        {
          significand |= 1ull << 52;
          exponent = i32 (raw_exp) - 1075;
        }
        else // subnorm
        {
          exponent = -1074;
        }
      }
      
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

      /*friend SoftFloat operator - (SoftFloat lhs, SoftFloat rhs)
      {
        return SoftFloat (lhs.significand - rhs.significand, lhs.exponent);
      }*/

      SoftFloat& operator *= (SoftFloat rhs)
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

      friend SoftFloat operator * (SoftFloat lhs, SoftFloat rhs)
      {
        lhs *= rhs;
        return lhs;
      }

    };

  }

}
