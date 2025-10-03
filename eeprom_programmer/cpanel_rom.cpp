
#include "eeprom_data.h"

const EEPROM_Data CPANEL_ROM[] = {
  //           ABCD     CASDRB
  { "xxxxxxxxx 0000", 0b000000 },
  { "xxxxxxxxx 0001", 0b100110 },
  { "xxxxxxxxx 0010", 0b101010 },
  { "xxxxxxxxx 0011", 0b100110 },
  { "xxxxxxxxx 1000", 0b000000 },
  { "xxxxxxxxx 1001", 0b000110 },
  { "xxxxxxxxx 1010", 0b011010 },
  { "xxxxxxxxx 1011", 0b000110 },
  { "xxxxxxxxx 1100", 0b010111 },
  { "xxxxxxxxx 1101", 0b000110 },
  { "xxxxxxxxx 1110", 0b000110 },
  { "xxxxxxxxx 1111", 0b000000 },
  { nullptr, 0x0 }
};
