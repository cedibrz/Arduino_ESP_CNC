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

// Buttons
extern Button buttonX1;
extern Button buttonX2;
extern Button buttonY; 

/******************************************************************************
   Functions
 *****************************************************************************/
void moveX1(bool dir_Clockwise, float distance, int speedMikS);  // Moves Axis X1
void moveX2(bool dir_Clockwise, float distance, int speedMikS);  // Moves Axis X2
void moveX(bool dir_Clockwise, float distance, int speedikMS);   // Moves Axis X1 & X2
void moveY(bool dir_Clockwise, float distance, int speedikMS);   // Moves Axis Y
void randomRotation();                                           // Moves Axis X1, x2 & Y1
void example();                                                  // Example for Stepper control

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
    if (dir_Clockwise) {
      axisX1.positionMM = axisX1.positionMM + (distancePerSteps * 1.00);
      axisX1.rotation = axisX1.rotation + 1.00;
      axisX2.positionMM = axisX2.positionMM + (distancePerSteps * 1.00);
      axisX2.rotation = axisX2.rotation + 1.00;
    } else {
      axisX1.positionMM = axisX1.positionMM - (distancePerSteps * 1.00);
      axisX1.rotation = axisX1.rotation - 1.00;
      axisX2.positionMM = axisX2.positionMM - (distancePerSteps * 1.00);
      axisX2.rotation = axisX2.rotation - 1.00;
    }
    if (axisX1.positionMM >= MAXDISTANCEX || axisX1.rotation >= MAXROTATIONSX) {
      Serial.println("To Far can't move more X1");
      buttonX1.pressed = true;
      return;
    } else if (axisX2.positionMM >= MAXDISTANCEX || axisX2.rotation >= MAXROTATIONSX) {
      Serial.println("To Far can't move more X2");
      buttonX2.pressed = true;
      return;
    }

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
  @brief  Moves Axis Y

  @note   Moves Axis Y

  @param  bool dir_Clockwise, float distance, int speedMS
  @retval None
 *****************************************************************************/
void moveY(bool dir_Clockwise, float distance, int speedikMS) {
  // Configure direction
  if (dir_Clockwise) {
    digitalWrite(dirPinY, HIGH);
    axisY.positionMM = axisY.positionMM + (distancePerSteps * 1.00);
    axisY.rotation = axisY.rotation + 1.00;
  } else {
    digitalWrite(dirPinY, LOW);
    axisY.positionMM = axisY.positionMM - (distancePerSteps * 1.00);
    axisY.rotation = axisY.rotation - 1.00;
  }
  
  if (axisY.positionMM >= MAXDISTANCEY || axisY.rotation >= MAXROTATIONSY) {
    Serial.println("To Far can't move more Y");
    buttonY.pressed = true;
    return;
  } 
  
  //Run just one Step
  for (int x = 0; x < distance; x++) {
    digitalWrite(stepPinY, HIGH);
    delayMicroseconds(speedikMS);
    digitalWrite(stepPinY, LOW);
    delayMicroseconds(speedikMS);
  }
  digitalWrite(stepPinY, LOW);
}

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