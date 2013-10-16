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

#include <Rk/tio/forward.hpp>
#include <Rk/tio/sink.hpp>

#include <Rk/string_ref.hpp>
#include <Rk/memory.hpp>

#include <type_traits>
#include <utility>

namespace Rk
{
  namespace tio
  {
    template <
      typename                  char_t,
      template <typename> class sink,
      template <typename> class speller>
    struct out_stream_policy
    {
      typedef char_t           char_t;
      typedef sink <char_t>    sink_t;
      typedef speller <sink_t> speller_t;

    };

    template <typename policy>
    class out_stream
    {
    public:
      typedef typename policy::char_t    char_t;
      typedef typename policy::sink_t    sink_t;
      typedef typename policy::speller_t speller_t;
      
    private:
      sink_t    sn;
      speller_t sp;

    public:
      explicit out_stream (sink_t new_sn = sink_t (), speller_t new_sp = speller_t ()) :
        sn (std::forward <sink_t>    (new_sn)),
        sp (std::forward <speller_t> (new_sp))
      { }
      
      out_stream             (const out_stream&) = default;
      out_stream& operator = (const out_stream&) = default;

      out_stream (out_stream&& other) :
        sn (std::forward <sink_t>    (other.sn)),
        sp (std::forward <speller_t> (other.sp))
      { }
      
      out_stream& operator = (out_stream&& other)
      {
        sn = std::move (other.sn);
        sp = std::move (other.sp);
        return *this;
      }

            sink_t&    sink  ()       { return sn; }
      const sink_t&    sink  () const { return sn; }
            speller_t& spell ()       { return sp; }
      const speller_t& spell () const { return sp; }
      
      template <typename... arg_ts>
      void write (arg_ts&&... args)
      {
        tio::write (sn, std::forward <arg_ts> (args)...);
      }

      // Strings
      template <typename T>
      auto operator << (T&& value)
        -> std::enable_if_t <
            std::is_convertible <T, string_ref_base <char_t>>::value,
            out_stream&
          >
      {
        sp.apply (sn, string_ref_base <char_t> (std::forward <T> (value)));
        return *this;
      }

      // Chars
      template <typename T>
      auto operator << (T value)
        -> std::enable_if_t <
            std::is_same <T, char_t>::value,
            out_stream&
          >
      {
        sp.apply (sn, value);
        return *this;
      }

      // Ints
      template <typename T>
      auto operator << (T value)
        -> std::enable_if_t <
            std::is_integral <T>        ::value &&
            !std::is_same    <T, char_t>::value &&
            !std::is_same    <T, bool>  ::value,
            out_stream&
          >
      {
        sp.apply (sn, decompose (value));
        return *this;
      }

      // bool
      template <typename T>
      auto operator << (T value)
        -> typename std::enable_if <
            std::is_same <T, bool>::value,
            out_stream&
          >::type
      {
        sp.apply (sn, value);
        return *this;
      }

      // Floats
      template <typename T>
      auto operator << (T value)
        -> typename std::enable_if <
            std::is_floating_point <T>::value,
            out_stream&
          >::type
      {
        sp.apply (sn, double (value));
        return *this;
      }

    }; // out_stream

  } // tio

} // Rk
