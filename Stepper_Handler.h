/** ***************************************************************************
  @file
  @brief  See Stepper_Handler.cpp

  Prefix  WIFI_HANDLER

 *****************************************************************************/
#ifndef STEPPER_HANDLER_H_
#define STEPPER_HANDLER_H_

// Local Moduls
#include "constants.h"
/******************************************************************************
   Defines
 *****************************************************************************/
// Stepper Motor X1
extern const int stepPinX_1; //X.STEP
extern const int dirPinX_1; // X.DIR

// Stepper Motor Y
extern const int stepPinY; //Y.STEP
extern const int dirPinY; // Y.DIR

// Stepper Motor X2
extern const int stepPinX_2; //Z.STEP
extern const int dirPinX_2; // Z.DIR

#define POSITIVEDIRECTION 1
#define NEGATIVEDIRECTION 0

/******************************************************************************
   Functions
 *****************************************************************************/
void moveX1(bool dir_Clockwise, float distance, int speedMikS);  // Moves Axis X1
void moveX2(bool dir_Clockwise, float distance, int speedMikS);  // Moves Axis X2
void moveX(bool dir_Clockwise, float distance, int speedikMS);   // Moves Axis X1 & X2
void moveY(bool dir_Clockwise, float distance, int speedikMS);  // Moves Axis Y
void randomRotation();                            // Moves Axis X1, x2 & Y1
void example();                                   // Example for Stepper control
#endif
