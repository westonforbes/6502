// Define the clock pin.
#define CLOCK_PIN 2

// Define the read/write data bus pin.
#define READ_WRITE_PIN 3

// Define the address pins, LSB 52 - MSB 22.
const char ADDRESS_PIN[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};

// Define the data pins, LSB 53 - MSB 39.
const char DATA_PIN[] = {39, 41, 43, 45, 47, 49, 51, 53};

// Define the data2 pins, LSB 37 - MSB 23.
const char DATA2_PIN[] = {23, 25, 27, 29, 31, 33, 35, 37};

// Define the data3 pins, LSB 21 - MSB 14.
const char DATA3_PIN[] = {14, 15, 16, 17, 18, 19, 20, 21};

unsigned long count = 0; // 0 to 18,446,744,073,709,551,615

void setup() {
  for (int n = 0; n < 16; n += 1) pinMode(ADDRESS_PIN[n], INPUT);
  for (int n = 0; n < 8; n += 1) pinMode(DATA_PIN[n], INPUT);
  for (int n = 0; n < 8; n += 1) pinMode(DATA2_PIN[n], INPUT);
  pinMode(CLOCK_PIN, INPUT);
  pinMode(READ_WRITE_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), clock_rising_edge, RISING);
  
  Serial.begin(57600);
  Serial.println("bus monitor started.");
}

void clock_rising_edge() {
  count++;
  bool read_mode = digitalRead(READ_WRITE_PIN);
  char rw_mode = read_mode ? 'R': 'W';
  
  String address_string = "";
  unsigned int address = 0;
  for (int n = 0; n < 16; n += 1) {
    int bit = digitalRead(ADDRESS_PIN[n]) ? 1 : 0;
    address_string += String(bit);
    address = (address << 1) + bit;
  }

  String data_string = "";
  byte data = 0;
  for (int n = 0; n < 8; n += 1) {
    int bit = digitalRead(DATA_PIN[n]) ? 1 : 0;
    data_string += String(bit);
    data = (data << 1) + bit;
  }

  String data2_string = "";
  byte data2 = 0;
  for (int n = 0; n < 8; n += 1) {
    int bit = digitalRead(DATA2_PIN[n]) ? 1 : 0;
    data2_string += String(bit);
    data2 = (data2 << 1) + bit;
  }

  String data3_string = "";
  byte data3 = 0;
  for (int n = 0; n < 8; n += 1) {
    int bit = digitalRead(DATA3_PIN[n]) ? 1 : 0;
    data3_string += String(bit);
    data3 = (data3 << 1) + bit;
  }

  // --- Color Logic ---
  String color_code = "";
  String reset_code = "\033[0m";

  if (data == 0xA0) {
    color_code = "\033[31m"; // Red for LDY breadcrumb
  } else if (address == 0x8000) {
    color_code = "\033[32m"; // Green for Reset target
  }

  char output[160]; // Increased buffer size to accommodate escape codes
  sprintf(output, "%s%010lu  [%c]    a, d1, d2, d3: [%s][%s][%s][%s] - [%04x][%02x][%02x][%02x]%s", 
          color_code.c_str(), count, rw_mode, address_string.c_str(), data_string.c_str(), 
          data2_string.c_str(), data3_string.c_str(), address, data, data2, data3, reset_code.c_str());

  Serial.println(output);
}

void loop() {
}
