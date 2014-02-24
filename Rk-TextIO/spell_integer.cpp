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

// Implements
#include <Rk/tio/spell_integer.hpp>

// Uses
#include <Rk/tio/radix_expander.hpp>

#include <array>

namespace Rk
{
  namespace tio
  {
    template <typename re_t>
    void expand_impl (csink& sn, const cnumeric_ortho& ortho, re_t& re, int group_len, int group_final)
    {
      int group_counter = re.remaining () - group_final;

      while (!re.done ())
      {
        sn.write (ortho.digits [re.next_digit ()]);

        group_counter--;
        if (group_counter >= 0 && (group_counter % group_len) == 0)
          sn.write (ortho.group_seperator);
      }
    }

    void spell_prefix (csink& sn, const cnumeric_ortho& ortho, bool neg, const numeric_ortho_options& opts)
    {
      if (opts.radix > ortho.digits.count ())
        throw std::logic_error ("Unsupported radix");

      if (neg)
        sn.write (ortho.minus);
      else if (opts.show_plus)
        sn.write (ortho.plus);
    }

    void group_params (bool group, const cnumeric_ortho& ortho, int& group_len, int& group_final)
    {
      group_len = ortho.group_length;
      if (!group || group_len < 0)
        group_len = 0;

      group_final = std::max (ortho.group_final, 0);
      if (!group)
        group_final = 1000;
    }

    void spell_integer (csink& sn, soft_int_32 value, const cnumeric_ortho& ortho, const numeric_ortho_options& opts)
    {
      spell_prefix (sn, ortho, value.sign, opts);

      int group_len, group_final;
      group_params (opts.group, ortho, group_len, group_final);

      auto re = expand_radix (value.magnitude, opts.radix);
      expand_impl (sn, ortho, re, group_len, group_final);
    }

    void spell_integer (csink& sn, soft_int_64 value, const cnumeric_ortho& ortho, const numeric_ortho_options& opts)
    {
      spell_prefix (sn, ortho, value.sign, opts);

      int group_len, group_final;
      group_params (opts.group, ortho, group_len, group_final);

      auto re = expand_radix (value.magnitude, opts.radix);
      expand_impl (sn, ortho, re, group_len, group_final);
    }

  }

}
