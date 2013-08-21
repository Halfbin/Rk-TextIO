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

// Implements
#include <Rk/TextIO/OutTransform.hpp>

// Uses
#include <Rk/TextIO/SpellNatural.hpp>
#include <Rk/TextIO/SpellFloat.hpp>

#include <Rk/Memory.hpp>

namespace Rk
{
  namespace TextIO
  {
    template <typename Device>
    class DefaultOutTransform :
      public OutTransform <Device>
    {
      template <typename Decomp>
      void apply_int (Device& d, Decomp i)
      {
        Char buffer [66];
        auto ptr = std::begin (buffer);
        auto end = std::end   (buffer);

        if (i.sign)
          *ptr++ = '-';

        auto speller = spell_natural (i.magnitude, 10, "0123456789");
        ptr = speller.get_digits (ptr, end);

        auto length = ptr - buffer;

        auto dest = d.acquire (length);
        copy (dest, length, buffer);
        d.release (length);
      }

    public:
      void apply (Device& d, StringRefBase <Char> s)
      {
        write (d, s.begin (), s.end ());
      }

      void apply (Device& d, Char c)
      {
        write (d, &c, 1);
      }

      void apply (Device& d, Int32Decomp i)
      {
        apply_int (d, i);
      }

      void apply (Device& d, Int64Decomp i)
      {
        apply_int (d, i);
      }

      void apply (Device& d, bool value)
      {
        if (value)
          write (d, "true", 4);
        else
          write (d, "false", 5);
      }

      void apply (Device& d, double value)
      {
        auto spelling = spell_float (value, '0');

        Char buffer [64];
        auto ptr = std::begin (buffer);
        auto end = std::end   (buffer);

        if (!spelling.empty ())
        {
          ptr = copy (ptr, end, spelling.begin (), spelling.end ());
          *ptr++ = Char ('E');

          if (spelling.exponent () < 0)
            *ptr++ = '-';

          auto speller = spell_natural (std::abs (spelling.exponent ()), 10, "0123456789");
          ptr = speller.get_digits (ptr, end);
        }
        else
        {
          if (spelling.negative ())
            *ptr++ = '-';
          else
            *ptr++ = '+';

          if (spelling.zero ())
            *ptr++ = '0';
          else if (spelling.infinite ())
            ptr = copy (ptr, end, "#inf", 4);
          else if (spelling.nan ())
            ptr = copy (ptr, end, "#nan", 4);
        }

        write (d, buffer, ptr);
      }

    };

  }

}
