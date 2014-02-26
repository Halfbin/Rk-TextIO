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

// Implements
#include <Rk/tio/float_expander.hpp>
#include <Rk/tio/spell_float.hpp>

// Uses
#include <Rk/tio/spell_integer.hpp>

#include "soft_float.hpp"

#include <stdexcept>

namespace Rk
{
  namespace tio
  {
    struct scale_factor
    {
      soft_float value;
      i32        decimal_exponent;

      scale_factor () { }

      scale_factor (soft_float v, i32 de) :
        value (v), decimal_exponent (de)
      { }
      
    };

    const scale_factor scale_factors [] = {
      scale_factor (soft_float (0x9a6bb0aa55653b2dull, -1113), -316),
      scale_factor (soft_float (0xe61acf033d1a45dfull, -1087), -308),
      scale_factor (soft_float (0xab70fe17c79ac6caull, -1060), -300),
      scale_factor (soft_float (0xff77b1fcbebcdc4full, -1034), -292),
      scale_factor (soft_float (0xbe5691ef416bd60cull, -1007), -284),
      scale_factor (soft_float (0x8dd01fad907ffc3cull,  -980), -276),
      scale_factor (soft_float (0xd3515c2831559a83ull,  -954), -268),
      scale_factor (soft_float (0x9d71ac8fada6c9b5ull,  -927), -260),
      scale_factor (soft_float (0xea9c227723ee8bcbull,  -901), -252),
      scale_factor (soft_float (0xaecc49914078536dull,  -874), -244),
      scale_factor (soft_float (0x823c12795db6ce57ull,  -847), -236),
      scale_factor (soft_float (0xc21094364dfb5637ull,  -821), -228),
      scale_factor (soft_float (0x9096ea6f3848984full,  -794), -220),
      scale_factor (soft_float (0xd77485cb25823ac7ull,  -768), -212),
      scale_factor (soft_float (0xa086cfcd97bf97f4ull,  -741), -204),
      scale_factor (soft_float (0xef340a98172aace5ull,  -715), -196),
      scale_factor (soft_float (0xb23867fb2a35b28eull,  -688), -188),
      scale_factor (soft_float (0x84c8d4dfd2c63f3bull,  -661), -180),
      scale_factor (soft_float (0xc5dd44271ad3cdbaull,  -635), -172),
      scale_factor (soft_float (0x936b9fcebb25c996ull,  -608), -164),
      scale_factor (soft_float (0xdbac6c247d62a584ull,  -582), -156),
      scale_factor (soft_float (0xa3ab66580d5fdaf6ull,  -555), -148),
      scale_factor (soft_float (0xf3e2f893dec3f126ull,  -529), -140),
      scale_factor (soft_float (0xb5b5ada8aaff80b8ull,  -502), -132),
      scale_factor (soft_float (0x87625f056c7c4a8bull,  -475), -124),
      scale_factor (soft_float (0xc9bcff6034c13053ull,  -449), -116),
      scale_factor (soft_float (0x964e858c91ba2655ull,  -422), -108),
      scale_factor (soft_float (0xdff9772470297ebdull,  -396), -100),
      scale_factor (soft_float (0xa6dfbd9fb8e5b88full,  -369),  -92),
      scale_factor (soft_float (0xf8a95fcf88747d94ull,  -343),  -84),
      scale_factor (soft_float (0xb94470938fa89bcfull,  -316),  -76),
      scale_factor (soft_float (0x8a08f0f8bf0f156bull,  -289),  -68),
      scale_factor (soft_float (0xcdb02555653131b6ull,  -263),  -60),
      scale_factor (soft_float (0x993fe2c6d07b7facull,  -236),  -52),
      scale_factor (soft_float (0xe45c10c42a2b3b06ull,  -210),  -44),
      scale_factor (soft_float (0xaa242499697392d3ull,  -183),  -36),
      scale_factor (soft_float (0xfd87b5f28300ca0eull,  -157),  -28),
      scale_factor (soft_float (0xbce5086492111aebull,  -130),  -20),
      scale_factor (soft_float (0x8cbccc096f5088ccull,  -103),  -12),
      scale_factor (soft_float (0xd1b71758e219652cull,   -77),   -4),
      scale_factor (soft_float (0x9c40000000000000ull,   -50),    4),
      scale_factor (soft_float (0xe8d4a51000000000ull,   -24),   12),
      scale_factor (soft_float (0xad78ebc5ac620000ull,     3),   20),
      scale_factor (soft_float (0x813f3978f8940984ull,    30),   28),
      scale_factor (soft_float (0xc097ce7bc90715b3ull,    56),   36),
      scale_factor (soft_float (0x8f7e32ce7bea5c70ull,    83),   44),
      scale_factor (soft_float (0xd5d238a4abe98068ull,   109),   52),
      scale_factor (soft_float (0x9f4f2726179a2245ull,   136),   60),
      scale_factor (soft_float (0xed63a231d4c4fb27ull,   162),   68),
      scale_factor (soft_float (0xb0de65388cc8ada8ull,   189),   76),
      scale_factor (soft_float (0x83c7088e1aab65dbull,   216),   84),
      scale_factor (soft_float (0xc45d1df942711d9aull,   242),   92),
      scale_factor (soft_float (0x924d692ca61be758ull,   269),  100),
      scale_factor (soft_float (0xda01ee641a708deaull,   295),  108),
      scale_factor (soft_float (0xa26da3999aef774aull,   322),  116),
      scale_factor (soft_float (0xf209787bb47d6b85ull,   348),  124),
      scale_factor (soft_float (0xb454e4a179dd1877ull,   375),  132),
      scale_factor (soft_float (0x865b86925b9bc5c2ull,   402),  140),
      scale_factor (soft_float (0xc83553c5c8965d3dull,   428),  148),
      scale_factor (soft_float (0x952ab45cfa97a0b3ull,   455),  156),
      scale_factor (soft_float (0xde469fbd99a05fe3ull,   481),  164),
      scale_factor (soft_float (0xa59bc234db398c25ull,   508),  172),
      scale_factor (soft_float (0xf6c69a72a3989f5cull,   534),  180),
      scale_factor (soft_float (0xb7dcbf5354e9beceull,   561),  188),
      scale_factor (soft_float (0x88fcf317f22241e2ull,   588),  196),
      scale_factor (soft_float (0xcc20ce9bd35c78a5ull,   614),  204),
      scale_factor (soft_float (0x98165af37b2153dfull,   641),  212),
      scale_factor (soft_float (0xe2a0b5dc971f303aull,   667),  220),
      scale_factor (soft_float (0xa8d9d1535ce3b396ull,   694),  228),
      scale_factor (soft_float (0xfb9b7cd9a4a7443cull,   720),  236),
      scale_factor (soft_float (0xbb764c4ca7a44410ull,   747),  244),
      scale_factor (soft_float (0x8bab8eefb6409c1aull,   774),  252),
      scale_factor (soft_float (0xd01fef10a657842cull,   800),  260),
      scale_factor (soft_float (0x9b10a4e5e9913129ull,   827),  268),
      scale_factor (soft_float (0xe7109bfba19c0c9dull,   853),  276),
      scale_factor (soft_float (0xac2820d9623bf429ull,   880),  284),
      scale_factor (soft_float (0x80444b5e7aa7cf85ull,   907),  292),
      scale_factor (soft_float (0xbf21e44003acdd2dull,   933),  300),
      scale_factor (soft_float (0x8e679c2f5e44ff8full,   960),  308),
      scale_factor (soft_float (0xd433179d9c8cb841ull,   986),  316),
      scale_factor (soft_float (0x9e19db92b4e31ba9ull,  1013),  324),
      scale_factor (soft_float (0xeb96bf6ebadf77d9ull,  1039),  332)
    };

