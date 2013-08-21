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

#include <Rk/TextIO/SpellFloat.hpp>

#include "SoftFloat.hpp"

#include <stdexcept>

namespace Rk
{
  namespace TextIO
  {
    struct ScaleFactor
    {
      SoftFloat value;
      i32      decimal_exponent;

      ScaleFactor () { }

      ScaleFactor (SoftFloat v, i32 de) :
        value (v), decimal_exponent (de)
      { }
    
    };

    const ScaleFactor scale_factors [] = {
      ScaleFactor (SoftFloat (0x9a6bb0aa55653b2dull, -1113), -316),
      ScaleFactor (SoftFloat (0xe61acf033d1a45dfull, -1087), -308),
      ScaleFactor (SoftFloat (0xab70fe17c79ac6caull, -1060), -300),
      ScaleFactor (SoftFloat (0xff77b1fcbebcdc4full, -1034), -292),
      ScaleFactor (SoftFloat (0xbe5691ef416bd60cull, -1007), -284),
      ScaleFactor (SoftFloat (0x8dd01fad907ffc3cull,  -980), -276),
      ScaleFactor (SoftFloat (0xd3515c2831559a83ull,  -954), -268),
      ScaleFactor (SoftFloat (0x9d71ac8fada6c9b5ull,  -927), -260),
      ScaleFactor (SoftFloat (0xea9c227723ee8bcbull,  -901), -252),
      ScaleFactor (SoftFloat (0xaecc49914078536dull,  -874), -244),
      ScaleFactor (SoftFloat (0x823c12795db6ce57ull,  -847), -236),
      ScaleFactor (SoftFloat (0xc21094364dfb5637ull,  -821), -228),
      ScaleFactor (SoftFloat (0x9096ea6f3848984full,  -794), -220),
      ScaleFactor (SoftFloat (0xd77485cb25823ac7ull,  -768), -212),
      ScaleFactor (SoftFloat (0xa086cfcd97bf97f4ull,  -741), -204),
      ScaleFactor (SoftFloat (0xef340a98172aace5ull,  -715), -196),
      ScaleFactor (SoftFloat (0xb23867fb2a35b28eull,  -688), -188),
      ScaleFactor (SoftFloat (0x84c8d4dfd2c63f3bull,  -661), -180),
      ScaleFactor (SoftFloat (0xc5dd44271ad3cdbaull,  -635), -172),
      ScaleFactor (SoftFloat (0x936b9fcebb25c996ull,  -608), -164),
      ScaleFactor (SoftFloat (0xdbac6c247d62a584ull,  -582), -156),
      ScaleFactor (SoftFloat (0xa3ab66580d5fdaf6ull,  -555), -148),
      ScaleFactor (SoftFloat (0xf3e2f893dec3f126ull,  -529), -140),
      ScaleFactor (SoftFloat (0xb5b5ada8aaff80b8ull,  -502), -132),
      ScaleFactor (SoftFloat (0x87625f056c7c4a8bull,  -475), -124),
      ScaleFactor (SoftFloat (0xc9bcff6034c13053ull,  -449), -116),
      ScaleFactor (SoftFloat (0x964e858c91ba2655ull,  -422), -108),
      ScaleFactor (SoftFloat (0xdff9772470297ebdull,  -396), -100),
      ScaleFactor (SoftFloat (0xa6dfbd9fb8e5b88full,  -369),  -92),
      ScaleFactor (SoftFloat (0xf8a95fcf88747d94ull,  -343),  -84),
      ScaleFactor (SoftFloat (0xb94470938fa89bcfull,  -316),  -76),
      ScaleFactor (SoftFloat (0x8a08f0f8bf0f156bull,  -289),  -68),
      ScaleFactor (SoftFloat (0xcdb02555653131b6ull,  -263),  -60),
      ScaleFactor (SoftFloat (0x993fe2c6d07b7facull,  -236),  -52),
      ScaleFactor (SoftFloat (0xe45c10c42a2b3b06ull,  -210),  -44),
      ScaleFactor (SoftFloat (0xaa242499697392d3ull,  -183),  -36),
      ScaleFactor (SoftFloat (0xfd87b5f28300ca0eull,  -157),  -28),
      ScaleFactor (SoftFloat (0xbce5086492111aebull,  -130),  -20),
      ScaleFactor (SoftFloat (0x8cbccc096f5088ccull,  -103),  -12),
      ScaleFactor (SoftFloat (0xd1b71758e219652cull,   -77),   -4),
      ScaleFactor (SoftFloat (0x9c40000000000000ull,   -50),    4),
      ScaleFactor (SoftFloat (0xe8d4a51000000000ull,   -24),   12),
      ScaleFactor (SoftFloat (0xad78ebc5ac620000ull,     3),   20),
      ScaleFactor (SoftFloat (0x813f3978f8940984ull,    30),   28),
      ScaleFactor (SoftFloat (0xc097ce7bc90715b3ull,    56),   36),
      ScaleFactor (SoftFloat (0x8f7e32ce7bea5c70ull,    83),   44),
      ScaleFactor (SoftFloat (0xd5d238a4abe98068ull,   109),   52),
      ScaleFactor (SoftFloat (0x9f4f2726179a2245ull,   136),   60),
      ScaleFactor (SoftFloat (0xed63a231d4c4fb27ull,   162),   68),
      ScaleFactor (SoftFloat (0xb0de65388cc8ada8ull,   189),   76),
      ScaleFactor (SoftFloat (0x83c7088e1aab65dbull,   216),   84),
      ScaleFactor (SoftFloat (0xc45d1df942711d9aull,   242),   92),
      ScaleFactor (SoftFloat (0x924d692ca61be758ull,   269),  100),
      ScaleFactor (SoftFloat (0xda01ee641a708deaull,   295),  108),
      ScaleFactor (SoftFloat (0xa26da3999aef774aull,   322),  116),
      ScaleFactor (SoftFloat (0xf209787bb47d6b85ull,   348),  124),
      ScaleFactor (SoftFloat (0xb454e4a179dd1877ull,   375),  132),
      ScaleFactor (SoftFloat (0x865b86925b9bc5c2ull,   402),  140),
      ScaleFactor (SoftFloat (0xc83553c5c8965d3dull,   428),  148),
      ScaleFactor (SoftFloat (0x952ab45cfa97a0b3ull,   455),  156),
      ScaleFactor (SoftFloat (0xde469fbd99a05fe3ull,   481),  164),
      ScaleFactor (SoftFloat (0xa59bc234db398c25ull,   508),  172),
      ScaleFactor (SoftFloat (0xf6c69a72a3989f5cull,   534),  180),
      ScaleFactor (SoftFloat (0xb7dcbf5354e9beceull,   561),  188),
      ScaleFactor (SoftFloat (0x88fcf317f22241e2ull,   588),  196),
      ScaleFactor (SoftFloat (0xcc20ce9bd35c78a5ull,   614),  204),
      ScaleFactor (SoftFloat (0x98165af37b2153dfull,   641),  212),
      ScaleFactor (SoftFloat (0xe2a0b5dc971f303aull,   667),  220),
      ScaleFactor (SoftFloat (0xa8d9d1535ce3b396ull,   694),  228),
      ScaleFactor (SoftFloat (0xfb9b7cd9a4a7443cull,   720),  236),
      ScaleFactor (SoftFloat (0xbb764c4ca7a44410ull,   747),  244),
      ScaleFactor (SoftFloat (0x8bab8eefb6409c1aull,   774),  252),
      ScaleFactor (SoftFloat (0xd01fef10a657842cull,   800),  260),
      ScaleFactor (SoftFloat (0x9b10a4e5e9913129ull,   827),  268),
      ScaleFactor (SoftFloat (0xe7109bfba19c0c9dull,   853),  276),
      ScaleFactor (SoftFloat (0xac2820d9623bf429ull,   880),  284),
      ScaleFactor (SoftFloat (0x80444b5e7aa7cf85ull,   907),  292),
      ScaleFactor (SoftFloat (0xbf21e44003acdd2dull,   933),  300),
      ScaleFactor (SoftFloat (0x8e679c2f5e44ff8full,   960),  308),
      ScaleFactor (SoftFloat (0xd433179d9c8cb841ull,   986),  316),
      ScaleFactor (SoftFloat (0x9e19db92b4e31ba9ull,  1013),  324),
      ScaleFactor (SoftFloat (0xeb96bf6ebadf77d9ull,  1039),  332)
    };

