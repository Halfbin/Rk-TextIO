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

#include <Rk/StringRef.hpp>
#include <Rk/Types.hpp>

#include <type_traits>

namespace Rk
{
  namespace TextIO
  {
    struct Int32Decomp
    {
      Int32Decomp () { }

      Int32Decomp (u32 mag, bool sig) :
        magnitude (mag),
        sign      (sig)
      { }
      
      u32  magnitude;
      bool sign;
    };

    struct Int64Decomp
    {
      Int64Decomp () { }

      Int64Decomp (u64 mag, bool sig) :
        magnitude (mag),
        sign      (sig)
      { }

      u64  magnitude;
      bool sign;
    };

    template <typename T>
    auto decomp (T value)
      -> typename std::enable_if <
          std::is_integral <T>::value &&
          std::is_signed   <T>::value &&
          sizeof (T) <= 4,
          Int32Decomp
        >::type
    {
      bool neg = value < T (0);
      return Int32Decomp (u32 (neg ? -value : value), neg);
    }

    template <typename T>
    auto decomp (T value)
      -> typename std::enable_if <
          std::is_integral <T>::value &&
          !std::is_signed  <T>::value &&
          sizeof (T) <= 4,
          Int32Decomp
        >::type
    {
      return Int32Decomp (value, false);
    }

    template <typename T>
    auto decomp (T value)
      -> typename std::enable_if <
          std::is_integral <T>::value &&
          std::is_signed   <T>::value &&
          sizeof (T) == 8,
          Int64Decomp
        >::type
    {
      bool neg = value < T (0);
      return Int64Decomp (u64 (neg ? -value : value), neg);
    }

    template <typename T>
    auto decomp (T value)
      -> typename std::enable_if <
          std::is_integral <T>::value &&
          !std::is_signed  <T>::value &&
          sizeof (T) == 8,
          Int64Decomp
        >::type
    {
      return Int64Decomp (value, false);
    }

    template <typename Device>
    class OutTransform
    {
    public:
      typedef typename Device::Char Char;

      virtual void apply (Device&, StringRefBase <Char>) = 0;
      virtual void apply (Device&, Char)                 = 0;
      virtual void apply (Device&, Int32Decomp)          = 0;
      virtual void apply (Device&, Int64Decomp)          = 0;
      virtual void apply (Device&, bool)                 = 0;
      virtual void apply (Device&, double)               = 0;

    };

  }

}
