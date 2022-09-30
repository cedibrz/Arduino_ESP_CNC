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
Button buttonY = { endSwitchY_1, 0, false };

// Axis
AxisData axisX1 = { false, 0.00, 0.00, POSITIVEDIRECTION };
AxisData axisX2 = { false, 0.00, 0.00, POSITIVEDIRECTION };
AxisData axisY = { false, 0.00, 0.00, POSITIVEDIRECTION };

// One Rotation
#define STEPS Full_Rotation
int speed = 1000;

// State Machine parameter
loop_state_control state = STATE_INIT;  ///< States variable

// WIFI
bool wifiAP_OK;  // Defined in "WIFI_Modul"


// Interrupt Service Routine ISR
void ARDUINO_ISR_ATTR isr(void* arg) {
  Button* s = static_cast<Button*>(arg);
  s->numberKeyPresses += 1;
  s->pressed = true;
}

// Configure Everything
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
  pinMode(buttonY.PIN, INPUT_PULLDOWN);

  // Attach Interrupts
  attachInterruptArg(buttonX1.PIN, isr, &buttonX1, RISING);
  attachInterruptArg(buttonX2.PIN, isr, &buttonX2, RISING);
  attachInterruptArg(buttonY.PIN, isr, &buttonY, RISING);
}

void loop() {
  bool done = false;
  switch (state) {
    case STATE_INIT:
      Serial.println("STATE_INIT");
      delay(1000);
      state = STATE_HOMING_X;
      break;

    case STATE_HOMING_X:
      Serial.println("STATE_HOMING_X");
      if (axisX1.homed == false || axisX2.homed == false) {
        Serial.println("Home X: Start Homing of X-Axis");
        homingX();
      } else {
        state = STATE_HOMING_Y;
      }
      break;

    case STATE_HOMING_Y:
      Serial.println("STATE_HOMING_Y");
      if (axisY.homed == false) {
        Serial.println("Home Y: Start Homing of Y-Axis");
        homingY();
      } else {
        Serial.println("Homing Done: ***********************************************************************************************");
        Serial.printf("Rotation X1: %fsteps,\t distance X1: %fmm, Rotation X2: %fsteps,\t distance X2: %fmm\n", axisX1.rotation, axisX1.positionMM, axisX2.rotation, axisX2.positionMM);
        Serial.println("Homing Done: ***********************************************************************************************");
        //state = STATE_READY;
        state = STATE_CENTER;
        speed = 2000;
      };
      break;

    case STATE_CENTER:
      Serial.println("STATE_CENTER");
      // Move to the middle
      if (axisX1.positionMM <= ((MAXDISTANCEX / 2) - Tolerance_Distance) && axisX1.positionMM <= ((MAXDISTANCEX / 2) + Tolerance_Distance)) {
        moveX(POSITIVEDIRECTION, 1, 1000);
      } else {
        done = true;
      }
      if (axisY.positionMM <= ((MAXDISTANCEY / 2) - Tolerance_Distance) && axisY.positionMM <= ((MAXDISTANCEY / 2) + Tolerance_Distance)) {
        moveY(POSITIVEDIRECTION, 1, 1000);
      } else if (done == true) {
        state = STATE_READY;
      }
      break;


    case STATE_READY:
      //Serial.println("STATE_READY");
      delay(1000);
      Serial.printf("Rotation X1: %fsteps,\t distance X1: %fmm, Rotation X2: %fsteps,\t distance X2: %fmm\n", axisX1.rotation, axisX1.positionMM, axisX2.rotation, axisX2.positionMM);
      Serial.printf("Rotation Y: %fsteps,\t distance Y: %fmm\n", axisY.rotation, axisY.positionMM);
      break;

    case STATE_OFF:
      Serial.println("STATE_OFF");
      break;
    case STATE_OFF_BUFFER:
      Serial.println("STATE_OFF_BUFFER");
      break;
  }
}


