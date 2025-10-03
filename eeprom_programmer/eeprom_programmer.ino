#include <Arduino_BuiltIn.h>
#include <string.h>

#include "eeprom_data.h"

#define ADDR_WIDTH 13
#define ADDR_MAX   (1 << 13)
#define DATA_WIDTH 8

#define PIN_LATCH 2
#define PIN_SHIFT 3
#define PIN_ADDR  4
#define PIN_WE    13
#define PIN_D0    5
#define PIN_D1    6
#define PIN_D2    7
#define PIN_D3    8
#define PIN_D4    9
#define PIN_D5    10
#define PIN_D6    11
#define PIN_D7    12

// Configuration
#define LOADED_ROM CPANEL_ROM // What content should we load into the EEPROM?

static char FMTBUF[128] = {};

typedef void (*AddrCollector)(int address, byte data);

int _countx_and_validate(const char* addr, int N)
{
  int nx = 0, len = 0;

  for (int i = 0; i < N; ++i)
  {
    switch (addr[i])
    {
    case '0':
    case '1':
      ++len;
    case ' ':
      break;
    case 'X':
    case 'x':
      ++nx; ++len;
      break;
    default:
      Serial.println("[[!]] Address contains unexpected characters");
      return -1;
    }
  }

  if (len != ADDR_WIDTH)
  {
    Serial.println("[[!]] Address length does not match address width");
    return -1;
  }

  return nx;
}

int _collect_indices(int* indices, const char* addr, int N)
{
  int cur = 0, raw_address = 0, i_cur = 0;
  for (int i = N - 1; i >= 0; --i)
  {
    if (addr[i] == ' ') continue;
    if (addr[i] == 'X' || addr[i] == 'x')
    {
      indices[i_cur++] = cur;
      // Keep wildcards zero for easier bitmasking (by not changing raw_address)
    }
    else
    {
      int bit = addr[i] == '0' ? 0 : 1;
      raw_address |= bit << cur;
    }

    ++cur;
  }
  return raw_address;
}

void _addressWrite(int address, int oeState)
{
  // An address word is 13 bits + 1 bit for the OE pin
  shiftOut(PIN_ADDR, PIN_SHIFT, MSBFIRST, (address >> 8) | (oeState ? 0x20 : 0x0));
  shiftOut(PIN_ADDR, PIN_SHIFT, MSBFIRST, address);

  // Take the latching clock low-high-low to copy the address to the output latches
  digitalWrite(PIN_LATCH, LOW);
  digitalWrite(PIN_LATCH, HIGH);
  digitalWrite(PIN_LATCH, LOW);
}

void _writeToAddress(int address, byte data)
{
  /* Byte write procedure for AT28C64
   *  1) WE is pulled high
   *  2) Setup address pins (in series using 74HC595)
   *  3) Setup data pins (in parallel)
   *  4) Wait for tAS = 10ns. The MCU is too slow for this to need a delay call
   *  5) WE is pulled low to latch the address
   *  6) Wait for tWP = 1000ns
   *  7) WE is pulled high to latch the data
   *  8) Wait for tWC - tWP = 1ms - 1000ns ~~ 1ms (is this really needed?)
   */

  // WE is pulled high
  digitalWrite(PIN_WE, HIGH);
  // Setup address pins and set OE high
  _addressWrite(address, HIGH);
  // Setup data pins
  for (int i = PIN_D0; i <= PIN_D7; ++i)
  {
    digitalWrite(i, address & 0x1);
    address >>= 1;
  }
  // WE is pulled low
  digitalWrite(PIN_WE, LOW);
  // Wait for tWP = 1000ns/1us
  delayMicroseconds(1);
  // WE is pulled high
  digitalWrite(PIN_WE, HIGH);
  // Wait for tWC - tWP = 1ms
  delay(1);

  sprintf(FMTBUF, "(!) Wrote '%02X' at [0x%X]", data, address);
  Serial.println(FMTBUF);
}

void _forEachAddress(const char* addr, byte data, AddrCollector collector)
{
  static int s_indices[ADDR_WIDTH] = { 0 };
  const int N = strlen(addr);
  if (N < 1) return;

  const int NX = _countx_and_validate(addr, N);
  if (NX == -1) return;

  // Calculate all indices for wildcards
  int raw_address = _collect_indices(s_indices, addr, N);

  collector(raw_address, data);

  const int WMAX = 1 << NX;
  for (int xval = 1; xval < WMAX; ++xval)
  {
    // Insert xval to the address
    int address = raw_address;
    for (int i = 0; i < NX; ++i)
    {
      int targetIndex = s_indices[i];
      // Insert bit [i] at xval to bit [targetIndex] at address
      int ithBit = (xval >> i) & 0x1;
      address |= ithBit << targetIndex;
    }
    collector(address, data);
  }
}

