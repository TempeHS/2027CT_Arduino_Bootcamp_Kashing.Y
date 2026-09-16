/*
  Author:
  Learning Intention: Students will build a monitoring system that watches
  conditions and raises alerts, like the guardian inside a smart fridge

  The build: FRIDGE MONITOR
  1. Calibrate the light sensor FIRST (see below), then set your threshold
  2. The light sensor detects the door opening (light floods into a dark fridge)
  3. If the door stays open longer than 10 seconds, sound the buzzer and flash
the LED until the door closes
  4. Keep the loop non-blocking: use the millis() pattern for the alarm flash
AND the Serial Plotter output (no delay() anywhere)
  5. Plot the light level and door state so you can watch your threshold work

  Calibration (why: every bench is different):
    Ambient light, sensor tolerance and how enclosed your "fridge" is all
    change the raw readings, so a threshold that works for one build fails on
    another. Upload a bare reader first:
        void setup() { Serial.begin(115200); }
        void loop()  { Serial.println(analogRead(A3)); delay(50); }
    Watch the Serial Plotter. Record the covered (door closed) and uncovered
    (door open) values, set your threshold roughly halfway between them, and
    keep both measurements as a comment: evidence the number is not a guess.

  Student Notes:
  1. Make sure you include a flowchart of your logic
  2. Stretch goal: add a second sensor (Temp & Humidity I2C) or an OLED dashboard

  Suggested Grove ports: Light A3, Buzzer D5, LED D6
*/
const int LIGHT_SENSOR_PIN = A3;
const int LED_PIN = 6;
const int LIGHT_THRESHOLD = 200;
const unsigned long FRIDGE_OPEN_TIME_LIMIT = 10000;
const unsigned long FLASH_INTERVAL = 200;

unsigned long lightStartedAt = 0;
unsigned long lastFlashTime = 0;
bool lightIsOn = false;
bool ledState = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int lightLevel = analogRead(LIGHT_SENSOR_PIN);
  bool doorIsOpen = lightLevel > LIGHT_THRESHOLD;

  Serial.print(lightLevel);
  Serial.print(",");
  Serial.println(doorIsOpen ? 1 : 0);

  if (doorIsOpen) {
    // 1. Record the exact time the door was first opened
    if (!lightIsOn) {
      lightStartedAt = millis();
      lightIsOn = true;
    }

    // 2. Separately check if the door has been open past the limit
    if (millis() - lightStartedAt >= FRIDGE_OPEN_TIME_LIMIT) {
      if (millis() - lastFlashTime >= FLASH_INTERVAL) {
        lastFlashTime = millis();
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
      }
    } else {
      // Keep LED off while waiting for the timer to expire
      digitalWrite(LED_PIN, LOW);
    }
  } else {
    // Reset all states completely when the door is closed
    lightIsOn = false;
    ledState = LOW;
    digitalWrite(LED_PIN, LOW);
  }

  delay(50);
}
