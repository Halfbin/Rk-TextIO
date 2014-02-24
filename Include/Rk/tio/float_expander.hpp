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

#include <Rk/tio/radix_expander.hpp>

#ifndef RK_API
#define RK_API
#endif

namespace Rk
{
  namespace tio
  {
    enum float_kind
    {
      float_nan = 0,
      float_subnormal,
      float_zero,
      float_infinite,
      float_normal
    };

    class float_expander
    {
      float_kind kn;
      u64        margin;
      i32        exp,
                 dec_exp;
      u64        frac_part;
      bool       neg;

      radix_expander <u32> int_expander;

    public:
      RK_API float_expander (double value);

      u32 next_digit () // pre: !done ()
      {
        if (!int_expander.done ())
        {
          dec_exp--;
          return int_expander.next_digit ();
        }

        frac_part *= 10;
        u32 digit = u32 (frac_part >> -exp);
        frac_part &= (1ull << -exp) - 1;
        dec_exp--;
        margin *= 10;
        return digit;
      }

      float_kind kind () const
      {
        return kn;
      }

      bool negative () const
      {
        return neg;
      }

      int exponent () const
      {
        return dec_exp;
      }

      bool done () const
      {
        return frac_part <= margin && !int_expander.significant ();
      }

      bool normal    () const { return kn == float_normal;    }
      bool subnormal () const { return kn == float_subnormal; }
      bool zero      () const { return kn == float_zero;      }
      bool infinite  () const { return kn == float_infinite;  }
      bool nan       () const { return kn == float_nan;       }

      bool representable () const
      {
        return normal () || subnormal ();
      }

    };

  }

}
