/** ***************************************************************************
  @file   Stepper_Handler.cpp
  @brief  Stepper_Handler

  @note   Provides functions to control the steppers

  @author Kindler Cedric
  @date   30.09.2022
 *****************************************************************************/

/******************************************************************************
   Includes
 *****************************************************************************/
// Public Library's
#include <Arduino.h>

// Local Moduls
#include "Stepper_Handler.h"

/******************************************************************************
   Variables
 *****************************************************************************/
 // Axis
extern AxisData axisX1;
extern AxisData axisX2;
extern AxisData axisY;

/******************************************************************************
   Functions
 *****************************************************************************/
void moveX1(bool dir_Clockwise, float distance, int speedMikS);  // Moves Axis X1
void moveX2(bool dir_Clockwise, float distance, int speedMikS);  // Moves Axis X2
void moveX(bool dir_Clockwise, float distance, int speedikMS);   // Moves Axis X1 & X2
void randomRotation();                            // Moves Axis X1, x2 & Y1
void example();                                   // Example for Stepper control

/** ***************************************************************************
  @brief  Moves Axis X1

  @note   Moves Axis X1

  @param  bool dir_Clockwise, float distance, int speedMS
  @retval None
 *****************************************************************************/
void moveX1(bool dir_Clockwise, float distance, int speedikMS) {
  // Configure direction
  if (dir_Clockwise) {
    digitalWrite(dirPinX_1, HIGH);
  } else {
    digitalWrite(dirPinX_1, LOW);
  }
  //Run just one Step
  for (int x = 0; x < distance; x++) {
    digitalWrite(stepPinX_1, HIGH);
    delayMicroseconds(speedikMS);
    digitalWrite(stepPinX_1, LOW);
    delayMicroseconds(speedikMS);
  }
  digitalWrite(stepPinX_1, LOW);
}

/** ***************************************************************************
  @brief  Moves Axis X2

  @note   Moves Axis X2

  @param  bool dir_Clockwise, float distance, int speedMS
  @retval None
 *****************************************************************************/
void moveX2(bool dir_Clockwise, float distance, int speedikMS) {
  // Configure direction
  if (dir_Clockwise) {
    digitalWrite(dirPinX_2, HIGH);
  } else {
    digitalWrite(dirPinX_2, LOW);
  }
  //Run just one Step
  for (int x = 0; x < distance; x++) {
    digitalWrite(stepPinX_2, HIGH);
    delayMicroseconds(speedikMS);
    digitalWrite(stepPinX_2, LOW);
    delayMicroseconds(speedikMS);
  }
  digitalWrite(stepPinX_2, LOW);
}

/** ***************************************************************************
  @brief  Moves Axis X1 & X2

  @note   Moves Axis X1 & X3

  @param  bool dir_Clockwise, float distance, int speedMS
  @retval None
 *****************************************************************************/
void moveX(bool dir_Clockwise, float distance, int speedikMS) {
  // Configure direction
  if (dir_Clockwise) {
    digitalWrite(dirPinX_1, HIGH);
    digitalWrite(dirPinX_2, HIGH);
  } else {
    digitalWrite(dirPinX_1, LOW);
    digitalWrite(dirPinX_2, LOW);
  }
  //Run just one Step
  for (int x = 0; x < distance; x++) {
    digitalWrite(stepPinX_1, HIGH);
    digitalWrite(stepPinX_2, HIGH);
    delayMicroseconds(speedikMS);
    digitalWrite(stepPinX_1, LOW);
    digitalWrite(stepPinX_2, LOW);
    delayMicroseconds(speedikMS);
  }
  digitalWrite(stepPinX_1, LOW);
  digitalWrite(stepPinX_2, LOW);
}

/** ***************************************************************************
  @brief  Moves Axis X1, x2 & Y1

  @note   Moves Axis X1, x2 & Y1

  @param  bool dir_Clockwise, float distance
  @retval None
 *****************************************************************************/
void randomRotation() {
  digitalWrite(stepPinX_1, HIGH);
  digitalWrite(stepPinY, HIGH);
  digitalWrite(stepPinX_2, HIGH);

  delayMicroseconds(1000);

  digitalWrite(stepPinX_1, LOW);
  digitalWrite(stepPinY, LOW);
  digitalWrite(stepPinX_2, LOW);
}

/** ***************************************************************************
  @brief  Example for Stepper control

  @note   Can be deleted on a later point

  @param  None
  @retval None
 *****************************************************************************/
void example() {
  digitalWrite(dirPinX_1, HIGH);  // Enables the motor to move in a particular direction
  digitalWrite(dirPinY, HIGH);    // Enables the motor to move in a particular direction
  digitalWrite(dirPinX_2, HIGH);  // Enables the motor to move in a particular direction
  // Makes 200 pulses for making one full cycle rotation
  for (int x = 0; x < 400; x++) {
    digitalWrite(stepPinX_1, HIGH);
    digitalWrite(stepPinY, HIGH);
    digitalWrite(stepPinX_2, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPinX_1, LOW);
    digitalWrite(stepPinY, LOW);
    digitalWrite(stepPinX_2, LOW);
    delayMicroseconds(1000);
  }
  delay(1000);  // One second delay
  Serial.println("Lauft");
  delay(1000);
}
