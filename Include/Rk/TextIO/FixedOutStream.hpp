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
#include <Rk/TextIO/OutDevice.hpp>
#include <Rk/TextIO/OutStream.hpp>

// Uses
#include <Rk/TextIO/DefaultOutTransform.hpp>

namespace Rk
{
  namespace TextIO
  {
    template <typename Char>
    class FixedOutDevice :
      public OutDevice <Char>
    {
      Char* ptr,
          * end;

    public:
      FixedOutDevice (Char* new_ptr, Char* new_end) :
        ptr (new_ptr),
        end (new_end)
      {
        if (ptr >= end)
          throw std::invalid_argument ("Invalid range");
      }
      
      Char* acquire (uptr request)
      {
        if (request > max ())
          throw std::length_error ("Out of fixed buffer space");
        return ptr;
      }

      void release (uptr commit)
      {
        ptr += commit;
      }

      void flush ()
      { }

      uptr max () const
      {
        return uptr (end - ptr);
      }

    };

    template <
      typename                  Char,
      template <typename> class Transform = DefaultOutTransform>
    struct FixedOutStreamPolicy :
      OutStreamPolicy <Char, FixedOutDevice, Transform>
    { };

    template <
      typename                  Char/*,
      template <typename> class Transform = DefaultOutTransform*/>
    auto make_fixed_out_stream (Char* begin, Char* end)
      -> OutStream <FixedOutStreamPolicy <Char/*, Transform*/>>
    {
      return OutStream <FixedOutStreamPolicy <Char/*, Transform*/>> (
        FixedOutDevice <Char> (begin, end)
      );
    }

    template <
      typename                  Char/*,
      template <typename> class Transform = DefaultOutTransform*/>
    auto make_fixed_out_stream (Char* begin, uptr capacity)
      -> OutStream <FixedOutStreamPolicy <Char/*, Transform*/>>
    {
      return make_fixed_out_stream (begin, begin + capacity);
    }

    typedef OutStream <FixedOutStreamPolicy <char>>
      FixedOutStream;

    typedef OutStream <FixedOutStreamPolicy <char16>>
      FixedOutStream16;

    typedef OutStream <FixedOutStreamPolicy <char16>>
      FixedOutStream32;

  }

}
