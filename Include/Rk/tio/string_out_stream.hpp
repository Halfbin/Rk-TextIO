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
#include <Rk/tio/out_stream.hpp>
#include <Rk/tio/sink.hpp>

// Uses
#include <Rk/tio/default_ortho.hpp>

#include <Rk/exception.hpp>

namespace Rk
{
  namespace tio
  {
    namespace detail
    {
      template <typename unit_t, typename alloc_t>
      class string_sink :
        public sink <unit_t>
      {
      public:
        using string_t = std::basic_string <unit_t, std::char_traits <unit_t>, alloc_t>;

      private:
        string_t store;

      public:
        string_sink () = default;

        void write (string_ref_base <unit_t> src)
        {
          store.append (src.data (), src.length ());
        }

        void put (unit_t unit)
        {
          store.push_back (unit);
        }

        void flush ()
        { }

        string_t& string ()
        {
          return store;
        }

      };

    } // detail

    template <
      typename unit_t,
      typename alloc_t = std::allocator <unit_t>,
      typename ortho_t = default_ortho <unit_t>>
    class string_out_stream :
      public out_stream_base <
        unit_t,
        detail::string_sink <unit_t, alloc_t>,
        ortho_t
      >
    {
    public:
      string_out_stream () :
        stream_base_t (
          sink_t  (),
          ortho_t ())
      { }

      typename sink_t::string_t& string ()
      {
        return sink ().string ();
      }

    };

    typedef string_out_stream <char>   cstring_out_stream;
  //typedef string_out_stream <wchar>  wstring_out_stream;
    typedef string_out_stream <char16> u16string_out_stream;
    typedef string_out_stream <char32> u32string_out_stream;

  } // tio

} // Rk
