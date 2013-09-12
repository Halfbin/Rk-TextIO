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

#include <Rk/TextIO/FixedOutStream.hpp>

namespace TIO = Rk::TextIO;

namespace RkTest
{
  

  bool test_outstream ()
  {
    char buf [64];
    auto os = TIO::make_fixed_out_stream (buf, 64);



    return true;
  }

}
