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

#include <Rk/TextIO/SpellNatural.hpp>
#include <Rk/StringRef.hpp>

#include "Test.hpp"

namespace RkTest
{
  template <typename T>
  bool test_spell_natural (Rk::StringRef canonical, T value, uint radix)
  {
    char buffer [66];
    
    auto end = Rk::TextIO::
      spell_natural (value, radix, "0123456789abcdefghijklmnopqrstuvwxyz")
      .get_digits (buffer, 66);

    return Rk::StringRef (buffer, end) == canonical;
  }

  bool test_spell_natural () try
  {
    bool ok = true;

    TEST (test_spell_natural (                   "0",                    0, 10));
    TEST (test_spell_natural (                   "1",                    1, 10));
    TEST (test_spell_natural (                 "123",                  123, 10));
    TEST (test_spell_natural (          "1234567890",           1234567890, 10));
    TEST (test_spell_natural ("18446744073709551615", 18446744073709551615, 10));
    TEST (test_spell_natural (            "fedcba01",           0xfedcba01, 16));
    TEST (test_spell_natural (            "10101010",                  170,  2));
    TEST (test_spell_natural (                 "gc3",                21171, 36));

    TEST_EXCEPT (test_spell_natural ("",   -1, 10), std::domain_error);
    TEST_EXCEPT (test_spell_natural ("10", 90, 90), std::invalid_argument);

    return ok;
  }
  catch (...)
  {
    return false;
  }

}
