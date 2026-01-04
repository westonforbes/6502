#define CODE_VERSION "1.1.1"

/*
 * Arduino Nano Clock Source
 * 
 * Provides a controllable clock signal for electronics testing/development
 * 
 * Commands via Serial (9600 baud):
 *   FREQ <hz>     - Set clock frequency (1 Hz to 4 MHz)
 *   PULSE [ms] [count] - Generate pulse(s) (default 10us, optional count)
 *   STOP          - Stop the clock and set output LOW
 *   START         - Resume clock at current frequency
 *   STATUS        - Display current settings
 *   HELP          - Show available commands
 *   DUTY <pct>    - Set duty cycle (1-99%, default 50%)
 *   RESET         - Assert reset signal for 1 second
 * 
 * Hardware:
 *   Clock output on Pin 9 (OC1A - Timer1, PWM capable)
 *   Status LED on Pin 13 (built-in LED)
 *   Reset output on Pin 8 (active low, tri-state)
 */

#define CLOCK_PIN  9       //
#define STATUS_LED 13      // Built-in LED
#define RESET_PIN  8       // Active low reset output

// Frequency limits for Arduino Nano (16 MHz)
#define MIN_FREQ 1         // 1 Hz minimum
#define MAX_FREQ 4000000   // 4 MHz maximum (safe upper limit)

// Pulse width for STEP command (microseconds)
#define STEP_PULSE_WIDTH 10000

// Reset pulse duration (milliseconds)
#define RESET_DURATION 1000

// State variables
volatile bool clockRunning = false;
volatile uint32_t currentFreq = 1000;  // Default 1 kHz
volatile uint8_t dutyCycle = 50;       // Default 50% duty cycle
String inputBuffer = "";
bool inputComplete = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);
  pinMode(RESET_PIN, INPUT);  // Start in high-Z state (released)
  
  digitalWrite(CLOCK_PIN, LOW);
  digitalWrite(STATUS_LED, LOW);
  
  // Initialize Timer1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  inputBuffer.reserve(50);
  
  Serial.println(F("\n=== Arduino Nano Clock Source ==="));
  Serial.print(F("Version: "));
  Serial.println(CODE_VERSION);
  Serial.println(F("Set FREQ and DUTY, then use START"));
  Serial.println(F("Type HELP for commands"));
  Serial.println();
  
  // Configure timer with default values but don't start it
  clockRunning = false;
  setFrequency(currentFreq);  // This sets up ICR1 and OCR1A properly
}

void loop() {
  // Process serial commands
  if (inputComplete) {
    processCommand(inputBuffer);
    inputBuffer = "";
    inputComplete = false;
  }
  
  // Update status LED to show running state
  digitalWrite(STATUS_LED, clockRunning);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    
    if (inChar == '\n' || inChar == '\r') {
      if (inputBuffer.length() > 0) {
        inputComplete = true;
      }
    } else {
      inputBuffer += inChar;
    }
  }
}

