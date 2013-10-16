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

#include <Rk/tio/spell_float.hpp>
#include <Rk/string_ref.hpp>

#include "Test.hpp"

namespace RkTest
{
  bool test_spell_float_kind (double value, Rk::tio::float_kind canon)
  {
    auto spelling = Rk::tio::spell_float (value, '0');
    return spelling.kind () == canon;
  }

  bool test_spell_float (Rk::string_ref canon_digits, i32 canon_exp, double value)
  {
    auto spelling = Rk::tio::spell_float (value, '0');
    return Rk::string_ref (spelling.begin (), spelling.length ()) == canon_digits && spelling.exponent () == canon_exp;
  }

  bool test_spell_float () try
  {
    bool ok = true;

    TEST (test_spell_float ("1", -10,            0.0000000001));
    TEST (test_spell_float ("1",  -8,              0.00000001));
    TEST (test_spell_float ("1",  -6,                0.000001));
    TEST (test_spell_float ("1",  -4,                  0.0001));
    TEST (test_spell_float ("1",  -2,                    0.01));
    TEST (test_spell_float ("1",   0,                     1.0));
    TEST (test_spell_float ("1",   2,                   100.0));
    TEST (test_spell_float ("1",   4,                 10000.0));
    TEST (test_spell_float ("1",   6,               1000000.0));
    TEST (test_spell_float ("1",   8,             100000000.0));
    TEST (test_spell_float ("1",  10,           10000000000.0));
    TEST (test_spell_float ("1",  12,         1000000000000.0));
    TEST (test_spell_float ("1",  14,       100000000000000.0));
    TEST (test_spell_float ("1",  16,     10000000000000000.0));
    TEST (test_spell_float ("1",  18,   1000000000000000000.0));
    TEST (test_spell_float ("1",  20, 100000000000000000000.0));
    
    auto nan = std::numeric_limits <double>::quiet_NaN (),
         inf = std::numeric_limits <double>::infinity  ();

    TEST (test_spell_float_kind (0.0, Rk::tio::float_zero));
    TEST (test_spell_float_kind (nan, Rk::tio::float_nan));
    TEST (test_spell_float_kind (inf, Rk::tio::float_infinite));

    return ok;
  }
  catch (...)
  {
    return false;
  }

}
