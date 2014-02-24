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

#include "test.hpp"

namespace RkTest
{
/*bool test_spell_natural ();
  bool test_spell_float   ();
  bool test_outstream     ();*/
  bool test_format        ();
  bool test_utf8_encoding ();
  bool test_utf8_decoding ();

  bool test_textio ()
  {
    bool ok = true;

  /*TEST (test_spell_natural ());
    TEST (test_spell_float ());
    TEST (test_outstream ());*/
    TEST (test_format ());
    TEST (test_utf8_encoding ());
    TEST (test_utf8_decoding ());
    
    return ok;
  }

  extern "C" int main () try
  {
    bool ok = true;
    TEST (test_textio ());
    return !ok;
  }
  catch (...)
  {
    return 1;
  }

  int depth = 0,
      prev  = 0;

}