    const i32 target_exponent_min = -59,
              target_exponent_max = -32,
              decimal_exponent_min = scale_factors [0].decimal_exponent,
              decimal_exponent_stride = scale_factors [1].decimal_exponent - decimal_exponent_min;

    soft_float find_scale_factor (i32 exponent, i32& decimal_exponent)
    {
      const auto recip_log2_10 = 0.30102999566398114;
      auto k = (i32) std::ceil ((target_exponent_min - exponent - 1) * recip_log2_10);
      auto index = (k - decimal_exponent_min - 1) / decimal_exponent_stride + 1;
      decimal_exponent = -scale_factors [index].decimal_exponent;
      return scale_factors [index].value;
    }

    void compute_parameters (u64 significand, i32 exponent, soft_float& upper_bound, u64& margin, i32& decimal_exponent)
    {
      soft_float
        value (significand << 1, exponent - 1),
        lower_bound (value.significand - 1, value.exponent);

      upper_bound = soft_float (value.significand + 1, value.exponent);
      upper_bound.normalize ();

      lower_bound.significand <<= value.exponent - upper_bound.exponent;
      lower_bound.exponent = upper_bound.exponent;

      value.normalize ();
      
      if (upper_bound.exponent != value.exponent)
        throw std::logic_error ("Boundary computation failure");

      auto scale_factor = find_scale_factor (value.exponent, decimal_exponent);

      upper_bound *= scale_factor;
      lower_bound *= scale_factor;
      upper_bound.significand -= 1;
      
      if (upper_bound.exponent < target_exponent_min || upper_bound.exponent > target_exponent_max)
        throw std::logic_error ("Suitable cached power unavailable");

      margin = upper_bound.significand - lower_bound.significand - 1;
    }

