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

#include <Rk/tio/format.hpp>

#include <string>

#include "test.hpp"

namespace RkTest
{
  bool test_format () try
  {
    bool ok = true;

    TEST (Rk::tio::format ("Hello %1 %2 %4 %3 %% %9 %m %", "A", 'B', 3, 4) == "Hello A B 4 3 % \xef\xbf\xbd \xef\xbf\xbd ");

    return ok;
  }
  catch (...)
  {
    return false;
  }

}
