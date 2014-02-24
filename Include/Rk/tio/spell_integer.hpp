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

#include <Rk/tio/ortho.hpp>
#include <Rk/tio/sink.hpp>

#include <Rk/string_ref.hpp>

#ifndef RK_TEXTIO_API
#define RK_TEXTIO_API __declspec(dllimport)
#endif

namespace Rk
{
  namespace tio
  {
    RK_TEXTIO_API void spell_integer (csink&, soft_int_32, const cnumeric_ortho&, const numeric_ortho_options&);
    RK_TEXTIO_API void spell_integer (csink&, soft_int_64, const cnumeric_ortho&, const numeric_ortho_options&);

  }

}
