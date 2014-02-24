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
    template <typename int_t>
    struct soft_int
    {
      int_t magnitude;
      bool  sign;
    };

    using soft_int_32 = soft_int <u32>;
    using soft_int_64 = soft_int <u64>;

    template <typename int_t>
    auto decompose (int_t value)
      -> std::conditional_t <sizeof (int_t) <= 4, soft_int_32, soft_int_64>
    {
      static_assert (std::is_integral <int_t>::value, "Rk::tio::decompose is for integrals only");

      if (std::is_signed <int_t>::value && value < 0)
        return { -value, true };
      else
        return { value, false };
    }

    template <typename unit_t>
    class digit_map
    {
    public:
      typedef string_ref_base <unit_t> strref_t;

      const strref_t* digs;
      size_t          n;

      strref_t operator [] (int i) const
      {
        return digs [i];
      }

      size_t count () const
      {
        return n;
      }

    };

    typedef digit_map <char> cdigit_map;

    template <typename unit_t>
    struct numeric_ortho
    {
      typedef digit_map <unit_t>       digits_t;
      typedef string_ref_base <unit_t> strref_t;

      digits_t digits;
      strref_t plus,
               minus,
               group_seperator,
               radix_point,
               infinity,
               nan,
               exponent_seperator;
      int      group_length,
               group_final;

    };

    typedef numeric_ortho <char> cnumeric_ortho;

    struct numeric_ortho_options
    {
      uint radix;
      bool group,
           show_plus;
    };

    template <typename unit_t>
    struct boolean_ortho
    {
      string_ref_base <unit_t> verum,
                               falsum;
    };

    typedef boolean_ortho <char> cboolean_ortho;
    
  }

}
