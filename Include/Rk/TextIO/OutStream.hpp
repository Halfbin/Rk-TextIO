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

#include <Rk/TextIO/Forward.hpp>

#include <Rk/StringRef.hpp>
#include <Rk/Memory.hpp>

#include <type_traits>
#include <utility>

namespace Rk
{
  namespace TextIO
  {
    template <
      typename                  Char,
      template <typename> class Device,
      template <typename> class Transform>
    struct OutStreamPolicy
    {
      typedef Char               Char;
      typedef Device    <Char>   Device;
      typedef Transform <Device> Transform;

    };

    template <typename Policy>
    class OutStream
    {
    public:
      typedef typename Policy::Char      Char;
      typedef typename Policy::Device    Device;
      typedef typename Policy::Transform Transform;
      
    private:
      Device    dev;
      Transform xfm;

    public:
      explicit OutStream (Device new_dev = Device (), Transform new_xfm = Transform ()) :
        dev (std::forward <Device>    (new_dev)),
        xfm (std::forward <Transform> (new_xfm))
      { }
      
      OutStream (const OutStream& other) :
        dev (other.dev),
        xfm (other.xfm)
      { }
      
      OutStream (OutStream&& other) :
        dev (std::forward <Device>    (other.dev)),
        xfm (std::forward <Transform> (other.xfm))
      { }
      
      OutStream& operator = (const OutStream& other)
      {
        dev = other.dev;
        xfm = other.xfm;
        return *this;
      }
      
      OutStream& operator = (OutStream&& other)
      {
        dev = std::move (other.dev);
        xfm = std::move (other.xfm);
        return *this;
      }

            Device&    device    ()       { return dev; }
      const Device&    device    () const { return dev; }
            Transform& transform ()       { return xfm; }
      const Transform& transform () const { return xfm; }
      
      // Strings
      template <typename T>
      auto operator << (const T& value)
        -> typename std::enable_if <
            std::is_convertible <const T&, StringRefBase <Char>>::value,
            OutStream&
          >::type
      {
        xfm.apply (dev, StringRefBase <Char> (value));
        return *this;
      }

      // Chars
      template <typename T>
      auto operator << (const T& value)
        -> typename std::enable_if <
            std::is_same <T, Char>::value,
            OutStream&
          >::type
      {
        xfm.apply (dev, value);
        return *this;
      }

      // Ints
      template <typename T>
      auto operator << (const T& value)
        -> typename std::enable_if <
            std::is_integral <T>      ::value &&
            !std::is_same    <T, Char>::value &&
            !std::is_same    <T, bool>::value,
            OutStream&
          >::type
      {
        xfm.apply (dev, decomp (value));
        return *this;
      }

      // bool
      template <typename T>
      auto operator << (const T& value)
        -> typename std::enable_if <
            std::is_same <T, bool>::value, OutStream&
          >::type
      {
        xfm.apply (dev, value);
        return *this;
      }

      // Floats
      template <typename T>
      auto operator << (const T& value)
        -> typename std::enable_if <
            std::is_floating_point <T>::value,
            OutStream&
          >::type
      {
        xfm.apply (dev, double (value));
        return *this;
      }

    }; // OutStream

  } // TextIO

} // Rk
