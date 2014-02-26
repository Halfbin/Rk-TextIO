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
    struct soft_int_32
    {
      bool sign;
      u32  magnitude;

      template <typename int_t, typename en = typename std::enable_if <sizeof (int_t) <= 4>::type>
      soft_int_32 (int_t value) :
        sign      (value < 0),
        magnitude (sign ? -value : value)
      { }

    };

    struct soft_int_64
    {
      bool sign;
      u64  magnitude;

      template <typename int_t, typename en = typename std::enable_if <sizeof (int_t) == 8>::type>
      soft_int_64 (int_t value) :
        sign      (value < 0),
        magnitude (sign ? -value : value)
      { }

    };

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
