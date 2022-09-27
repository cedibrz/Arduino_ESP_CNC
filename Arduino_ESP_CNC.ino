// Local Library's
#include "WIFI_Handler.h"
#include "Stepper_Handler.h"

// Public Library's
#include <Arduino.h>
#include <WiFi.h>

// NEW Comment to test GIT

// Drehrichtung
bool dirX1 = NEGATIVEDIRECTION;
bool dirX2 = NEGATIVEDIRECTION;
bool dirY = NEGATIVEDIRECTION;

// End-Switches
Button buttonX1 = { endSwitchX_1, 0, false };
Button buttonX2 = { endSwitchX_2, 0, false };
Button buttonY1 = { endSwitchY_1, 0, false };

// Axis
AxisData axisX1 = { false, 0.00, 0.00, POSITIVEDIRECTION };
AxisData axisX2 = { false, 0.00, 0.00, POSITIVEDIRECTION };
AxisData axisY = { false, 0.00, 0.00, POSITIVEDIRECTION };

// One Rotation
#define STEPS Full_Rotation
int speed = 1000;

// WIFI
bool wifiAP_OK;  // Defined in "WIFI_Modul"
//WiFiServer server(80);

// Interrupt Service Routine ISR
void ARDUINO_ISR_ATTR isr(void* arg) {
  Button* s = static_cast<Button*>(arg);
  s->numberKeyPresses += 1;
  s->pressed = true;
}

void setup() {
  // Start Serial Monitor (Debugging)
  Serial.begin(115200);
  Serial.println("Welcom Bitch");

  // Connect to WIFI
  connect_WIFI();

  // Configure Outputs (Stepper)
  pinMode(stepPinX_1, OUTPUT);
  pinMode(dirPinX_1, OUTPUT);
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  pinMode(stepPinX_2, OUTPUT);
  pinMode(dirPinX_2, OUTPUT);

  // Configure Inputs (Stepper)
  pinMode(buttonX1.PIN, INPUT_PULLDOWN);
  pinMode(buttonX2.PIN, INPUT_PULLDOWN);
  pinMode(buttonY1.PIN, INPUT_PULLDOWN);

  // Attach Interrupts
  attachInterruptArg(buttonX1.PIN, isr, &buttonX1, RISING);
  attachInterruptArg(buttonX2.PIN, isr, &buttonX2, RISING);
  attachInterruptArg(buttonY1.PIN, isr, &buttonY1, RISING);
}

void loop() {
  //randomRotation();
  //moveX(0,200);
  //uint i = 0;

  moveX(POSITIVEDIRECTION, 1000, 1000);
  delay(2000);

  if (axisX1.homed == false || axisX2.homed == false) {
    Serial.println("Start Homing of X-Axis");
    homingX();
  }

  /*
  while (buttonX1.pressed == false) {
    moveX1(dirX1, 1, speed);
    if (buttonX1.pressed) {
      Serial.println("Done with the fucking rotation");
      buttonX1.pressed = false;
      dirX1 = !dirX1;
    }
  }
  */

  // Interrupt Routines
  /*
  if (buttonX1.pressed) {
    Serial.printf("Button X1 has been pressed %u times\n", buttonX1.numberKeyPresses);
    buttonX1.pressed = false;
  }
  
  if (buttonX2.pressed) {
    Serial.printf("Button X2 has been pressed %u times\n", buttonX2.numberKeyPresses);
    buttonX2.pressed = false;
  }
  if (buttonY1.pressed) {
    Serial.printf("Button Y1 has been pressed %u times\n", buttonY1.numberKeyPresses);
    buttonY1.pressed = false;
  }
  */
}


void homingX() {
  //lower the speed
  int steps = 400;
  speed = 1000;
  //First drive a little in Positive direction in case its already homed
  Serial.println("Move a litte forwards");
  moveX(POSITIVEDIRECTION, steps, speed);  // driver one Rotation forward
  Serial.println("Pause 1s");
  delay(500);  // Short break

  //start the actual homing proceder
  Serial.println("Move Back till Home");
  while (buttonX1.pressed == false) {
    moveX(NEGATIVEDIRECTION, 1, speed);
    if (buttonX1.pressed) {
      Serial.println("Done with the fucking rotation");
      buttonX1.pressed = false;
      break;
    }
  }

  // Move to the front again
  Serial.println("Move a litte forwards V2");
  moveX(POSITIVEDIRECTION, steps, speed);  // driver one Rotation forward
  Serial.println("Pause 1s");
  delay(1000);  // Short break
  buttonX1.pressed = false; // Reset endswitch just to make sure

  // start the homing verification
  int i = 0;
  Serial.println("Homing verification");

  while (buttonX1.pressed == false) {
    moveX(NEGATIVEDIRECTION, 1, speed);
    i++;
    if (buttonX1.pressed) {
      Serial.println("Done with the fucking rotation");
      buttonX1.pressed = false;
      break;
    }
  }
  if (i == steps) {
    Serial.println("*******************");
    Serial.println("Homing Succesful");
    Serial.println("*******************");
    axisX1.homed = true;
    axisX1.rotation = 0;
    axisX2.homed = true;
    axisX1.rotation = 0;
  } else {
    Serial.println("*******************");
    Serial.println("Homing Failed");
    Serial.println("*******************");
    axisX1.homed = false;
    axisX2.homed = false;
  }
  Serial.printf("It made %i steps but it should be 400", i);

  // Move to the front again
  Serial.println("Move a litte forwards V2");
  moveX(POSITIVEDIRECTION, steps, speed);  // driver one Rotation forward
  Serial.println("Pause 1s");
  delay(1000);  // Short break
  buttonX1.pressed = false; // Reset endswitch just to make sure

  Serial.println("End Homing of X-Axis");
}

void homingY() {
  //lower the speed
  speed = 1000;
  //First drive a little in Positive direction in case its already homed
  moveX(POSITIVEDIRECTION, 200, speed);  // driver one Rotation forward

  delay(500);  // Short break

  //start the actual homing proceder
  while (buttonX1.pressed == false) {
    moveX(dirX1, 1, speed);
    if (buttonX1.pressed) {
      Serial.println("Done with the fucking rotation");
      buttonX1.pressed = false;
      break;
    }
  }
}





















void debugEndSwitches(void) {
  Serial.print("endSwitchX_1: ");
  Serial.println(digitalRead(buttonX1.PIN));
  Serial.print("endSwitchY: ");
  Serial.println(digitalRead(buttonY1.PIN));
  Serial.print("endSwitchX_2: ");
  Serial.println(digitalRead(buttonX2.PIN));
  Serial.println("****************************");
  delay(1000);
}


void BufferDetachInterrupt(void) {
  static uint32_t lastMillis = 0;
  if (millis() - lastMillis > 10000) {
    lastMillis = millis();
    Serial.println("Interrupts detach!!!");
    detachInterrupt(buttonX1.PIN);
    detachInterrupt(buttonX2.PIN);
    detachInterrupt(buttonY1.PIN);
  }
}
