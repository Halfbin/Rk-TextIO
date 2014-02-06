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
#include <Rk/tio/default_speller.hpp>
#include <Rk/raw_io.hpp>

#include <vector>

namespace Rk
{
  namespace tio
  {
    template <typename next_sink, typename char_t>
    class raw_sink_base :
      public sink <char_t>
    {
      next_sink   next;
      std::vector
      <char_t>    buf;
      char_t*     ptr;

    public:
      raw_sink_base (next_sink new_next, size_t size = 8192) :
        next (std::forward <next_sink> (new_next)),
        buf  (size),
        ptr  (buf.data ())
      { }
      
      char_t* acquire (size_t request)
      {
        if (buf.data () + buf.size () - ptr < request)
          flush ();

        if (buf.size () < request)
        {
          buf.resize (request);
          ptr = buf.data ();
        }

        return ptr;
      }

      void release (size_t commit)
      {
        ptr += commit;
      }

      void flush ()
      {
        write (next, buf.data (), buf.size () * sizeof (char_t));
        ptr = buf.data ();
      }

      size_t max () const
      {
        return buf.max_size ();
      }

    };

    template <typename next_sink>
    struct raw_sink_binder
    {
      template <typename char_t>
      using type = raw_sink_base <next_sink, char_t>;
    };

    template <typename next_sink>
    using raw_sink = typename raw_sink_binder <next_sink>::type;

    template <
      typename                  char_t,
      typename                  next_sink,
      template <typename> class speller = default_speller>
    using raw_out_stream_policy = out_stream_policy <char_t, raw_sink <next_sink>, speller>;

    template <typename char_t, typename next_sink, typename... args>
    auto make_raw_out_stream (next_sink next, args&&... args)
      -> out_stream <raw_out_stream_policy <char_t, next_sink>>
    {
      return { raw_sink_base <next_sink, char_t> (next, std::forward <args> (args)...) };
    }

  } // tio

} // Rk
