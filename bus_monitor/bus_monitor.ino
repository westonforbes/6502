// Define the clock pin.
#define CLOCK_PIN 2

// Define the read/write data bus pin.
#define READ_WRITE_PIN 3

// Define the address pins, LSB 52 - MSB 22.
const char ADDRESS_PIN[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};

// Define the data pins, LSB 53 - MSB 39.
const char DATA_PIN[] = {39, 41, 43, 45, 47, 49, 51, 53};

void setup() {

  // Configure the address bus pins as inputs.
  for (int n = 0; n < 16; n += 1) pinMode(ADDRESS_PIN[n], INPUT);

  // Configure the data bus pins as inputs.
  for (int n = 0; n < 8; n += 1) pinMode(DATA_PIN[n], INPUT);

  // Configure the clock pin as a input.
  pinMode(CLOCK_PIN, INPUT);

  // Configure the read/write data bus pin as a input.
  pinMode(READ_WRITE_PIN, INPUT);

  // Set a rising edge interrupt for the clock pin.
  attachInterrupt(digitalPinToInterrupt(CLOCK_PIN), clock_rising_edge, RISING);
  
  // Estabish the serial interface.
  Serial.begin(57600);

  Serial.println("bus monitor started.");
}


void clock_rising_edge() {
  
  // Check the data bus read mode. If the RWB_PIN is high, that means the processor wants to read data from the data bus.
  bool read_mode = digitalRead(READ_WRITE_PIN);

  // Determine if the processor is reading or writing to the busses.
  char rw_mode = read_mode ? 'R': 'W';
  
  // Create a string to hold a string binary representation of the bus.
  String address_string = "";

  // Var to hold the bytecode of the address (unsigned int is 16 bits).
  unsigned int address = 0;

  // For each address bus line...
  for (int n = 0; n < 16; n += 1) {

    // Read if the line is on or off.
    int bit = digitalRead(ADDRESS_PIN[n]) ? 1 : 0;

    // Add the 0 or 1 to the string representation.
    address_string += String(bit);

    // Bit shift the address values over one and add in the new value.
    address = (address << 1) + bit;
  }

  // Create a string to hold a string binary representation of the bus.
  String data_string = "";

  // Var to hold the bytecode of the address (byte is 8 bits).
  byte data;

  // For each data bus line...
  for (int n = 0; n < 8; n += 1) {

    // Read if the line is on or off.
    int bit = digitalRead(DATA_PIN[n]) ? 1 : 0;

    // Add the 0 or 1 to the string representation.
    data_string += String(bit);

    // Bit shift the address values over one and add in the new value.
    data = (data << 1) + bit;
  }

  // Create a string to hold the op code, then lookup the op code.
  String op_code = hexToOpcode(data);
  
  // Create a var to hold the string we're going to print.
  char output[128];
  //sprintf(output, "%c: address bus: %s : %04x data bus: %s : %02x : op code: %s", rw_mode, address_string.c_str(), address, data_string.c_str(), data, op_code.c_str());
  sprintf(output, "%c      address bus: %04x      data bus: %02x", rw_mode, address, data);

  // Print the output.
  Serial.println(output);
}

void loop() {
}
