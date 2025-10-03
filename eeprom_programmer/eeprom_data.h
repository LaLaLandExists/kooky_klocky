#ifndef HAS_EEPROM_DATA_H
#define HAS_EEPROM_DATA_H

struct EEPROM_Data
{
  const char* address;
  char        data;
};

extern const EEPROM_Data TTABLE_ROM[];
extern const EEPROM_Data CPANEL_ROM[];
extern const EEPROM_Data RGBTABLE_ROM[];

#endif