void homingX() {
  //lower the speed
  int steps = 100;
  speed = 1500;

  // First drive a little in Positive direction in case its already homed
  Serial.println("Home X: *********************Homing Y-Axis Start*********************");
  moveX(POSITIVEDIRECTION, steps, speed);  // driver one Rotation forward
  delay(500);                              // Short break


  //start the actual homing proceder
  while (buttonX1.pressed == false || buttonX2.pressed == false) {
    moveX(NEGATIVEDIRECTION, 1, speed);
    if (buttonX1.pressed) {
      Serial.println("Home X: End-Switch X1 triggered");
      break;
    } else if (buttonX2.pressed) {
      Serial.println("Home X: End-Switch X2 triggered");
      break;
    }
  }

  //************************************Synchronising************************************
  // Synchronising X1
  if (buttonX1.pressed == false) {
    Serial.println("Home X: X1 muss be synchronised");
    while (buttonX1.pressed == false) {
      moveX1(NEGATIVEDIRECTION, 1, speed);
      if (buttonX1.pressed) {
        Serial.println("Home X: End-Switch X1 triggered");
        buttonX1.pressed = false;
        break;
      }
    }
  } else {
    buttonX1.pressed = false;
  }

  // Synchronising X2
  if (buttonX2.pressed == false) {
    Serial.println("Home X: X2 muss be synchronised");
    while (buttonX2.pressed == false) {
      moveX2(NEGATIVEDIRECTION, 1, speed);
      if (buttonX2.pressed) {
        Serial.println("Home X: End-Switch X2 triggered");
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
  delay(1000);                             // Short break

  // Reset endswitches just to make sure
  buttonX1.pressed = false;
  buttonX2.pressed = false;

  // start the homing verification
  int i = 0;
  Serial.println("Home X: Homing verification");

  while (buttonX1.pressed == false || buttonX2.pressed == false) {
    moveX(NEGATIVEDIRECTION, 1, speed);
    i++;
    if (buttonX1.pressed) {
      buttonX1.pressed = false;
      buttonX2.pressed = false;
      break;
    } else if (buttonX2.pressed) {
      buttonX1.pressed = false;
      buttonX2.pressed = false;
      break;
    }
  }

  if (i >= (steps - Tolerance_Rotation) && i <= (steps + Tolerance_Rotation)) {
    Serial.println("Home X: _______________________");
    Serial.println("Home X: Homing Succesful");
    axisX1.homed = true;
    axisX1.rotation = 0;
    axisX1.positionMM = 0;
    axisX2.homed = true;
    axisX2.rotation = 0;
    axisX2.positionMM = 0;
  } else {
    Serial.println("Home X: _______________________");
    Serial.println("Home X: Homing Failed");
    axisX1.homed = false;
    axisX2.homed = false;
  }
  Serial.printf("Home X: It made %i steps but it should be %i\n", i, steps);
  Serial.println("Home X: _______________________");

  // Move to the front again
  Serial.println("Home X: Move a litte forwards V2");
  moveX(POSITIVEDIRECTION, 100, speed);  // driver one Rotation forward
  Serial.println("Home X: Pause 1s");
  delay(1000);  // Short break

  // Reset endswitch just to make sure
  buttonX1.pressed = false;
  buttonX2.pressed = false;

  Serial.println("Home X: *********************Homing X-Axis End*********************");
}

void homingY() {
  //lower the speed
  int steps = 100;
  speed = 1500;

  //First drive a little in Positive direction in case its already homed
  Serial.println("Home Y: *********************Homing Y-Axis Start*********************");
  moveY(POSITIVEDIRECTION, steps, speed);  // driver one Rotation forward
  delay(500);                              // Short break

  //start the actual homing proceder
  while (buttonY.pressed == false) {
    moveY(NEGATIVEDIRECTION, 1, speed);
    if (buttonY.pressed) {
      Serial.println("Home Y: End-Switch Y triggered");
      break;
    }
  }

  // Move to the front again
  moveY(POSITIVEDIRECTION, steps, speed);  // driver one Rotation forward
  delay(1000);                             // Short break

  // Reset endswitches just to make sure
  buttonY.pressed = false;

  // start the homing verification
  int i = 0;
  Serial.println("Home Y: Homing verification");

  while (buttonY.pressed == false) {
    moveY(NEGATIVEDIRECTION, 1, speed);
    i++;
    if (buttonY.pressed) {
      buttonY.pressed = false;
      break;
    }
  }

  if (i >= (steps - Tolerance_Rotation) && i <= (steps + Tolerance_Rotation)) {
    Serial.println("Home Y: _______________________");
    Serial.println("Home Y: Homing Succesful");
    axisY.homed = true;
    axisY.rotation = 0;
    axisY.positionMM = 0;
  } else {
    Serial.println("_______________________");
    Serial.println("Home Y: Homing Failed");
    axisY.homed = false;
  }
  Serial.printf("Home Y: It made %i steps but it should be %i\n", i, steps);
  Serial.println("Home Y: _______________________");

  // Move to the front again
  Serial.println("Home Y: Move a litte forwards V2");
  moveY(POSITIVEDIRECTION, 100, speed);  // driver one Rotation forward
  Serial.println("Home Y: Pause 1s");
  delay(1000);  // Short break

  // Reset endswitch just to make sure
  buttonY.pressed = false;

  Serial.println("Home Y: *********************Homing Y-Axis End*********************");
}





















void debugEndSwitches(void) {
  Serial.print("endSwitchX_1: ");
  Serial.println(digitalRead(buttonX1.PIN));
  Serial.print("endSwitchY: ");
  Serial.println(digitalRead(buttonY.PIN));
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
    detachInterrupt(buttonY.PIN);
  }
}