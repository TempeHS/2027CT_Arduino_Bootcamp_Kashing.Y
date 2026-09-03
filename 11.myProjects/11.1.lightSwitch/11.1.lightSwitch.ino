/*
  Author:
  Learning Intention: Students will use a range of simple sensors to control an LED UI

  Student Notes:
  1. Make sure you include a copy of your schematic
  2. Students should use either the button or potentiometer in an unconventional way

  3. Suggested Grove ports: Button D4, Potentiometer A0, LED D6, Light sensor A3
*/
const int BUTTON_PIN = 4;
const int LED_PIN = 6;

unsigned long pressedTime = 0; //Unsigned means that the value cannot be negative, making the maximum of long twice as large
bool pressed = false; //Bool is an option of either true or false
bool lightOn = false;

void setup() {
  pinMode(BUTTON_PIN, INPUT); //Input allows the button to receive the electronic signal
  pinMode(LED_PIN, OUTPUT); //Output releases an electronic signal
  digitalWrite(LED_PIN, LOW);
  Serial.begin(115200);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == HIGH) { //If the button is pressed, this code will subtract 2 seconds from the time the button was originally pressed until the pressed time is equal to or greater than 2000 milliseconds (2 seconds)
    if (pressed == false) {
      pressed = true;
      pressedTime = millis();
    } else {
      if ((millis() - pressedTime >= 2000) && !lightOn) { //If this happens, the light wil turn on
        digitalWrite(LED_PIN, HIGH);
        lightOn = true;
      }
    }
  } else {
    pressed = false; //If the button is unpressed, the light will quickly turn back off
    digitalWrite(LED_PIN, LOW);
    lightOn = false;
  }
}