void processCommand(String cmd) {
  cmd.trim();
  cmd.toUpperCase();
  
  if (cmd.startsWith("FREQ ")) {
    // Extract frequency value
    String freqStr = cmd.substring(5);
    uint32_t freq = freqStr.toInt();
    
    if (freq >= MIN_FREQ && freq <= MAX_FREQ) {
      setFrequency(freq);
      Serial.print(F("Frequency set to "));
      Serial.print(currentFreq);
      Serial.println(F(" Hz"));
    } else {
      Serial.print(F("ERROR: Frequency out of range ("));
      Serial.print(MIN_FREQ);
      Serial.print(F(" Hz to "));
      Serial.print(MAX_FREQ);
      Serial.println(F(" Hz)"));
    }
  }
  else if (cmd.startsWith("PULSE")) {
    // Extract optional pulse duration in milliseconds and count
    uint32_t pulseDuration = 0;  // 0 means use default microsecond pulse
    uint32_t pulseCount = 1;     // Default to single pulse
    
    if (cmd.length() > 5) {
      String params = cmd.substring(5);
      params.trim();
      
      // Find space separator between duration and count
      int spaceIndex = params.indexOf(' ');
      
      if (spaceIndex > 0) {
        // Both duration and count provided
        String durationStr = params.substring(0, spaceIndex);
        String countStr = params.substring(spaceIndex + 1);
        countStr.trim();
        
        pulseDuration = durationStr.toInt();
        pulseCount = countStr.toInt();
        
        if (pulseCount == 0) pulseCount = 1;  // Ensure at least 1 pulse
      } else if (params.length() > 0) {
        // Only duration provided
        pulseDuration = params.toInt();
      }
    }
    
    // Generate the pulse(s)
    for (uint32_t i = 0; i < pulseCount; i++) {
      if (pulseDuration > 0) {
        stepPulse(pulseDuration);
      } else {
        stepPulse(0);  // Use default microsecond pulse
      }
      
      // Add a small delay between pulses if multiple
      if (i < pulseCount - 1) {
        delay(10);  // 10ms between pulses
      }
    }
    
    // Report what was done
    if (pulseDuration > 0) {
      Serial.print(F("Generated "));
      Serial.print(pulseCount);
      Serial.print(F(" pulse(s) of "));
      Serial.print(pulseDuration);
      Serial.println(F(" ms"));
    } else {
      Serial.print(F("Generated "));
      Serial.print(pulseCount);
      Serial.println(F(" pulse(s) (default)"));
    }
  }
  else if (cmd == "STEP") {
    // Keep STEP for backward compatibility
    stepPulse(0);
    Serial.println(F("Step pulse generated"));
  }
  else if (cmd == "STOP") {
    stopClock();
    Serial.println(F("Clock stopped"));
  }
  else if (cmd == "START") {
    startClock();
    Serial.println(F("Clock started"));
  }
  else if (cmd == "STATUS") {
    printStatus();
  }
  else if (cmd.startsWith("DUTY ")) {
    String dutyStr = cmd.substring(5);
    uint8_t duty = dutyStr.toInt();
    
    if (duty >= 1 && duty <= 99) {
      dutyCycle = duty;
      // Immediately apply new duty cycle
      setFrequency(currentFreq);
      Serial.print(F("Duty cycle set to "));
      Serial.print(dutyCycle);
      Serial.println(F("%"));
    } else {
      Serial.println(F("ERROR: Duty cycle must be 1-99%"));
    }
  }
  else if (cmd == "RESET") {
    assertReset();
    Serial.println(F("Reset asserted for 1 second"));
  }
  else if (cmd == "HELP") {
    printHelp();
  }
  else {
    Serial.print(F("Unknown command: "));
    Serial.println(cmd);
    Serial.println(F("Type HELP for available commands"));
  }
}

void setFrequency(uint32_t freq) {
  currentFreq = freq;
  
  // Calculate Timer1 settings for requested frequency
  // Arduino Nano runs at 16 MHz
  uint16_t prescaler;
  uint32_t compareValue;
  
  // Choose appropriate prescaler
  if (freq >= 123) {
    // No prescaling for higher frequencies
    prescaler = 1;
    compareValue = (F_CPU / (1UL * prescaler * freq)) - 1;
  } else if (freq >= 16) {
    // Prescaler = 8
    prescaler = 8;
    compareValue = (F_CPU / (1UL * prescaler * freq)) - 1;
  } else if (freq >= 2) {
    // Prescaler = 64
    prescaler = 64;
    compareValue = (F_CPU / (1UL * prescaler * freq)) - 1;
  } else if (freq >= 1) {
    // Prescaler = 256
    prescaler = 256;
    compareValue = (F_CPU / (1UL * prescaler * freq)) - 1;
  } else {
    // Prescaler = 1024 for very low frequencies
    prescaler = 1024;
    compareValue = (F_CPU / (1UL * prescaler * freq)) - 1;
  }
  
  // Ensure compare value fits in 16 bits
  if (compareValue > 65535) {
    compareValue = 65535;
  }
  
  // Set prescaler bits
  uint8_t prescalerBits;
  switch (prescaler) {
    case 1:    prescalerBits = _BV(CS10); break;
    case 8:    prescalerBits = _BV(CS11); break;
    case 64:   prescalerBits = _BV(CS11) | _BV(CS10); break;
    case 256:  prescalerBits = _BV(CS12); break;
    case 1024: prescalerBits = _BV(CS12) | _BV(CS10); break;
    default:   prescalerBits = _BV(CS10); break;
  }
  
  // Apply changes atomically to avoid glitches
  // Disable interrupts during timer reconfiguration
  noInterrupts();
  
  // Stop timer first
  TCCR1B = 0;
  TCCR1A = 0;
  
  // Reset counter
  TCNT1 = 0;
  
  // Set up Timer1 in Fast PWM mode (Mode 14: Fast PWM with ICR1 as TOP)
  ICR1 = compareValue;
  OCR1A = (compareValue * dutyCycle) / 100;  // Set duty cycle
  
  // Configure PWM mode - set TCCR1A first
  TCCR1A = _BV(COM1A1) | _BV(WGM11);  // Non-inverting mode, Fast PWM (lower bits)
  
  // Start timer by setting TCCR1B (upper WGM bits + prescaler)
  if (clockRunning) {
    TCCR1B = _BV(WGM13) | _BV(WGM12) | prescalerBits;
  }
  
  interrupts();
}

