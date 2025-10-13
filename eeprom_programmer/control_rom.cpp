
#include "eeprom_data.h"

// You don't have to clear the EEPROM if you load this as it defines the entire address space

// [C]IF - Clock Inhibit Flag - stops the clock from advancing
// [A]DF - Alarm Display Flag - switches the display to show alarm
// [S]EF - Set Enabled Flag - enables the setting of the time shown in display
// [D]PF - Display Pulsate Flag - enables display pulsing at 1 Hz
// [R]DF - Red Display Flag - forces the display to show the time in red
// [B]EF - Blare Enabled Flag - enables the buzzer
const DataDescriptor CONTROL_ROM[] = {
  //           ABCD     CASDRB
  { "xxxxxxxxx xxxx", 0b100110 }, // At first, set everything to faulty configuration (the clock is paused, the display is red and blinking).
  { "xxxxxxxxx 0000", 0b000000 }, // S0 - "Running"
  { "xxxxxxxxx 0001", 0b100010 }, // S1 - "Pre-setting"
  { "xxxxxxxxx 0010", 0b101010 }, // S2 - "Setting"
  { "xxxxxxxxx 0011", 0b100010 }, // S3 - "Post-setting"
  { "xxxxxxxxx 1000", 0b000000 }, // S8 - "Running w/ Alarm"
  { "xxxxxxxxx 1001", 0b110010 }, // S9 - "Pre-alarm"
  { "xxxxxxxxx 1010", 0b111010 }, // S10 - "Alarm Setting"
  { "xxxxxxxxx 1011", 0b110010 }, // S11 - "Post-alarm"
  { "xxxxxxxxx 1100", 0b000000 }, // S12 - "Abort"
  { "xxxxxxxxx 1101", 0b010111 }, // S13 - "Blare"
  { "xxxxxxxxx 1110", 0b000010 }, // S14 - "Repeat Alarm"
  { "xxxxxxxxx 1111", 0b000010 }, // S15 - "Cancel Alarm"
  { nullptr, 0 }
};
