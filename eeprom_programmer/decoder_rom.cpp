#include "eeprom_data.h"

//              ABCEDGF  // Why is it like this? for ease of routing of course.
#define BLANK 0b00000000
#define S_0   0b11111010
#define S_1   0b01100000
#define S_2   0b11011100
#define S_3   0b11101100
#define S_4   0b01100110
#define S_5   0b10101110
#define S_6   0b10111110
#define S_7   0b11100000
#define S_8   0b11111110
#define S_9   0b11101110

// This is equivalent to a 4-bit 2 to 1 Multiplexer connected to a 7 segment decoder with strobe
// DPF - Display Pulsate Flag is marked as B.
// Pulse X is marked as I.
// C is a 1 Hz negative clock pulse
const DataDescriptor DECODER_ROM[] = {
  //    M C I S TTT UUUU
  { "xx x x x 0 xxx 0000", S_0 },
  { "xx x x x 0 xxx 0001", S_1 },
  { "xx x x x 0 xxx 0010", S_2 },
  { "xx x x x 0 xxx 0011", S_3 },
  { "xx x x x 0 xxx 0100", S_4 },
  { "xx x x x 0 xxx 0101", S_5 },
  { "xx x x x 0 xxx 0110", S_6 },
  { "xx x x x 0 xxx 0111", S_7 },
  { "xx x x x 0 xxx 1000", S_8 },
  { "xx x x x 0 xxx 1001", S_9 },
  
  { "xx x x x 1 000 xxxx", S_0 },
  { "xx x x x 1 001 xxxx", S_1 },
  { "xx x x x 1 010 xxxx", S_2 },
  { "xx x x x 1 011 xxxx", S_3 },
  { "xx x x x 1 100 xxxx", S_4 },
  { "xx x x x 1 101 xxxx", S_5 },
  { "xx x x x 1 110 xxxx", S_6 },
  { "xx x x x 1 111 xxxx", S_7 },
  // If B = 1, The output is asserted if C = 1, else the output is blank
  { "xx 1 0 1 x xxx xxxx", BLANK }, // Impose blank on C = 0, I = 1, B = 1
  { nullptr, 0x0 }
};

// A variant of DECODER_ROM intended for seconds.
// The seconds display does not pulse, it responds to DPF by turning it off. ADF is marked as B
// ADF - Alarm Display Flag is marked as I
// The S variant does not respond to a clock pulse
const DataDescriptor DECODER_S_ROM[] = {
 //       I S TTT UUUU
  { "xxxx 0 0 xxx 0000", S_0 },
  { "xxxx 0 0 xxx 0001", S_1 },
  { "xxxx 0 0 xxx 0010", S_2 },
  { "xxxx 0 0 xxx 0011", S_3 },
  { "xxxx 0 0 xxx 0100", S_4 },
  { "xxxx 0 0 xxx 0101", S_5 },
  { "xxxx 0 0 xxx 0110", S_6 },
  { "xxxx 0 0 xxx 0111", S_7 },
  { "xxxx 0 0 xxx 1000", S_8 },
  { "xxxx 0 0 xxx 1001", S_9 },
  
  { "xxxx 0 1 000 xxxx", S_0 },
  { "xxxx 0 1 001 xxxx", S_1 },
  { "xxxx 0 1 010 xxxx", S_2 },
  { "xxxx 0 1 011 xxxx", S_3 },
  { "xxxx 0 1 100 xxxx", S_4 },
  { "xxxx 0 1 101 xxxx", S_5 },
  { "xxxx 0 1 110 xxxx", S_6 },
  { "xxxx 0 1 111 xxxx", S_7 },
  // If B = 1, The output is asserted if C = 1, else the output is blank
  { "xxxx 1 x xxx xxxx", BLANK }, // Impose blank on C = 0, I = 0, B = 1
  { nullptr, 0x0 }
};
