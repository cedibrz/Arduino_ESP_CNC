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

  //moveX(POSITIVEDIRECTION, 1000, 1000);
  delay(2000);

  if (axisX1.homed == false || axisX2.homed == false) {
    Serial.println("Start Homing of X-Axis");
    homingX();
  }


}


void homingX() {
  //lower the speed
  int steps = 200;
  speed = 1500;

  //First drive a little in Positive direction in case its already homed
  Serial.println("*********************Homing X-Axis Start*********************");
  moveX(POSITIVEDIRECTION, steps, speed);  // driver one Rotation forward
  delay(500);                              // Short break

  //start the actual homing proceder
  while (buttonX1.pressed == false || buttonX2.pressed == false) {
    moveX(NEGATIVEDIRECTION, 1, speed);
    if (buttonX1.pressed) {
      Serial.println("End-Switch X1 triggered");
      break;
    } else if (buttonX2.pressed) {
      Serial.println("End-Switch X2 triggered");
      break;
    }
  }

  //************************************Synchronising************************************
  // Synchronising X1
  if (buttonX1.pressed == false) {
    Serial.println("X1 muss be synchronised");
    while (buttonX1.pressed == false) {
      moveX1(NEGATIVEDIRECTION, 1, speed);
      if (buttonX1.pressed) {
        Serial.println("End-Switch X1 triggered");
        buttonX1.pressed = false;
        break;
      }
    }
  } else {
    buttonX1.pressed = false;
  }

  // Synchronising X2
  if (buttonX2.pressed == false) {
    Serial.println("X2 muss be synchronised");
    while (buttonX2.pressed == false) {
      moveX2(NEGATIVEDIRECTION, 1, speed);
      if (buttonX2.pressed) {
        Serial.println("End-Switch X2 triggered");
        buttonX2.pressed = false;
        break;
      }
    }
  } else {
    buttonX2.pressed = false;
  }
  //************************************Synchronising************************************

  // Move to the front again
  moveX(POSITIVEDIRECTION, steps, speed);  // driver one Rotation forward
  delay(1000);               // Short break
  
  // Reset endswitches just to make sure
  buttonX1.pressed = false;
  buttonX2.pressed = false;

  // start the homing verification
  int i = 0;
  Serial.println("Homing verification");

  while (buttonX1.pressed == false || buttonX2.pressed == false) {
    moveX(NEGATIVEDIRECTION, 1, speed);
    i++;
    if (buttonX1.pressed) {
      Serial.println("Done with the fucking rotation");
      buttonX1.pressed = false;
      buttonX2.pressed = false;
      break;
    } else if (buttonX2.pressed) {
      Serial.println("Done with the fucking rotation");
      buttonX1.pressed = false;
      buttonX2.pressed = false;
      break;
    }
  }

  if (i >= (steps-Tolerance_Rotation) || i <= (steps+Tolerance_Rotation)) {
    Serial.println("_______________________");
    Serial.println("Homing Succesful");
    axisX1.homed = true;
    axisX1.rotation = 0;
    axisX2.homed = true;
    axisX1.rotation = 0;
  } else {
    Serial.println("_______________________");
    Serial.println("Homing Failed");
    axisX1.homed = false;
    axisX2.homed = false;
  }
  Serial.printf("It made %i steps but it should be %i\n", i,steps);
  Serial.println("_______________________");

  // Move to the front again
  Serial.println("Move a litte forwards V2");
  moveX(POSITIVEDIRECTION, steps, speed);  // driver one Rotation forward
  Serial.println("Pause 1s");
  delay(1000);               // Short break
  buttonX1.pressed = false;  // Reset endswitch just to make sure

  Serial.println("*********************Homing X-Axis End*********************");
}

void homingY() {
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