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
#include <Rk/tio/sink.hpp>
#include <Rk/tio/out_stream.hpp>

// Uses
#include <Rk/tio/default_speller.hpp>

namespace Rk
{
  namespace tio
  {
    template <typename char_t>
    class fixed_sink :
      public sink <char_t>
    {
      char_t* beg,
            * ptr,
            * ed;

    public:
      fixed_sink (char_t* new_ptr, char_t* new_end) :
        beg (new_ptr),
        ptr (new_ptr),
        ed  (new_end)
      {
        if (ptr >= ed)
          throw std::invalid_argument ("Invalid range");
      }

      fixed_sink (char_t* new_ptr, size_t len) :
        fixed_sink (new_ptr, new_ptr + len)
      { }

      char_t* acquire (size_t request)
      {
        if (request > max ())
          throw std::length_error ("Out of fixed buffer space");
        return ptr;
      }

      void release (size_t commit)
      {
        ptr += commit;
      }

      void flush ()
      { }

      size_t max () const
      {
        return size_t (ed - ptr);
      }

      const char_t* begin () const
      {
        return beg;
      }

      const char_t* pointer () const
      {
        return ptr;
      }

      const char_t* end () const
      {
        return ed;
      }

    };

    template <
      typename                  char_t,
      template <typename> class spell = default_speller>
    using fixed_out_stream_policy = out_stream_policy <char_t, fixed_sink, spell>;

    template <typename char_t, typename Limit>
    auto make_fixed_out_stream (char_t* begin, Limit&& limit)
      -> out_stream <fixed_out_stream_policy <char_t>>
    {
      return out_stream <fixed_out_stream_policy <char_t>> {
        fixed_sink <char_t> (begin, std::forward <Limit> (limit))
      };
    }

    typedef out_stream <fixed_out_stream_policy <char>>
      fixed_out_stream;

    typedef out_stream <fixed_out_stream_policy <wchar>>
      fixed_out_stream_w;

    typedef out_stream <fixed_out_stream_policy <char16>>
      fixed_out_stream_u16;

    typedef out_stream <fixed_out_stream_policy <char16>>
      fixed_out_stream_u32;

  } // tio

} // Rk
