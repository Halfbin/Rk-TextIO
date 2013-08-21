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

#include <Rk/TypeTraits.hpp>

namespace Rk
{
  namespace TextIO
  {
    template <typename Char>
    class OutDevice
    {
    public:
      typedef Char Char;

      virtual Char* acquire (uptr request) = 0;
      virtual void  release (uptr commit)  = 0;
      virtual void  flush   ()             = 0;
      virtual uptr  max     () const       = 0;

    };

    template <typename T>
    struct IsOutDevice :
      public std::is_base_of <OutDevice <typename T::Char>, T>
    { };

    template <typename Device, typename Char>
    auto write (Device& dev, const Char* chars, const Char* end)
      -> typename std::enable_if <IsOutDevice <Device>::value>::type
    {
      while (chars != end)
      {
        auto space = std::min (uptr (end - chars), dev.max ());
        auto dest = dev.acquire (space);
        copy (dest, chars, space);
        dev.release (space);
        chars += space;
      }
    }

    template <typename Device, typename Char>
    auto write (Device& dev, const Char* chars, uptr length)
      -> typename std::enable_if <IsOutDevice <Device>::value>::type
    {
      write (dev, chars, chars + length);
    }

  }

}
