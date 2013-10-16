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

#include <utility>

#ifndef RK_API
#define RK_API
#endif

namespace Rk
{
  namespace tio
  {
    template <typename digit_map>
    struct float_format_params
    {
      uint       width,
                 integral_places;
      digit_map& digit_map;
    };

    enum float_kind
    {
      float_nan = 0,
      float_subnormal,
      float_zero,
      float_infinite,
      float_normal
    };

    template <typename char_t>
    class float_spelling
    {
      char_t     digs [18];
      u32        len;
      bool       neg;
      i32        exp;
      float_kind fk;

    public:
      explicit float_spelling (float_kind new_kind = float_nan) :
        len (0),
        exp (0),
        fk  (new_kind)
      { }
      
      auto buffer ()
        -> std::pair <char_t*, char_t*>
      {
        return { digs, std::end (digs) };
      }

      const char_t* begin () const
      {
        return digs;
      }

      const char_t* end () const
      {
        return digs + len;
      }

      auto digits () const
        -> std::pair <const char_t*, const char_t*> 
      {
        return { begin (), end () };
      }

      bool empty () const
      {
        return len == 0;
      }

      bool full () const
      {
        return len == 18;
      }

      void push_back (char_t digit)
      {
        digs [len++] = digit;
      }

      u32&        length   ()       { return len; }
      u32         length   () const { return len; }
      bool&       negative ()       { return neg; }
      bool        negative () const { return neg; }
      i32&        exponent ()       { return exp; }
      i32         exponent () const { return exp; }
      float_kind& kind     ()       { return fk; }
      float_kind  kind     () const { return fk; }

      bool normal () const
      {
        return fk == float_normal;
      }

      bool subnormal () const
      {
        return fk == float_subnormal;
      }

      bool zero () const
      {
        return fk == float_zero;
      }

      bool infinite () const
      {
        return fk == float_infinite;
      }

      bool nan () const
      {
        return fk == float_nan;
      }

      bool representable () const
      {
        return normal () || subnormal ();
      }

    };

    auto RK_API spell_float (f64 magnitude, char zero)
      -> float_spelling <char>;

    auto RK_API spell_float (f64 magnitude, char16 zero)
      -> float_spelling <char16>;

  }

}
