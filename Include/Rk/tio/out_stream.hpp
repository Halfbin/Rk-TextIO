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

#include <Rk/tio/sink.hpp>

#include <Rk/string_ref.hpp>
#include <Rk/memory.hpp>

#include <type_traits>
#include <utility>

namespace Rk
{
  namespace tio
  {
    template <typename unit_t, typename ortho_t, typename sink_t>
    struct out_stream_policy
    {
      typedef unit_t  unit_t;
      typedef ortho_t ortho_t;
      typedef sink_t  sink_t;
      
    };

    template <typename policy>
    class out_stream
    {
    public:
      typedef typename policy::unit_t  unit_t;
      typedef typename policy::ortho_t ortho_t;
      typedef typename policy::sink_t  sink_t;
      
    private:
      sink_t  sn;
      ortho_t orth;

    public:
      explicit out_stream (sink_t new_sn = sink_t (), ortho_t new_orth = ortho_t ()) :
        sn   (std::forward <sink_t>  (new_sn)),
        orth (std::forward <ortho_t> (new_orth))
      { }
      
      out_stream             (const out_stream&) = default;
      out_stream& operator = (const out_stream&) = default;

      out_stream (out_stream&& other) :
        sn   (std::forward <sink_t>  (other.sn)),
        orth (std::forward <ortho_t> (other.orth))
      { }
      
      out_stream& operator = (out_stream&& other)
      {
        sn = std::move (other.sn);
        orth = std::move (other.orth);
        return *this;
      }

      sink_t&        sink  ()       { return sn; }
      const sink_t&  sink  () const { return sn; }
      ortho_t&       ortho ()       { return orth; }
      const ortho_t& ortho () const { return orth; }

      template <typename... arg_ts>
      void write (arg_ts&&... args)
      {
        tio::write (sn, std::forward <arg_ts> (args)...);
      }

      // Strings
      template <typename arg_t>
      auto operator << (arg_t&& value)
        -> typename std::enable_if <
            std::is_convertible <arg_t, string_ref_base <unit_t>>::value,
            out_stream&
          >::type
      {
        sn.write (string_ref_base <unit_t> (std::forward <arg_t> (value)));
        return *this;
      }

      // Code units
      template <typename arg_t>
      auto operator << (arg_t value)
        -> typename std::enable_if <
            std::is_same <arg_t, unit_t>::value,
            out_stream&
          >::type
      {
        write (&value, 1);
        return *this;
      }

      // Ints
      template <typename arg_t>
      auto operator << (arg_t value)
        -> typename std::enable_if <
            std::is_integral <arg_t>        ::value &&
            !std::is_same    <arg_t, unit_t>::value &&
            !std::is_same    <arg_t, bool>  ::value,
            out_stream&
          >::type
      {
        orth.spell (sn, decompose (value));
        return *this;
      }

      // bool
      template <typename arg_t>
      auto operator << (arg_t value)
        -> typename std::enable_if <
            std::is_same <arg_t, bool>::value,
            out_stream&
          >::type
      {
        orth.spell (sn, value);
        return *this;
      }

      // Floats
      template <typename arg_t>
      auto operator << (arg_t value)
        -> typename std::enable_if <
            std::is_floating_point <arg_t>::value,
            out_stream&
          >::type
      {
        orth.spell (sn, double (value));
        return *this;
      }

    }; // out_stream

  } // tio

} // Rk
