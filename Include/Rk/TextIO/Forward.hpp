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

namespace Rk
{
  namespace TextIO
  {
    template <typename Char>
    class OutDevice;

    template <typename Device>
    class OutTransform;

    template <
      typename                  Char,
      template <typename> class Device,
      template <typename> class Transform>
    struct OutStreamPolicy;

    template <typename Policy>
    class OutStream;

  }

}
