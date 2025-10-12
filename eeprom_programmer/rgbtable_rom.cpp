
#include "eeprom_data.h"

const DataDescriptor RGBTABLE_ROM[] = {
  //        F HH HHHH     RGB
  { "xxxxxx 0 00 0000", 0b101 }, // 00
  { "xxxxxx 0 00 0001", 0b101 }, // 01
  { "xxxxxx 0 00 0010", 0b101 }, // 02
  { "xxxxxx 0 00 0011", 0b101 }, // 03
  { "xxxxxx 0 00 0100", 0b101 }, // 04
  { "xxxxxx 0 00 0101", 0b101 }, // 05
  { "xxxxxx 0 00 0110", 0b011 }, // 06
  { "xxxxxx 0 00 0111", 0b011 }, // 07
  { "xxxxxx 0 00 1000", 0b011 }, // 08
  { "xxxxxx 0 00 1001", 0b011 }, // 09
  { "xxxxxx 0 01 0000", 0b011 }, // 10
  { "xxxxxx 0 01 0001", 0b011 }, // 11
  { "xxxxxx 0 01 0010", 0b110 }, // 12
  { "xxxxxx 0 01 0011", 0b110 }, // 13
  { "xxxxxx 0 01 0100", 0b110 }, // 14
  { "xxxxxx 0 01 0101", 0b110 }, // 15
  { "xxxxxx 0 01 0110", 0b110 }, // 16
  { "xxxxxx 0 01 0111", 0b110 }, // 17
  { "xxxxxx 0 01 1000", 0b001 }, // 18
  { "xxxxxx 0 01 1001", 0b001 }, // 19
  { "xxxxxx 0 10 0000", 0b001 }, // 20
  { "xxxxxx 0 10 0001", 0b001 }, // 21
  { "xxxxxx 0 10 0010", 0b001 }, // 22
  { "xxxxxx 0 10 0011", 0b001 }, // 23
  // Force Red everything
  { "xxxxxx 1 xx xxxx", 0b100 },
  { nullptr, 0 }
};
