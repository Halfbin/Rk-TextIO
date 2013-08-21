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

#include <Rk/TextIO/SpellNatural.hpp>

#include <stdexcept>

namespace Rk
{
  namespace TextIO
  {
    // Spell naturals independent of specifics
    /*template <typename Unit, typename T>
    uptr spell_natural_impl (
      Unit* buffer,
      uptr  size,
      Unit  zero,
      T     magnitude,
      uint  radix,
      Unit  group_seperator,
      uint  group_length)
    {
      if (!buffer || size == 0)
        throw std::invalid_argument ("Invalid buffer");

      if (radix < 2 || T (zero) + radix > (T) std::numeric_limits <Unit>::max ())
        throw std::invalid_argument ("Radix out-of-range");

      T divisor = 1;
      while (divisor < std::numeric_limits <T>::max () / radix)
        divisor *= radix;

      if (group_length == 0)
        group_length = size;

      uptr index = 0;

      while (divisor != 0 && index != size)
      {
        for (uint
          group_digit = 0;
          group_digit  != group_length
            && divisor != 0
            && index   != size;
          group_digit++)
        {
          auto digit = zero + Unit (magnitude / divisor);
          magnitude %= divisor;
          divisor /= radix;

          // Omit leading zeros
          if (digit == zero && index == 0)
            continue;

          buffer [index++] = digit;
        }
        
        if (index != size && divisor != 0)
          buffer [index++] = group_seperator;
      }

      if (divisor != 0)
        throw std::runtime_error ("Buffer too small");

      if (index == 0)
        buffer [index++] = zero;

      return index;
    }

    uptr RK_API spell_natural (char* buf, uptr size, char zero, u32 mag, uint radix, char gsep, uint glen)
    {
      return spell_natural_impl (buf, size, zero, mag, radix, gsep, glen);
    }

    uptr RK_API spell_natural (char* buf, uptr size, char zero, u64 mag, uint radix, char gsep, uint glen)
    {
      return spell_natural_impl (buf, size, zero, mag, radix, gsep, glen);
    }

    uptr RK_API spell_natural (char16* buf, uptr size, char16 zero, u32 mag, uint radix, char16 gsep, uint glen)
    {
      return spell_natural_impl (buf, size, zero, mag, radix, gsep, glen);
    }

    uptr RK_API spell_natural (char16* buf, uptr size, char16 zero, u64 mag, uint radix, char16 gsep, uint glen)
    {
      return spell_natural_impl (buf, size, zero, mag, radix, gsep, glen);
    }*/

  }

}
