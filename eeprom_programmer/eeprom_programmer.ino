
#include <stdlib.h>

#include "eeprom_data.h"

// Pin assignments
#define PIN_ADDR 2
#define PIN_SHIFT 3
#define PIN_LATCH 4
#define PIN_WRITE 13
#define PIN_D0 5
// PIN_Dn = 5 + n
#define PIN_D7 12

// Limits for AT28C64. Modify if needed
#define MAX_ADDR 1 << 13 // Exclusive maximum. (2^13 - 1)

// Which ROM do we load?
#define LOADED_ROM RGBTMUX_ROM2

const DataDescriptor TEST_ROM[] = {
  { "0 0xxx xxxx xxxx", 0xCA },
  { "0 1xxx xxxx xxxx", 0xFE },
  { "1 0xxx xxxx xxxx", 0xBA },
  { "1 1xxx xxxx xxxx", 0xBE },

  { nullptr, 0 }
};

// Utils
#define FMTBUF_SIZE 72

#define PRINTF(...) do {          \
    sprintf(FMTBUF, __VA_ARGS__); \
    Serial.print(FMTBUF);         \
  } while (0)

#define UNREACHABLE(line) do {                                     \
    sprintf(FMTBUF, "Debug unreachable code at line %d.\n", line); \
    Serial.print(FMTBUF);                                          \
  } while (0)

char FMTBUF[FMTBUF_SIZE] = {};

void pollInput(const char* query)
{
  if (query != nullptr) Serial.print(query);
  while (Serial.available() == 0) delay(10);
}

String input(const char* query = nullptr)
{
  pollInput(query);
  String got = Serial.readString();
  got.trim();
  return got;
}

void setAddress(int address, bool outputEnable)
{
  // Output is enabled if OE is low
  shiftOut(PIN_ADDR, PIN_SHIFT, MSBFIRST, (address >> 8) | (outputEnable ? 0x0 : 0x20));
  shiftOut(PIN_ADDR, PIN_SHIFT, MSBFIRST, address);

  // Transfer the bits in shift register to output register by sending a 0-1-0 pulse to RCLK
  digitalWrite(PIN_LATCH, LOW);
  digitalWrite(PIN_LATCH, HIGH);
  digitalWrite(PIN_LATCH, LOW);
}

void prepareForWrite()
{
  // Turn the EEPROM output off first.
  setAddress(0, false);
  // Then we can assert the data on the I/O pins
  for (int pin = PIN_D0; pin <= PIN_D7; ++pin)
  {
    pinMode(pin, OUTPUT);
  }
  // WE flag is set whenever the writing subroutine prepares a write operation
  digitalWrite(PIN_WRITE, HIGH); // Write is turned off if WE is high
}

void prepareForRead()
{
  // OE flag is set whenever the reading subroutine sets an address
  for (int pin = PIN_D0; pin <= PIN_D7; ++pin)
  {
    pinMode(pin, INPUT);
  }
  digitalWrite(PIN_WRITE, HIGH);
}

// EEPROM read code

// Before calling readAt(), the programmer must be put in reading state by calling prepareForRead()
byte readAt(int address)
{
  setAddress(address, true); // Enable data output from EEPROM
  byte data = 0;
  for (int dataPin = PIN_D7; dataPin >= PIN_D0; --dataPin)
  {
    data = (data << 1) | (digitalRead(dataPin) ? 0x1 : 0x0);
  }
  return data;
}

void printROM()
{
  prepareForRead();
  Serial.print("\nhADDR | 0  1  2  3  4  5  6  7  : 8  9  A  B  C  D  E  F \n");

  byte row[16];
  for (int base = 0; base < MAX_ADDR; base += 16)
  {
    for (int offset = 0; offset < 16; ++offset)
    {
      row[offset] = readAt(base + offset);
    }

    PRINTF("h%04X | %02x %02x %02x %02x %02x %02x %02x %02x : %02x %02x %02x %02x %02x %02x %02x %02x\n",
           base, row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7],
                 row[8], row[9], row[10], row[11], row[12], row[13], row[14], row[15]);
  }
}

// EEPROM write code

// Before calling pollData(), the programmer must be put in reading state by calling prepareForRead()
// Returns false if polling failed, i.e. the polling cycle has reached [n] attempts (500us x [n] of polling). True if written data has been detected
bool pollData(int address, byte data, int n = 0)
{
  constexpr int POLL_DELAY = 500; // in microsec

  int attempts = 0;
  while (readAt(address) != data)
  {
    if (attempts++ >= n)
    {
      // we have reached the polling limit.
      return false;
    }
    delayMicroseconds(POLL_DELAY);
  }

  return true;
}

// Returns false if EEPROM failed to write timely. True if written data is detected.
bool writeAt(int address, byte data, bool superimpose)
{
  PRINTF("[h%04X] <= %02X\n", address, data);
  prepareForRead();

  if (superimpose)
  {
    data |= readAt(address);
  }
  
  if (pollData(address, data))
  {
    return true; // No need to update the data in address
  }
  prepareForWrite();

  setAddress(address, false);
  byte ax = data;
  for (int dataPin = PIN_D0; dataPin <= PIN_D7; ++dataPin)
  {
    digitalWrite(dataPin, ax & 0x1);
    ax >>= 1;
  }
  digitalWrite(PIN_WRITE, LOW);
  // Hold the WE pin low for tWP = 1us
  delayMicroseconds(1);
  digitalWrite(PIN_WRITE, HIGH);
  // Wait for tWCmax = 1ms
  delay(1);

  //delay(10);
  //return;
  
  // Wait until the EEPROM has done writing
  prepareForRead();
  if (!pollData(address, data, 18))
  {
    PRINTF("(!) Failed to write data in h%04X! Skipping this address..\n", address);
    return false;
  }
  
  return true;
}

