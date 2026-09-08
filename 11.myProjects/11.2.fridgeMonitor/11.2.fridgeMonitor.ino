
const int LIGHT_PIN = A3;
const int LED_PIN = 6;
const int BUZZER_PIN = 5;

const int DOOR_OPEN_THRESHOLD = 2; //If light sensor detects this level of light or higher, the door state will be considered open
const unsigned long DOOR_TIMEOUT = 10000; //If the door is open for 10 seconds
const unsigned long FLASH_INTERVAL = 250; //Intervals between the LED flashing in milliseconds
const unsigned long PLOT_INTERVAL = 50;  //How often the data is plotted/checked

unsigned long doorOpenStartTime = 0;
bool doorWasOpenLastCheck = false; //Setting up the light and alarm to not start immediately
bool alarmActive = false;

unsigned long lastFlashTime = 0;
unsigned long lastPlotTime = 0;
bool ledState = LOW; //Making sure the light does not start before the door is considered open

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER_PIN, OUTPUT); //Supply electricity to the buzzer and LED pins
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); //Turn them off at the start of the program
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  unsigned long currentMillis = millis();
  int lightLevel = analogRead(LIGHT_PIN); //Convert voltage into a digital number, int lightLevel stores that data
  bool isDoorOpen = (lightLevel > DOOR_OPEN_THRESHOLD); //The door being open is decided by if the door open threshold is lower than the light level

  if (isDoorOpen) {
    if (!doorWasOpenLastCheck) {
      doorOpenStartTime = currentMillis;
      doorWasOpenLastCheck = true;
    }

    if (currentMillis - doorOpenStartTime >= DOOR_TIMEOUT) {
      alarmActive = true;
    }
  } else {
    doorWasOpenLastCheck = false;
    alarmActive = false; //If the door is open but hasn't been open for ten seconds, the alarm will not activate
    digitalWrite(BUZZER_PIN, LOW); //Making the buzzer and led turn off
    digitalWrite(LED_PIN, LOW);
  }

  if (alarmActive) {
    digitalWrite(BUZZER_PIN, HIGH); //Turning the buzzer on if the door is open for 10 seconds or more

    if (currentMillis - lastFlashTime >= FLASH_INTERVAL) {
      lastFlashTime = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }

  if (currentMillis - lastPlotTime >= PLOT_INTERVAL) {
    lastPlotTime = currentMillis;
    Serial.print("LightLevel:");
    Serial.print(lightLevel);
    Serial.print(",");
    Serial.print("Threshold:");
    Serial.print(DOOR_OPEN_THRESHOLD);
    Serial.print(",");
    Serial.print("DoorState:");
    Serial.println(isDoorOpen ? 500 : 0);
  }
}








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
//Value of door open is 5 while value of door closed is 0, door open threshold will be 2
