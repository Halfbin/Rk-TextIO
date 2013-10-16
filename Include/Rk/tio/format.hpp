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

#include <Rk/tio/out_stream.hpp>

#include <algorithm>
#include <iostream>
#include <array>

namespace Rk
{
  namespace tio
  {
    namespace format_private
    {
      template <typename policy>
      class format_item
      {
        typedef out_stream <policy> stream_t;

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

        void format (stream_t& stream) const
        {
          (this ->* invoke) (stream);
        }

        format_item             (const format_item&) = delete;
        format_item& operator = (const format_item&) = delete;

        format_item (format_item&& other) :
          raw    (other.raw)
          invoke (other.invoke)
        {
          other.raw    = nullptr;
          other.invoke = nullptr;
        }

        format_item& operator = (format_item&& rhs)
        {
          std::swap (raw    = nullptr, other.raw);
          std::swap (invoke = nullptr, other.invoke);
          return *this;
        }

      };

      template <typename policy, uint in_count>
      void format_impl (
        out_stream <policy>&                         stream,
        string_ref_base <typename policy::char_t>    format,
        std::array <format_item <policy>, in_count>& items)
      {
        using std::begin;
        using std::end;
        
        auto percent = std::find (begin (format), end (format), '%');

        stream.write (begin (format), percent);

        // No percents or orphan; output rest of the format string
        if (percent == end (format) || percent + 1 == end (format))
          return;

        // Percent sign
        char spec = *++percent;

        // Escaped percent
        if (spec == '%')
        {
          stream << '%';
          return format_impl (stream, { ++percent, end (format) }, items);
        }
        else if (spec < '0' || spec > '9')
        {
          throw std::invalid_argument ("Invalid format specifier");
        }

        // Format index
        uint index = spec - '0' - 1;

        if (index >= in_count)
          throw std::out_of_range ("Format specifier out-of-range");

        items [index].format (stream);

        format_impl (stream, { ++percent, end (format) }, items);
      }

    }

    template <typename policy, typename... item_ts>
    void format (
      out_stream <policy>&                      stream,
      string_ref_base <typename policy::char_t> format,
      const item_ts&...                         items)
    {
      using namespace format_private;

      std::array <format_item <policy>, sizeof... (items)> boxed {
        items...
      };

      format_impl (stream, format, boxed);
    }

  } // tio

} // Rk
