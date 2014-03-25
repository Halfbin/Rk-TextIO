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

#include <Rk/unicode_convert.hpp>
#include <Rk/exception.hpp>

namespace Rk
{
  namespace tio
  {
    namespace detail
    {
      extern "C" 
      {
        #define fromdll(t) __declspec(dllimport) t __stdcall
        fromdll (i32)           WriteConsoleW    (void* handle, const void* src, u32 len, u32* written, void*);
        fromdll (void*)         GetStdHandle     (i32 id);
        fromdll (i32)           GetConsoleMode   (void*, u32*);
        fromdll (i32)           WriteFile        (void*, const void*, u32, u32*, void*);
        fromdll (i32)           FlushFileBuffers (void*);
        #undef fromdll
      }

      enum : i32
      {
        std_output_handle = -11,
        std_error_handle  = -12
      };

      template <typename unit_t>
      class std_sink;

      template <>
      class std_sink <char> :
        public sink <char>
      {
        void*          handle;
        bool           convert;
        std::u16string buffer;

        bool is_console () const
        {
          u32 mode;
          return GetConsoleMode (handle, &mode) != 0;
        }

        void flush_conv_buffer ()
        {
          if (buffer.empty ())
            return;
          u32 written = 0;
          auto ok = WriteConsoleW (handle, buffer.data (), buffer.length (), &written, nullptr);
          if (!ok)
            throw win_error ("WriteConsoleW to standard output failed");
          buffer.clear ();
        }

        void write_console (cstring_ref src)
        {
          utf8_to_16 conv (src.begin (), src.end ());
          
          for (;;)
          {
            switch (conv.decode ())
            {
              case got_codepoint:
                buffer.append (conv.units (), conv.unit_count ());
              continue;

              case idle:
              default:;
                // some kind of failure indication? hm.
            }

            break;
          }

          flush_conv_buffer ();
        }

        void write_direct (cstring_ref src)
        {
          u32 written = 0;
          auto ok = WriteFile (handle, src.data (), src.length (), &written, nullptr);
          if (!ok)
            throw win_error ("WriteFile to standard output failed");
        }

      public:
        std_sink (i32 id) :
          handle  (GetStdHandle (id)),
          convert (is_console ())
        { }

        void write (cstring_ref src)
        {
          if (src.empty ())
            return;

          if (convert)
            write_console (src);
          else
            write_direct (src);
        }

        void put (char unit)
        {
          write (cstring_ref (&unit, 1));
        }

        void flush ()
        {
          if (convert)
            flush_conv_buffer ();
          FlushFileBuffers (handle);
        }

      };

    } // detail

    template <typename unit_t, typename ortho_t = default_ortho <unit_t>>
    class std_out_stream;

    template <typename ortho_t>
    class std_out_stream <char, ortho_t> :
      public out_stream_base <
        char,
        detail::std_sink <char>,
        ortho_t
      >
    {
    public:
      std_out_stream () :
        stream_base_t (
          sink_t  (-11),
          ortho_t ())
      { }

    };

    typedef std_out_stream <char>   cstd_out_stream;
  //typedef std_out_stream <wchar>  wstd_out_stream;
  //typedef std_out_stream <char16> u16std_out_stream;
  //typedef std_out_stream <char32> u32std_out_stream;

  } // tio

} // Rk
