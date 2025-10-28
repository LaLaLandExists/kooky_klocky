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
// DPF - Display Pulsate Flag is marked as B.
// Pulse X is marked as I.
// C is a 1 Hz negative clock pulse
const DataDescriptor DECODER_ROM[] = {
  //  C I B S TTTT UUUU
  { "xx x x 0 xxxx 0000", S_0 },
  { "xx x x 0 xxxx 0001", S_1 },
  { "xx x x 0 xxxx 0010", S_2 },
  { "xx x x 0 xxxx 0011", S_3 },
  { "xx x x 0 xxxx 0100", S_4 },
  { "xx x x 0 xxxx 0101", S_5 },
  { "xx x x 0 xxxx 0110", S_6 },
  { "xx x x 0 xxxx 0111", S_7 },
  { "xx x x 0 xxxx 1000", S_8 },
  { "xx x x 0 xxxx 1001", S_9 },
  
  { "xx x x 1 0000 xxxx", S_0 },
  { "xx x x 1 0001 xxxx", S_1 },
  { "xx x x 1 0010 xxxx", S_2 },
  { "xx x x 1 0011 xxxx", S_3 },
  { "xx x x 1 0100 xxxx", S_4 },
  { "xx x x 1 0101 xxxx", S_5 },
  { "xx x x 1 0110 xxxx", S_6 },
  { "xx x x 1 0111 xxxx", S_7 },
  { "xx x x 1 1000 xxxx", S_8 },
  { "xx x x 1 1001 xxxx", S_9 },
  // If B = 1, The output is asserted if C = 1, else the output is blank
  { "x0 0 1 x xxxx xxxx", BLANK }, // Impose blank on C = 0, I = 0, B = 1
  { "x0 1 0 x xxxx xxxx", BLANK }, // Impose blank on C = 0, I = 1, B = 1
  { "x0 1 1 x xxxx xxxx", BLANK }, // Impose blank on C = 0, I = 1, B = 1
  { nullptr, 0x0 }
};

// A variant of DECODER_ROM intended for seconds.
// The seconds display does not pulse, it responds to DPF by turning it off. ADF is marked as B
// ADF - Alarm Display Flag is marked as I
// The S variant does not respond to a clock pulse
const DataDescriptor DECODER_S_ROM[] = {
 //   C I B S TTTT UUUU
  { "xx 0 0 0 xxxx 0000", S_0 },
  { "xx 0 0 0 xxxx 0001", S_1 },
  { "xx 0 0 0 xxxx 0010", S_2 },
  { "xx 0 0 0 xxxx 0011", S_3 },
  { "xx 0 0 0 xxxx 0100", S_4 },
  { "xx 0 0 0 xxxx 0101", S_5 },
  { "xx 0 0 0 xxxx 0110", S_6 },
  { "xx 0 0 0 xxxx 0111", S_7 },
  { "xx 0 0 0 xxxx 1000", S_8 },
  { "xx 0 0 0 xxxx 1001", S_9 },
  
  { "xx 0 0 1 0000 xxxx", S_0 },
  { "xx 0 0 1 0001 xxxx", S_1 },
  { "xx 0 0 1 0010 xxxx", S_2 },
  { "xx 0 0 1 0011 xxxx", S_3 },
  { "xx 0 0 1 0100 xxxx", S_4 },
  { "xx 0 0 1 0101 xxxx", S_5 },
  { "xx 0 0 1 0110 xxxx", S_6 },
  { "xx 0 0 1 0111 xxxx", S_7 },
  { "xx 0 0 1 1000 xxxx", S_8 },
  { "xx 0 0 1 1001 xxxx", S_9 },
  // If B = 1, The output is asserted if C = 1, else the output is blank
  { "xx 0 1 x xxxx xxxx", BLANK }, // Impose blank on C = 0, I = 0, B = 1
  { "xx 1 0 x xxxx xxxx", BLANK }, // Impose blank on C = 0, I = 1, B = 1
  { "xx 1 1 x xxxx xxxx", BLANK }, // Impose blank on C = 0, I = 1, B = 1
  { nullptr, 0x0 }
};