void startClock() {
  clockRunning = true;
  setFrequency(currentFreq);  // This will start the timer with clean state
}

void stopClock() {
  clockRunning = false;
  TCCR1B = 0;  // Stop timer
  TCCR1A = 0;  // Disconnect OC1A
  digitalWrite(CLOCK_PIN, LOW);
}

void stepPulse(uint32_t durationMs) {
  // Ensure clock is stopped first
  bool wasRunning = clockRunning;
  if (wasRunning) {
    stopClock();
  }
  
  // Generate single pulse
  digitalWrite(CLOCK_PIN, HIGH);
  
  if (durationMs > 0) {
    // Use millisecond delay for longer pulses
    delay(durationMs);
  } else {
    // Use default microsecond pulse width
    delayMicroseconds(STEP_PULSE_WIDTH);
  }
  
  digitalWrite(CLOCK_PIN, LOW);
  
  // Restore running state if it was running
  if (wasRunning) {
    startClock();
  }
}

void assertReset() {
  // Ensure clock is stopped first
  bool wasRunning = clockRunning;
  if (wasRunning) {
    stopClock();
  }
  
  // Pull reset line LOW (active low reset)
  pinMode(RESET_PIN, OUTPUT);
  digitalWrite(RESET_PIN, LOW);
  
  // Generate two clock pulses while reset is asserted
  for (int i = 0; i < 4; i++) {
    Serial.println("Pulse");
    digitalWrite(CLOCK_PIN, HIGH);
    delay(15);  // 15ms high
    digitalWrite(CLOCK_PIN, LOW);
    delay(15);  // 15ms low (30ms total per pulse)
  }
  
  // Continue holding reset for remainder of 1 second
  // We've already used 60ms for the pulses (2 pulses × 30ms each)
  delay(RESET_DURATION - 60);
  
  // Release reset back to high-Z state
  pinMode(RESET_PIN, INPUT);
  
  // Restore running state if it was running
  if (wasRunning) {
    startClock();
  }
}

void printStatus() {
  Serial.println(F("\n--- Clock Status ---"));
  Serial.print(F("Version: "));
  Serial.println(CODE_VERSION);
  Serial.print(F("State: "));
  Serial.println(clockRunning ? F("RUNNING") : F("STOPPED"));
  Serial.print(F("Frequency: "));
  Serial.print(currentFreq);
  Serial.println(F(" Hz"));
  Serial.print(F("Duty Cycle: "));
  Serial.print(dutyCycle);
  Serial.println(F("%"));
  Serial.print(F("Clock Pin: "));
  Serial.println(CLOCK_PIN);
  Serial.print(F("Reset Pin: "));
  Serial.println(RESET_PIN);
  Serial.println(F("-------------------\n"));
}

void printHelp() {
  Serial.println(F("\n=== Available Commands ==="));
  Serial.print(F("Version: "));
  Serial.println(CODE_VERSION);
  Serial.println(F("FREQ <hz>     - Set frequency (1 Hz to 4 MHz)"));
  Serial.println(F("                Example: FREQ 1000"));
  Serial.println(F("PULSE [ms] [count] - Generate pulse(s)"));
  Serial.println(F("                Example: PULSE (default 10us, 1x)"));
  Serial.println(F("                Example: PULSE 1000 (1 second, 1x)"));
  Serial.println(F("                Example: PULSE 1000 7 (1 sec, 7x)"));
  Serial.println(F("STOP          - Stop clock signal"));
  Serial.println(F("START         - Start/resume clock"));
  Serial.println(F("DUTY <pct>    - Set duty cycle (1-99%)"));
  Serial.println(F("                Example: DUTY 25"));
  Serial.println(F("RESET         - Assert reset for 1 second"));
  Serial.println(F("STATUS        - Show current settings"));
  Serial.println(F("HELP          - Show this help"));
  Serial.println(F("=========================\n"));
}