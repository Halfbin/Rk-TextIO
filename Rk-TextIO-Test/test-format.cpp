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

#include <Rk/tio/array_out_stream.hpp>
#include <Rk/tio/format.hpp>

#include <string>

#include "test.hpp"

namespace RkTest
{
  template <typename... item_ts>
  std::string format_to_string (Rk::cstring_ref format, const item_ts&... items)
  {
    char buffer [256];
    auto stream = Rk::tio::make_array_out_stream (buffer, 255);

    Rk::tio::format (stream, format, items...);

    return std::string (stream.sink ().begin (), stream.sink ().pointer ());
  }

  bool test_format () try
  {
    bool ok = true;

    TEST (format_to_string ("Hello %1 %2 %4 %3 %% %9 %m %", "A", 'B', 3, 4) == "Hello A B 4 3 % \xef\xbf\xbd \xef\xbf\xbd ");

    return ok;
  }
  catch (...)
  {
    return false;
  }

}
