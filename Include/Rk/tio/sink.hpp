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
    template <typename char_t>
    class sink
    {
    public:
      typedef char_t char_t;

      virtual char_t* acquire (size_t request) = 0;
      virtual void    release (size_t commit)  = 0;
      virtual void    flush   ()               = 0;
      virtual size_t  max     () const         = 0;

    };

    template <typename T, typename char_t>
    struct is_sink :
      public std::is_base_of <sink <char_t>, T>
    { };

    template <typename sink_t, typename char_t>
    auto write (sink_t& sink, const char_t* chars, const char_t* end)
      -> std::enable_if_t <is_sink <sink_t, char_t>::value>
    {
      while (chars != end)
      {
        auto space = std::min (size_t (end - chars), sink.max ());
        auto dest = sink.acquire (space);
        copy (dest, chars, space);
        sink.release (space);
        chars += space;
      }
    }

    template <typename sink_t, typename char_t>
    auto write (sink_t& sink, const char_t* chars, size_t length)
      -> std::enable_if_t <is_sink <sink_t, char_t>::value>
    {
      write (sink, chars, chars + length);
    }

  }

}