    u64 as_bits (f64 value)
    {
      union U
      {
        f64 value;
        u64 bits;
      } u = { value };

      return u.bits;
    }

    void categorize_float (f64 value, u64& significand, i32& exponent, bool& negative, float_kind& kind)
    {
      const u64 
        f64_neg_mask  = 1ull << 63,
        f64_mag_mask  = f64_neg_mask - 1,
        f64_exp_shift = 52,
        f64_exp_mask  = 0x7ffull << f64_exp_shift,
        f64_sig_mask  = (1ull << f64_exp_shift) - 1;

      u64 bits = as_bits (value);

      negative = (bits & f64_neg_mask) != 0;

      if (negative)
        bits &= f64_mag_mask;

      u64 raw_exp = (bits & f64_exp_mask) >> f64_exp_shift,
          raw_sig = (bits & f64_sig_mask);

      if (raw_exp == 0)
      {
        if (raw_sig == 0)
          kind = float_zero;
        else
          kind = float_subnormal;

        exponent = -1074;
      }
      else if (raw_exp == 0x7ff)
      {
        if (raw_sig == 0)
          kind = float_infinite;
        else
          kind = float_nan;
      }
      else
      {
        kind = float_normal;

        // Include implicit bit
        raw_sig |= 1ull << 52;
        exponent = i32 (raw_exp) - 1075;
      }

      significand = raw_sig;
    }

    float_expander::float_expander (double value)
    {
      u64 value_sig;
      i32 value_exp;
      categorize_float (value, value_sig, value_exp, neg, kn);

      if (kn == float_normal || kn == float_subnormal)
      {
        soft_float upper_bound;
        compute_parameters (value_sig, value_exp, upper_bound, margin, dec_exp);

        exp = upper_bound.exponent;
        u32 int_part = u32 (upper_bound.significand >> -upper_bound.exponent);
        int_expander = expand_radix (int_part, 10);
        dec_exp += int_expander.remaining ();

        frac_part = upper_bound.significand & ((1ull << -exp) - 1);
      }
      else
      {
        // Don't generate digits for zero, inf, and nan
        frac_part = 0;
        margin = 1;
      }
    }

    void spell_float (csink& sn, double value, const cnumeric_ortho& ortho, const numeric_ortho_options& opts)
    {
      float_expander fe (value);

      if (fe.negative ())
        sn.write (ortho.minus);
      else if (opts.show_plus)
        sn.write (ortho.plus);

      if (!fe.representable ())
      {
        if (fe.infinite ())
          sn.write (ortho.infinity);
        else if (fe.nan ())
          sn.write (ortho.nan);
        else if (fe.zero ())
          sn.write (ortho.digits [0]);

        return;
      }

      while (!fe.done ())
      {
        auto dig = fe.next_digit ();
        sn.write (ortho.digits [dig]);
      }

      auto exp_opts = opts;
      exp_opts.show_plus = true;
      
      sn.write (ortho.exponent_seperator);
      spell_integer (sn, fe.exponent (), ortho, exp_opts);
    }

  }

}
