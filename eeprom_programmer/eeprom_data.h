#ifndef EEPROM_DATA_H
#define EEPROM_DATA_H

struct DataDescriptor
{
  const char* addressPattern;
  int         data;
};

extern const DataDescriptor CONTROL_ROM[];
extern const DataDescriptor TTABLE_ROM[];
extern const DataDescriptor RGBTABLE_ROM[];
extern const DataDescriptor RGBTMUX_ROM1[];
extern const DataDescriptor RGBTMUX_ROM2[];
extern const DataDescriptor DECODER_ROM[];

#endif // EEPROM_DATA_H
