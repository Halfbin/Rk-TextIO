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
#include <Rk/tio/ortho.hpp>

// Uses
#include <Rk/tio/spell_integer.hpp>
#include <Rk/tio/spell_float.hpp>
#include <Rk/tio/sink.hpp>

#include <Rk/memory.hpp>

#ifndef RK_TEXTIO_API
#define RK_TEXTIO_API __declspec(dllimport)
#endif

namespace Rk
{
  namespace tio
  {
    extern RK_TEXTIO_API const cnumeric_ortho        default_numeric_ortho;
    extern RK_TEXTIO_API const cboolean_ortho        default_boolean_ortho;
    extern RK_TEXTIO_API const numeric_ortho_options default_numeric_ortho_options;

    template <typename unit_t>
    class default_ortho;

    template <>
    class default_ortho <char>
    {
    public:
      // Params
      cnumeric_ortho numeric;
      cboolean_ortho boolean;

      // State
      numeric_ortho_options numeric_opts;
      bool digital_bool;
      
      RK_TEXTIO_API default_ortho ();

      cstring_ref spell_true () const
      {
        return digital_bool ? numeric.digits [1] : boolean.verum;
      }

      cstring_ref spell_false () const
      {
        return digital_bool ? numeric.digits [0] : boolean.falsum;
      }

      void spell (csink& sn, soft_int_32 value)
      {
        spell_integer (sn, value, numeric, numeric_opts);
      }

      void spell (csink& sn, soft_int_64 value)
      {
        spell_integer (sn, value, numeric, numeric_opts);
      }

      void spell (csink& sn, bool value)
      {
        sn.write (value ? spell_true () : spell_false ());
      }

      void spell (csink& sn, double value)
      {
        spell_float (sn, value, numeric, numeric_opts);
      }

    };

    typedef default_ortho <char> cdefault_ortho;

  }

}