// Programming utilities

// void disableSDP()
// {
//   prepareForWrite();
//   writeAt(0x1555, 0xAA);
//   writeAt(0x0AAA, 0x55);
//   writeAt(0x1555, 0x80);
//   writeAt(0x1555, 0xAA);
//   writeAt(0x0AAA, 0x55);
//   writeAt(0x1555, 0x20);
//   delay(10);
// }

int countX(const char* pattern, int N)
{
  int numX = 0, len = 0;

  for (int i = 0; i < N; ++i)
  {
    switch (pattern[i])
    {
    case 'X': case 'x':
      ++numX;
    case '0': case '1':
      ++len;
    case ' ':
      break;
    
    default:
      UNREACHABLE(__LINE__); // Contains unexpected characters
      return -1;
    }
  }

  if (len != 13)
  {
    UNREACHABLE(__LINE__); // Address length must be 13 bits
    return -1;
  }

  return numX;
}

int collectX(int* indices, const char* pattern, int N)
{
  int i = 0, base_address = 0, index_i = 0;
  for (int j = N - 1; j >= 0; --j)
  {
    switch (pattern[j])
    {
    case ' ': continue; // skip spaces
    case 'X': case 'x':
      indices[index_i++] = i;
      // Let X's be zero in the base address
      break;
    default:
      base_address |= (pattern[j] == '0' ? 0 : 1) << i;
    }

    ++i;
  }

  return base_address;
}

bool writeForEachAddress(const char* pattern, byte data, bool superimpose)
{
  static int s_indices[13] = {};
  const int N = strlen(pattern);
  if (N < 1) return true; // Nothing to process
  
  const int NX = countX(pattern, N);
  if (NX == -1) return false; // Ill-formed address table

  PRINTF("Writing %02X to '%s'\n", data, pattern);

  // Calculate the indices for X substitution
  int base_address = collectX(s_indices, pattern, N);

  bool goodSoFar = true;
  if (!writeAt(base_address, data, superimpose)) goodSoFar = false;

  const int SUBMAX = 1 << NX; // Count from 1 to (2^NX) - 1
  for (int sub = 1; sub < SUBMAX; ++sub)
  {
    // Insert sub to the address
    int address = base_address;
    for (int i = 0; i < NX; ++i)
    {
      int subDst = s_indices[i];
      // Insert bit [i] at [sub] to bit [subDst] at [address]
      int ithBit = (sub >> i) & 1;
      address |= ithBit << subDst;
    }

    if (!writeAt(address, data, superimpose)) goodSoFar = false;
  }

  return goodSoFar;
}

void setup()
{
  pinMode(PIN_ADDR, OUTPUT);
  pinMode(PIN_SHIFT, OUTPUT);
  pinMode(PIN_LATCH, OUTPUT);
  digitalWrite(PIN_WRITE, HIGH);
  pinMode(PIN_WRITE, OUTPUT);
  Serial.begin(74880);
}

void loop()
{
  Serial.println("AT28C64 EEPROM Programmer\n\tSelect [W]rite mode, [r]ead mode, or [c]lear to continue.");
  for (;;)
  {
    char ch;
    switch (ch = input().charAt(0))
    {
      case 'C': case 'c':
        Serial.println("Clearing EEPROM..");
        writeForEachAddress("x xxxx xxxx xxxx", 0xff, false);
        Serial.println("Finished clearing");
        break;
      case 'W': case 'w':
      {
        bool superimpose = false;

        switch (input("Superimpose ROM content? (Y/n)\n").charAt(0))
        {
        case 'Y': case 'y':
          Serial.println("Preparing for superimposition...");
          writeForEachAddress("x xxxx xxxx xxxx", 0x00, false);
          superimpose = true;
        }
        
        if (!superimpose) switch (input("Clear EEPROM? (Y/n)\n").charAt(0))
        {
        case 'Y': case 'y':
          Serial.println("Clearing EEPROM..");
          writeForEachAddress("x xxxx xxxx xxxx", 0xFF, false);
        }

        bool goodSoFar = true;
        for (const DataDescriptor* now = LOADED_ROM;; ++now)
        {
          if (now->addressPattern == nullptr)
          {
            if (now->data != 0) Serial.println("(info) This ROM is not fully written yet. Don't forget to load the next part/s");
            break;
          }

          if (!writeForEachAddress(now->addressPattern, now->data, superimpose)) goodSoFar = false;
        }

        if (!goodSoFar)
        {
          Serial.println("(!) This ROM is not properly written. Check for defects");
        }
      } break;

      case 'R': case 'r':
      {
        Serial.println("[Read start]");
        printROM();
        Serial.println("[Read end]");
      } break;

    default:
      PRINTF("(!) Unknown mode '%c'\n", ch);
      continue;
    }

    break;
  }
}
