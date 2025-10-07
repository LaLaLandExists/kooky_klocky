
#include "eeprom_data.h"

const EEPROM_Data TTABLE_ROM[] = {
  // Dynamic state transitions
  //        ABCD cat
  { "xxxxxx 0000 01x", 1 },
  { "xxxxxx 0000 10x", 9 },
  { "xxxxxx 0001 0xx", 2 },
  { "xxxxxx 0010 1xx", 3 },
  { "xxxxxx 0011 0xx", 0 },
  { "xxxxxx 1000 001", 12 },
  { "xxxxxx 1000 010", 15 },
  { "xxxxxx 1000 100", 1 },
  { "xxxxxx 1001 x0x", 10 },
  { "xxxxxx 1010 x1x", 11 },
  { "xxxxxx 1011 x0x", 8 },
  { "xxxxxx 1100 01x", 13 },
  { "xxxxxx 1100 10x", 14 },
  { "xxxxxx 1101 x0x", 8 },
  { "xxxxxx 1110 0xx", 0 },
  { "xxxxxx 1111 x0x", 0 },
  // Null state transitions
  { "xxxxxx 0000 00x", 0 },
  { "xxxxxx 0000 11x", 0 },
  { "xxxxxx 0001 1xx", 1 },
  { "xxxxxx 0010 0xx", 2 },
  { "xxxxxx 0011 1xx", 3 },
  { "xxxxxx 1000 000", 8 },
  { "xxxxxx 1000 011", 8 },
  { "xxxxxx 1000 110", 8 },
  { "xxxxxx 1000 101", 8 },
  { "xxxxxx 1000 111", 8 },
  { "xxxxxx 1001 x1x", 9 },
  { "xxxxxx 1010 x0x", 10 },
  { "xxxxxx 1011 x1x", 11 },
  { "xxxxxx 1100 00x", 12 },
  { "xxxxxx 1100 11x", 12 },
  { "xxxxxx 1101 x1x", 13 },
  { "xxxxxx 1110 1xx", 14 },
  { "xxxxxx 1111 x1x", 15 },
  { nullptr, 0 }
};
