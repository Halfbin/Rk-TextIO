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
#include <Rk/tio/default_ortho.hpp>

namespace Rk
{
  namespace tio
  {
    static const cstring_ref defdigs [36] = {
      "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
      "a", "b", "c", "d", "e", "f", "g", "h", "i", "j",
      "k", "l", "m", "n", "o", "p", "q", "r", "s", "t",
      "u", "v", "w", "x", "y", "z"
    };

    static const cdigit_map default_digits = {
      defdigs,
      36
    };

    const cnumeric_ortho default_numeric_ortho = {
      default_digits,
      "+",
      "-",//"\xe2\x88\x92", // U+2212 MINUS SIGN
      " ",//"\xe2\x80\x89", // U+2009 THIN SPACE
      ".",
      "inf",//"\xe2\x88\x9e", // U+221E INFINITY
      "nan",
      "e",
      3, 3 // group lengths
    };

    const cboolean_ortho default_boolean_ortho = {
      "true",
      "false"
    };

    const numeric_ortho_options default_numeric_ortho_options = {
      10,    // radix
      false, // group
      false  // show_plus
    };

    default_ortho <char>::default_ortho () :
      numeric      (default_numeric_ortho),
      boolean      (default_boolean_ortho),
      numeric_opts (default_numeric_ortho_options),
      digital_bool (false)
    { }

  }

}
