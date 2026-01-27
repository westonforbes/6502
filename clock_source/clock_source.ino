// define the code version.
#define CODE_VERSION "1.1.6"

// define the clock pin (d9 on nano).
#define CLOCK_PIN 9

// define the reset pin.
#define RESET_PIN 8

// define frequency limits (1 hz to 4mhz).
#define MIN_FREQ 1
#define MAX_FREQ 4000000

// pulse width for step command (microseconds).
#define STEP_PULSE_WIDTH 10000

// reset pulse duration (milliseconds).
#define RESET_DURATION 1000

// define global state variables.
volatile bool clockRunning = false;
volatile uint32_t currentFreq = 10000; // Updated default to 1MHz
String inputBuffer = "";

// initialize hardware and serial communication.
void setup() {
  Serial.begin(9600);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(RESET_PIN, INPUT); 
  digitalWrite(CLOCK_PIN, LOW);
  
  inputBuffer.reserve(50);
  Serial.println(F("clock source started."));
  
  // 1. Set frequency to 1MHz (currentFreq is initialized to 1000000)
  setFrequency(currentFreq);

  // 2. Perform system reset on start
  Serial.println(F("performing startup reset..."));
  assertReset();

  // 3. Automatically start the clock at 1MHz
  startClock();
  Serial.print(F("clock auto-started at: ")); Serial.print(currentFreq); Serial.println(F(" hz"));
}

// handle main loop and serial input.
void loop() {
  if (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n' || inChar == '\r') {
      processCommand(inputBuffer);
      inputBuffer = "";
    } else {
      inputBuffer += inChar;
    }
  }
}

// parse and execute incoming serial commands.
void processCommand(String cmd) {
  cmd.trim();
  cmd.toLowerCase();

  if (cmd.startsWith("freq ")) {
    uint32_t freq = cmd.substring(5).toInt();
    if (freq >= MIN_FREQ && freq <= MAX_FREQ) {
      setFrequency(freq);
      Serial.print(F("frequency set to: ")); Serial.println(currentFreq);
    } else {
      Serial.println(F("error: frequency out of range."));
    }
  } 
  else if (cmd.startsWith("pulse")) {
    int firstSpace = cmd.indexOf(' ', 6);
    uint32_t ms = (cmd.length() > 5) ? cmd.substring(5).toInt() : 0;
    uint32_t count = (firstSpace > 0) ? cmd.substring(firstSpace).toInt() : 1;
    
    for (uint32_t i = 0; i < count; i++) {
      stepPulse(ms);
      if (i < count - 1) delay(10);
    }
    Serial.println(F("pulse(s) complete."));
  }
  else if (cmd == "start")  { startClock(); Serial.println(F("clock: on")); }
  else if (cmd == "stop")   { stopClock();  Serial.println(F("clock: off")); }
  else if (cmd == "reset")  { assertReset(); Serial.println(F("system reset.")); }
  else if (cmd == "status") { printStatus(); }
  else if (cmd == "help")   { printHelp(); }
  else {
    Serial.println(F("unknown command. type help for list."));
  }
}

void setFrequency(uint32_t freq) {
  currentFreq = freq;
  uint16_t prescaler;
  uint8_t prescalerBits;

  if (freq >= 123)      { prescaler = 1;    prescalerBits = _BV(CS10); }
  else if (freq >= 16)  { prescaler = 8;    prescalerBits = _BV(CS11); }
  else if (freq >= 2)   { prescaler = 64;   prescalerBits = _BV(CS11) | _BV(CS10); }
  else                  { prescaler = 256;  prescalerBits = _BV(CS12); }

  uint32_t top = (F_CPU / (prescaler * freq)) - 1;
  if (top > 65535) top = 65535;

  noInterrupts();
  TCCR1B = 0; 
  TCNT1 = 0;
  ICR1 = top;
  OCR1A = top / 2; 
  
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  if (clockRunning) {
    TCCR1B = _BV(WGM13) | _BV(WGM12) | prescalerBits;
  }
  interrupts();
}

void startClock() {
  clockRunning = true;
  setFrequency(currentFreq);
}

void stopClock() {
  clockRunning = false;
  TCCR1B = 0;
  TCCR1A = 0;
  digitalWrite(CLOCK_PIN, LOW);
}

void stepPulse(uint32_t ms) {
  bool wasRunning = clockRunning;
  if (wasRunning) stopClock();

  digitalWrite(CLOCK_PIN, HIGH);
  if (ms > 0) delay(ms); else delayMicroseconds(STEP_PULSE_WIDTH);
  digitalWrite(CLOCK_PIN, LOW);

  if (wasRunning) startClock();
}

void assertReset() {
  bool wasRunning = clockRunning;
  if (wasRunning) stopClock();

  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);
  
  for (int i = 0; i < 4; i++) {
    digitalWrite(CLOCK_PIN, HIGH); delay(15);
    digitalWrite(CLOCK_PIN, LOW);  delay(15);
  }
  
  delay(RESET_DURATION - 120);
  pinMode(RESET_PIN, INPUT);

   for (int i = 0; i < 10; i++) {
    digitalWrite(CLOCK_PIN, HIGH); delay(15);
    digitalWrite(CLOCK_PIN, LOW);  delay(15);
  }
  if (wasRunning) startClock();
}

void printStatus() {
  Serial.print(F("\nversion: ")); Serial.println(CODE_VERSION);
  Serial.print(F("frequency: ")); Serial.print(currentFreq); Serial.println(F(" hz"));
  Serial.print(F("state: ")); Serial.println(clockRunning ? F("running") : F("stopped"));
}

void printHelp() {
  Serial.println(F("clock source help menu"));
  Serial.println(F("    freq <hz> - set the frequency of the clock."));
  Serial.println(F("        start - start the clock."));
  Serial.println(F("         stop - stop the clock."));
  Serial.println(F("        reset - reset the system."));
  Serial.println(F("pulse <x> <y> - pulse the clock for x milliseconds y times."));
  Serial.println(F("       status - display the status of the clock source."));
  Serial.println(F("         help - show this menu."));
}
