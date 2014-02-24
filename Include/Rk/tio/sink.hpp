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

#include <Rk/string_ref.hpp>
#include <Rk/types.hpp>

namespace Rk
{
  namespace tio
  {
    template <typename unit_t>
    class sink
    {
    public:
      typedef unit_t unit_t;

      virtual void write (string_ref_base <unit_t> src) = 0;
      virtual void flush () = 0;

    };

    typedef sink <char>   csink;
    typedef sink <wchar>  wsink;
    typedef sink <char16> u16sink;
    typedef sink <char32> u32sink;

    template <typename unit_t>
    void write (sink <unit_t>& sn, const unit_t* src, size_t len)
    {
      if (len != 0) sn.write (make_string_ref (src, len));
    }

    template <typename unit_t>
    void write (sink <unit_t>& sn, const unit_t* src, const unit_t* end)
    {
      if (end > src) sn.write (make_string_ref (src, end - src));
    }

  }

}
