#include "eeprom_data.h"

//              GFEDCBA
#define BLANK 0b0000000
#define S_0   0b0111111
#define S_1   0b0000110
#define S_2   0b1011011
#define S_3   0b1001111
#define S_4   0b1100110
#define S_5   0b1101101
#define S_6   0b1111101
#define S_7   0b0000111
#define S_8   0b1111111
#define S_9   0b1101111

// This is equivalent to a 4-bit 2 to 1 Multiplexer connected to a 7 segment decoder with strobe
const DataDescriptor DECODER_ROM[] = {
  //   C B S TTTT UUUU
  { "xxx x 0 xxxx 0000", S_0 },
  { "xxx x 0 xxxx 0001", S_1 },
  { "xxx x 0 xxxx 0010", S_2 },
  { "xxx x 0 xxxx 0011", S_3 },
  { "xxx x 0 xxxx 0100", S_4 },
  { "xxx x 0 xxxx 0101", S_5 },
  { "xxx x 0 xxxx 0110", S_6 },
  { "xxx x 0 xxxx 0111", S_7 },
  { "xxx x 0 xxxx 1000", S_8 },
  { "xxx x 0 xxxx 1001", S_9 },
  
  { "xxx x 1 0000 xxxx", S_0 },
  { "xxx x 1 0001 xxxx", S_1 },
  { "xxx x 1 0010 xxxx", S_2 },
  { "xxx x 1 0011 xxxx", S_3 },
  { "xxx x 1 0100 xxxx", S_4 },
  { "xxx x 1 0101 xxxx", S_5 },
  { "xxx x 1 0110 xxxx", S_6 },
  { "xxx x 1 0111 xxxx", S_7 },
  { "xxx x 1 1000 xxxx", S_8 },
  { "xxx x 1 1001 xxxx", S_9 },
  // If B = 1, The output is asserted if C = 1, else the output is blank
  { "xx0 1 x xxxx xxxx", BLANK }, // Impose blank on C = 0, B = 1
  { nullptr, 0x0 }
};
