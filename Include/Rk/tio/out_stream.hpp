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

#include <Rk/tio/ortho.hpp>
#include <Rk/tio/sink.hpp>

#include <Rk/string_ref.hpp>
#include <Rk/memory.hpp>

#include <type_traits>
#include <utility>

namespace Rk
{
  namespace tio
  {
    namespace detail
    {
      // Base class for is_out_stream check
      struct out_stream_tag
      { };

      //
      // out_stream_facade
      // Provides public interface for output streams.
      // Dispatches statically to self_t::put_*.
      // base_t used to inherit from other output streams already deriving from facade <...> without introducing
      //  ambiguous method names.
      //
      template <typename self_t, typename unit_t, typename base_t>
      class out_stream_facade :
        public base_t
      {
        self_t&       self ()       { return static_cast <self_t&>       (*this); }
        const self_t& self () const { return static_cast <const self_t&> (*this); }

      protected:
        using facade_t = out_stream_facade;

      public:
        typedef unit_t                   unit_t;
        typedef string_ref_base <unit_t> string_t;

        template <typename... arg_ts>
        void write (arg_ts&&... args)
        {
          self ().put_string (string_t (std::forward <arg_ts> (args)...));
        }

        template <typename arg_t>
        auto operator << (arg_t&& arg)
          -> typename std::enable_if <std::is_convertible <arg_t, typename self_t::string_t>::value, self_t&>::type
        {
          self ().put_string (std::forward <arg_t> (arg));
          return self ();
        }

        template <typename arg_t>
        auto operator << (arg_t arg)
          -> typename std::enable_if <std::is_same <arg_t, typename self_t::unit_t>::value, self_t&>::type
        {
          self ().put_unit (arg);
          return self ();
        }

        template <typename arg_t>
        auto operator << (arg_t arg)
          -> typename std::enable_if <std::is_same <arg_t, bool>::value, self_t&>::type
        {
          self ().put_bool (arg);
          return self ();
        }

        self_t& operator << (soft_int_32 value)
        {
          self ().put_int (value);
          return self ();
        }

        self_t& operator << (soft_int_64 value)
        {
          self ().put_int (value);
          return self ();
        }

        template <typename arg_t>
        auto operator << (arg_t arg)
          -> typename std::enable_if <std::is_floating_point <arg_t>::value, self_t&>::type
        {
          self ().put_float (arg);
          return self ();
        }

      }; // out_stream_facade

    } // detail
    
    //
    // is_out_stream
    // value is true is stream_t is an output stream, and false otherwise.
    //
    template <typename stream_t>
    struct is_out_stream :
      std::is_base_of <detail::out_stream_tag, stream_t>
    { };

  /*template <typename stream_t>
    static const bool is_out_stream_p = is_out_stream <stream_t>::value;*/

    //
    // out_stream
    // Abstract base for output streams of unit_ts.
    //
    template <typename unit_t>
    class out_stream :
      public detail::out_stream_facade <out_stream <unit_t>, unit_t, detail::out_stream_tag>
    {
      friend class facade_t;

      virtual void put_string (string_t) = 0;
      virtual void put_unit   (unit_t) = 0;
      virtual void put_bool   (bool) = 0;
      virtual void put_int    (soft_int_32) = 0;
      virtual void put_int    (soft_int_64) = 0;
      virtual void put_float  (double) = 0;

    public:
      virtual void flush () = 0;

    };
    
    // Common types. wchar_t considered harmful.
    typedef out_stream <char>  cout_stream;
  //typedef out_stream <wchar> wout_stream;
    typedef out_stream <u16>   u16out_stream;
    typedef out_stream <u32>   u32out_stream;
    
    //
    // out_stream_policy
    // Encapsulates policy for out_stream_base:
    //  unit_t  - code unit type
    //  ortho_t - orthographic component type
    //  sink_t  - text sink device type
    //
    template <typename unit_t, typename ortho_t, typename sink_t>
    struct out_stream_policy
    {
      typedef unit_t  unit_t;
      typedef ortho_t ortho_t;
      typedef sink_t  sink_t;
      
    };

    //
    // out_stream_base
    // An output stream. Derives from out_stream.
    // Contains a policy::ortho_t, used for spelling, and a policy::sink_t, for data output.
    //
    template <typename policy>
    class out_stream_base :
      public detail::out_stream_facade <
        out_stream_base <policy>,
        typename policy::unit_t,
        out_stream <typename policy::unit_t>>
    {
    public:
      using ortho_t = typename policy::ortho_t;
      using sink_t  = typename policy::sink_t;
      
    private:
      typedef out_stream_base self_t;

      sink_t  sn;
      ortho_t orth;

      friend class facade_t;

      void put_string (string_t str)
      {
        sn.write (str);
      }

      void put_unit (unit_t unit)
      {
        sn.put (unit);
      }

      void put_int (soft_int_32 value)
      {
        orth.spell (sn, value);
      }

      void put_int (soft_int_64 value)
      {
        orth.spell (sn, value);
      }

      void put_bool (bool value)
      {
        orth.spell (sn, value);
      }

      void put_float (double value)
      {
        orth.spell (sn, value);
      }

    public:
      explicit out_stream_base (sink_t new_sn = sink_t (), ortho_t new_orth = ortho_t ()) :
        sn   (std::forward <sink_t>  (new_sn)),
        orth (std::forward <ortho_t> (new_orth))
      { }
      
      out_stream_base             (const out_stream_base&) = default;
      out_stream_base& operator = (const out_stream_base&) = default;

      out_stream_base (out_stream_base&& other) :
        sn   (std::forward <sink_t>  (other.sn)),
        orth (std::forward <ortho_t> (other.orth))
      { }
      
      out_stream_base& operator = (out_stream_base&& other)
      {
        sn = std::move (other.sn);
        orth = std::move (other.orth);
        return *this;
      }

      sink_t&        sink  ()       { return sn; }
      const sink_t&  sink  () const { return sn; }
      ortho_t&       ortho ()       { return orth; }
      const ortho_t& ortho () const { return orth; }
      
      void flush ()
      {
        sn.flush ();
      }

    }; // out_stream_base

  } // tio

} // Rk