void _prepareForWrite()
{
  // Set the pins
  for (int pin = PIN_D0; pin <= PIN_D7; ++pin)
  {
    pinMode(pin, OUTPUT);
  }
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_SHIFT, OUTPUT);
  pinMode(PIN_ADDR, OUTPUT);
  pinMode(PIN_WE, OUTPUT);
  digitalWrite(PIN_WE, HIGH);
  _addressWrite(0, HIGH); // Turn off output enable
}

void _prepareForRead()
{
  for (int pin = PIN_D0; pin <= PIN_D7; ++pin)
  {
    pinMode(pin, INPUT);
  }
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_SHIFT, OUTPUT);
  pinMode(PIN_ADDR, OUTPUT);
  pinMode(PIN_WE, OUTPUT);
  digitalWrite(PIN_WE, HIGH);
}

static bool s_writeMatches = true;
static int s_brokenAddr = 0;

void _validateData(int address, byte data)
{
  if (_readData(address) != data)
  {
    s_writeMatches = false;
    s_brokenAddr = address;
  }
}

String input(const char* message = nullptr, unsigned int waitTimeUs = 750);
void writeData(const EEPROM_Data* data)
{
  _prepareForWrite();
  // Clear the EEPROM first (without using the EEPROM clear feature)
  Serial.println("Clearing EEPROM..");
  _forEachAddress("xxxxxxxxxxxxx", 0, _writeToAddress);

  for (const EEPROM_Data* now = data; now->address != nullptr; ++now)
  {
    _forEachAddress(now->address, now->data, _writeToAddress);
  }

  String response = input("Do an integrity check? (Y/n) $ ");
  response.trim();

  switch (response.charAt(0))
  {
  case 'Y':
  case 'y':
    _prepareForRead();
    s_writeMatches = true;
    for (const EEPROM_Data* now = data; now->address != nullptr && s_writeMatches; ++now)
    {
      _forEachAddress(now->address, now->data, _validateData);
    }
    if (!s_writeMatches)
    {
      sprintf(FMTBUF, "(!) EEPROM is not written properly at 0x%x.", s_brokenAddr);
    }
    break;
  default:
    Serial.println("Skipped write check");
  }
}

byte _readData(int address)
{
  _addressWrite(address, LOW);

  byte data = 0;
  for (int pin = PIN_D7; pin >= PIN_D0; --pin)
  {
    data = (data << 1) | digitalRead(pin);
  }

  return data;
}

void readAllData()
{
  _prepareForRead();
  byte row[8] = {};

  // 1 Byte Divisions
  sprintf(FMTBUF, "       | b0 b1 b2 b3 b4 b5 b6 b7");
  Serial.println(FMTBUF);
  for (int i = 0; i < ADDR_MAX; i += 8)
  {
    for (int j = 0; j < 8; ++j)
    {
      row[j] = _readData(i | j);
    }

    sprintf(FMTBUF, "0x%04X | %02X %02X %02X %02X %02X %02X %02X %02X", i,
            row[0], row[1], row[2], row[3], row[4], row[5], row[6], row[7]);
    Serial.println(FMTBUF);
  }

}

void _pollInput(const char* message, unsigned int waitTimeUs)
{
  if (message != nullptr) Serial.print(message);
  while (Serial.available() == 0) delayMicroseconds(waitTimeUs);
}

String input(const char* message, unsigned int waitTimeUs)
{
  _pollInput(message, waitTimeUs);
  return Serial.readString();
}

int inputInt(const char* message = nullptr, unsigned int waitTimeUs = 750)
{
  _pollInput(message, waitTimeUs);
  return Serial.parseInt();
}

void _readMode()
{
  Serial.println("Reading ROM..");
  readAllData();
  Serial.println("[end]");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("AT28C64 EEPROM Reader/Programmer");
  Serial.println("\tThis program runs in [W]rite mode or [R]ead mode.");

  for (;;)
  {
    String response = input("$ ");
    response.trim();

    switch (response.charAt(0))
    {
    case 'W':
    case 'w':
      writeData(LOADED_ROM);
      break;
    case 'R':
    case 'r':
      _readMode();
      break;
    default:
      Serial.println("(!) Unknown mode.");
      continue;
    }
    // This loop runs once on valid input
    break;
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}
