
#include "eeprom_data.h"

const DataDescriptor TTABLE_ROM[] = {
  // State transitions
  //       Q3210 cat
  { "xxxxxx 0000 10x", 1  }, // S0 =cst=> S1
  { "xxxxxx 0000 01x", 9  }, // S0 =ast=> S9
  { "xxxxxx 0001 0xx", 2  }, // S1 =!cst=> S2
  { "xxxxxx 0010 1xx", 3  }, // S2 =cst=> S3
  { "xxxxxx 0011 0xx", 0  }, // S3 =!cst=> S0
  { "xxxxxx 1000 100", 1  }, // S8 =cst=> S1
  { "xxxxxx 1000 010", 12 }, // S8 =ast=> S12
  { "xxxxxx 1000 001", 13 }, // S8 =att=> S13
  { "xxxxxx 1001 x0x", 10 }, // S9 =!ast=> S10
  { "xxxxxx 1010 x1x", 11 }, // S10 =ast=> S11
  { "xxxxxx 1011 x0x", 8  }, // S11 =!ast=> S8
  { "xxxxxx 1100 x0x", 0  }, // S12 =!ast=> S0
  { "xxxxxx 1101 01x", 14 }, // S13 =ast=> S14
  { "xxxxxx 1101 10x", 15 }, // S13 =cst=> S15
  { "xxxxxx 1110 x0x", 8  }, // S14 =!ast=> S8
  { "xxxxxx 1111 0xx", 0  }, // S15 =!cst=> S0
  // Null transitions
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
  { "xxxxxx 1100 x1x", 12 },
  { "xxxxxx 1101 00x", 13 },
  { "xxxxxx 1101 11x", 13 },
  { "xxxxxx 1110 x1x", 14 },
  { "xxxxxx 1111 1xx", 15 },
  // Error states, these all transition to S0
  { "xxxxxx 0100 xxx", 0 },
  { "xxxxxx 0101 xxx", 0 },
  { "xxxxxx 0110 xxx", 0 },
  { "xxxxxx 0111 xxx", 0 },
  { nullptr, 0 }
};
