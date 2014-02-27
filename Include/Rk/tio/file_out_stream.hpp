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

#include <Rk/file_stream.hpp>
#include <Rk/exception.hpp>

namespace Rk
{
  namespace tio
  {
    using file_open_mode = fio::open_mode;
    using file_seek_mode = fio::seek_mode;

    namespace detail
    {
      template <typename unit_t>
      class file_sink :
        public sink <unit_t>,
        public fio::out_stream
      {
      public:
        template <typename path_t>
        file_sink (path_t&& path, file_open_mode mode) :
          fio::out_stream (std::forward <path_t> (path), mode)
        { }

        file_sink             (const file_sink&) = delete;
        file_sink& operator = (const file_sink&) = delete;
        
        file_sink (file_sink&&) = default;
        
        void write (string_ref_base <unit_t> src)
        {
          fio::out_stream::write (src.data (), src.length () * sizeof (unit_t));
        }

        void put (unit_t unit)
        {
          fio::out_stream::write (&unit, sizeof (unit_t));
        }

        void flush ()
        {
          fio::out_stream::flush ();
        }

      };

    } // detail

    template <typename unit_t, typename ortho_t = default_ortho <unit_t>>
    class file_out_stream :
      public out_stream_base <
        unit_t,
        detail::file_sink <unit_t>,
        ortho_t
      >
    {
    public:
      template <typename path_t>
      file_out_stream (path_t&& path, file_open_mode mode = file_open_mode::modify) :
        stream_base_t (
          sink_t  (std::forward <path_t> (path), mode),
          ortho_t ())
      { }

      file_out_stream             (const file_out_stream&) = delete;
      file_out_stream& operator = (const file_out_stream&) = delete;

      file_out_stream             (file_out_stream&&) = default;
      file_out_stream& operator = (file_out_stream&&) = default;

      u64 seek (i64 offset, file_seek_mode mode = file_seek_mode::offset)
      {
        return sink ().seek (offset, mode);
      }

      u64 tell () const
      {
        return sink ().tell ();
      }

      u64 size () const
      {
        return sink ().size ();
      }

      bool eof () const
      {
        return sink ().eof ();
      }

      void* get_handle () const
      {
        return sink ().get_handle ();
      }

      void delete_on_close (bool value = true)
      {
        return sink ().delete_on_close (value);
      }

    };

    typedef file_out_stream <char>   cfile_out_stream;
  //typedef file_out_stream <wchar>  wfile_out_stream;
    typedef file_out_stream <char16> u16file_out_stream;
    typedef file_out_stream <char32> u32file_out_stream;

  } // tio

} // Rk