    const i32 target_exponent_min = -59,
              target_exponent_max = -32,
              decimal_exponent_min = scale_factors [0].decimal_exponent,
              decimal_exponent_stride = scale_factors [1].decimal_exponent - decimal_exponent_min;

    SoftFloat find_scale_factor (i32 exponent, i32& decimal_exponent)
    {
      const auto recip_log2_10 = 0.30102999566398114;
      auto k = (i32) std::ceil ((target_exponent_min - exponent - 1) * recip_log2_10);
      auto index = (k - decimal_exponent_min - 1) / decimal_exponent_stride + 1;
      decimal_exponent = -scale_factors [index].decimal_exponent;
      return scale_factors [index].value;
    }

    void compute_parameters (u64 significand, i32 exponent, SoftFloat& upper_bound, u64& margin, i32& decimal_exponent)
    {
      SoftFloat
        value (significand << 1, exponent - 1),
        lower_bound (value.significand - 1, value.exponent);

      upper_bound = SoftFloat (value.significand + 1, value.exponent);
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

    template <typename Char>
    auto generate_digits (SoftFloat upper_bound, u64 margin, i32 decimal_exponent, Char zero)
      -> FloatSpelling <Char>
    {
      const u64 fractional_mask = (1ull << -upper_bound.exponent) - 1;
      
      uptr length = 0;
      i32 descale_exponent = 10;
      
      u32 scaled_integrals = u32 (upper_bound.significand >> -upper_bound.exponent);
      u64 scaled_fractionals = upper_bound.significand & fractional_mask;
      
      FloatSpelling <Char> spelling;
      spelling.exponent () = decimal_exponent;

      // Generate integrals
      u32 divisor = 1000000000;
      while (descale_exponent > 0)
      {
        if (spelling.full ())
          throw std::runtime_error ("Out of buffer space");

        Char digit = Char (scaled_integrals / divisor);
        if (digit != 0 || length != 0)
          spelling.push_back (zero + digit);

        scaled_integrals %= divisor;
        descale_exponent--;
        divisor /= 10;

        if ((u64 (scaled_integrals) << -upper_bound.exponent) + scaled_fractionals <= margin)
        {
          spelling.exponent () += descale_exponent;
          return spelling;
        }
      }

      // Generate fractionals
      do
      {
        if (spelling.full ())
          throw std::runtime_error ("Out of buffer space");

        scaled_fractionals *= 10;

        Char digit = Char (scaled_fractionals >> -upper_bound.exponent);
        if (digit != 0 || length != 0)
          spelling.push_back (zero + digit);

        scaled_fractionals &= fractional_mask;
        descale_exponent--;
        margin *= 10;
      }
      while (scaled_fractionals > margin);

      spelling.exponent () += descale_exponent;
      return spelling;
    }

    template <typename Char>
    auto grisu2 (u64 significand, i32 exponent, Char zero)
      -> FloatSpelling <Char>
    {
      SoftFloat upper_bound;
      u64       margin;
      i32       decimal_exponent;

      compute_parameters (significand, exponent, upper_bound, margin, decimal_exponent);

      return generate_digits (upper_bound, margin, decimal_exponent, zero);
    }

    void categorize_float (f64 value, u64& significand, i32& exponent,  bool& negative, FloatKind& kind)
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

    template <typename Char>
    auto spell_float_impl (f64 value, Char zero)
      -> FloatSpelling <Char>
    {
      u64       significand;
      i32       exponent;
      bool      negative;
      FloatKind kind;
      categorize_float (value, significand, exponent, negative, kind);

      FloatSpelling <Char> result;

      // Nothing to do for zero, inf, and nan
      if (kind == float_normal || kind == float_subnormal)
        result = grisu2 (significand, exponent, zero);
      
      result.negative () = negative;
      result.kind     () = kind;

      return result;
    }

    auto RK_API spell_float (f64 mag, char zero)
      -> FloatSpelling <char>
    {
      return spell_float_impl (mag, zero);
    }

    auto RK_API spell_float (f64 mag, char16 zero)
      -> FloatSpelling <char16>
    {
      return spell_float_impl (mag, zero);
    }

  }

}
