// Define the clock pin.
#define CLOCK 2

// Define the read/write data bus pin.
#define READ_WRITE 3

// Define the address pins, LSB 52 - MSB 22.
// const char ADDRESS_PIN[] = {52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32, 30, 28, 26, 24, 22};
   const char ADDRESS_PIN[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};

// Define the data pins, LSB 53 - MSB 39.
// const char DATA_PIN[] = {53, 51, 49, 47, 45, 43, 41, 39};
   const char DATA_PIN[] = {39, 41, 43, 45, 47, 49, 51, 53};

void setup() {

  // Configure the address bus pins as inputs.
  for (int n = 0; n < 16; n += 1) pinMode(ADDRESS_PIN[n], INPUT);

  // Configure the data bus pins as inputs.
  for (int n = 0; n < 8; n += 1) pinMode(DATA_PIN[n], INPUT);

  // Configure the clock pin as a input.
  pinMode(CLOCK, INPUT);

  // Configure the read/write data bus pin as a input.
  pinMode(READ_WRITE, INPUT);

  // Set a rising edge interrupt for the clock pin.
  attachInterrupt(digitalPinToInterrupt(CLOCK), clock_rising_edge, RISING);
  
  // Estabish the serial interface.
  Serial.begin(57600);
}


void clock_rising_edge() {
  // Check the data bus read mode. If the RWB_PIN is high, that means the processor wants to read data from the data bus.
  bool read_mode = digitalRead(READ_WRITE);
  
  if (read_mode) {
    Serial.print(" READ: ");
  }
  else {
    Serial.print("WRITE: ");
  }
  
  // Read the address bus.
  String address_string = "";
  unsigned int address = 0;
  for (int n = 0; n < 16; n += 1) {
    int bit = digitalRead(ADDRESS_PIN[n]) ? 1 : 0;
    address_string += String(bit);
    address = (address << 1) + bit;
  }
  
  char output[128];
  byte data;
  String op_code;
  String data_string = "";
  for (int n = 0; n < 8; n += 1) {
    int bit = digitalRead(DATA_PIN[n]) ? 1 : 0;
    data_string += String(bit);
    data = (data << 1) + bit;
  }
  op_code = hexToOpcode(data);
  
  sprintf(output, "ADDRESS BUS: %s : %04x DATA BUS: %s : %02x : OP CODE: %s", address_string.c_str(), address, data_string.c_str(), data, op_code.c_str());
  Serial.println(output);
}

void loop() {
}
