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

#include <Rk/tio/string_out_stream.hpp>

#include <algorithm>
#include <iostream>
#include <array>

namespace Rk
{
  namespace tio
  {
    namespace format_private
    {
      template <typename stream_t>
      class format_item
      {
        typedef stream_t stream_t;

        const void* const ptr;
        void (format_item::*invoke) (stream_t&) const;

        template <typename value_t>
        void invoke_impl (stream_t& stream) const
        {
          stream << *static_cast <const value_t*> (ptr);
        }

      public:
        template <typename value_t>
        format_item (const value_t& value) :
          ptr    (&value),
          invoke (&format_item::template invoke_impl <value_t>)
        { }

        void put (stream_t& stream) const
        {
          (this ->* invoke) (stream);
        }

      };

      template <typename stream_t>
      void format_impl (
        stream_t&                     stream,
        cstring_ref                   pattern,
        const format_item <stream_t>* items,
        uint                          count)
      {
        using std::begin;
        using std::end;
        
        for (;;)
        {
          // Find % specifier and print intervening text
          auto percent = std::find (begin (pattern), end (pattern), '%');
          stream.write (begin (pattern), percent);

          // No percents or orphan
          if (percent + 1 >= end (pattern))
            return;

          // Grab specifier
          char spec = *++percent;

          pattern = { ++percent, end (pattern) };

          // Escaped percent
          if (spec == '%')
          {
            stream << "%";
            continue;
          }

          if (spec < '0' || spec > '9')
          {
            stream << "\xef\xbf\xbd";
            continue;
          }

          uint index = spec - '0' - 1;
          if (index >= count)
          {
            stream << "\xef\xbf\xbd";
            continue;
          }

          items [index].put (stream);
        }
      }

    }

    template <typename stream_t, typename... item_ts>
    auto format (stream_t& stream, cstring_ref pattern, const item_ts&... items)
      -> typename std::enable_if <is_out_stream <stream_t>::value>::type
    {
      using namespace format_private;

      format_item <stream_t> proxies [] =
      {
        items...
      };

      format_impl (stream, pattern, proxies, sizeof... (items));
    }

    template <typename... item_ts>
    std::string format (cstring_ref pattern, const item_ts&... items)
    {
      cstring_out_stream stream;
      format (stream, pattern, items...);
      return stream.string ();
    }

  } // tio

} // Rk
