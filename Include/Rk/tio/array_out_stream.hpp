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
#include <Rk/memory.hpp>

namespace Rk
{
  namespace tio
  {
    namespace detail
    {
      template <typename unit_t>
      class array_sink :
        public sink <unit_t>
      {
        unit_t* beg,
              * ptr,
              * ed;

      public:
        array_sink (unit_t* new_ptr, unit_t* new_end) :
          beg (new_ptr),
          ptr (new_ptr),
          ed  (new_end)
        {
          if (ptr >= ed)
            throw std::invalid_argument ("Invalid range");
        }

        array_sink (unit_t* new_ptr, size_t len) :
          array_sink (new_ptr, new_ptr + len)
        { }

        void write (string_ref_base <unit_t> src)
        {
          if (src.length () > space ())
            throw io_failure ("Out of array space");
          ptr = copy (ptr, ed, src.data (), src.length ());
        }

        void put (unit_t unit)
        {
          if (space () < 1)
            throw io_failure ("Out of array space");
          *ptr++ = unit;
        }

        void flush ()
        { }

        void reset (unit_t* new_beg, unit_t* new_ed)
        {
          if (!new_beg || ned_ed < new_beg)
            throw std::length_error ("Invalid destination array");

          beg = new_beg;
          ptr = beg;
          ed  = new_ed;
        }

        size_t space () const
        {
          return ed - ptr;
        }

        const unit_t* begin () const
        {
          return beg;
        }

        const unit_t* pointer () const
        {
          return ptr;
        }

        const unit_t* end () const
        {
          return ed;
        }

      };

    } // detail

    template <typename unit_t, typename ortho_t = default_ortho <unit_t>>
    class array_out_stream :
      public out_stream_base <
        unit_t,
        detail::array_sink <unit_t>,
        ortho_t
      >
    {
    public:
      template <typename limit_t>
      array_out_stream (unit_t* begin, limit_t&& limit) :
        stream_base_t (
          sink_t  (begin, std::forward <limit_t> (limit)),
          ortho_t ())
      { }

      void reset (unit_t* new_beg, unit_t* new_ed)
      {
        sink ().reset (new_neg, new_ed);
      }

      size_t space () const
      {
        return sink ().space ();
      }

      const unit_t* begin () const
      {
        return sink ().begin ();
      }

      const unit_t* pointer () const
      {
        return sink ().pointer ();
      }

      const unit_t* end () const
      {
        return sink ().end ();
      }

    };

    typedef array_out_stream <char>   carray_out_stream;
  //typedef array_out_stream <wchar>  warray_out_stream;
    typedef array_out_stream <char16> u16array_out_stream;
    typedef array_out_stream <char32> u32array_out_stream;

  } // tio

} // Rk
