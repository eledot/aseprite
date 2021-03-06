// ASEPRITE base library
// Copyright (C) 2001-2012  David Capello
//
// This source file is distributed under a BSD-like license, please
// read LICENSE.txt for more information.

#ifndef BASE_SERIALIZATION_H_INCLUDED
#define BASE_SERIALIZATION_H_INCLUDED

#include <iosfwd>

namespace base {
namespace serialization {

  std::ostream& write8(std::ostream& os, uint8_t byte);
  uint8_t read8(std::istream& is);

  namespace little_endian {

    std::ostream& write16(std::ostream& os, uint16_t word);
    std::ostream& write32(std::ostream& os, uint32_t dword);
    uint16_t read16(std::istream& is);
    uint32_t read32(std::istream& is);

  } // little_endian namespace

  namespace big_endian {

    std::ostream& write16(std::ostream& os, uint16_t word);
    std::ostream& write32(std::ostream& os, uint32_t dword);
    uint16_t read16(std::istream& is);
    uint32_t read32(std::istream& is);

  } // big_endian namespace

} // serialization namespace
} // base namespace

#endif
