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

#include <Rk/tio/fixed_out_stream.hpp>
#include <Rk/tio/format.hpp>

#include <string>

#include "test.hpp"

namespace RkTest
{
  template <typename... item_ts>
  std::string format_to_string (Rk::string_ref format, const item_ts&... items)
  {
    char buffer [256];
    auto stream = Rk::tio::make_fixed_out_stream (buffer, 255);

    Rk::tio::format (stream, format, items...);

    return std::string (stream.sink ().begin (), stream.sink ().pointer ());
  }

  bool test_format () try
  {
    bool ok = true;

    TEST (Rk::string_ref ("Hello A B 4 3 % ") == format_to_string ("Hello %1 %2 %4 %3 %% %", "A", 'B', 3, 4));

    TEST_EXCEPT (format_to_string ("%9", 1, 2, 3), std::out_of_range);
    TEST_EXCEPT (format_to_string ("%H", 1, 2, 3), std::invalid_argument);

    return ok;
  }
  catch (...)
  {
    return false;
  }

}
