// Move your helper functions into this tab.
// The Arduino build system automatically joins every .ino file in this folder
// into one program, so functions defined here can be called from the main sketch.
#include "Ultrasonic.h"

Ultrasonic ultrasonic(2);

int readDistance() {
  return ultrasonic.read();
}
int classifyZone(int distance, int nearLimit, int farLimit) {
  if (distance < nearLimit) {
    return 0;              // danger
  } else if (distance < farLimit) {
    return 1;              // warning
  }
  return 2;                // safe
}
