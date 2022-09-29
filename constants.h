/** ***************************************************************************
   @file  constants.h
   @brief constants

  @note Provides constants for pin numbars

   Prefix constants

   @bugs not testet yet

   @author  Cedric Kindler
   @date  	15.09.2022
 *****************************************************************************/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_


/******************************************************************************
   Includes
 *****************************************************************************/
#include <Arduino.h>

/******************************************************************************
   Defines
 *****************************************************************************/
// End-Switch Motor X1
const int stepPinX_1 = 17;    //X.STEP
const int dirPinX_1 = 19;     // X.DIR
const int endSwitchX_1 = 25;  // End Switch

// End-Switch Motor Y
const int stepPinY = 16;      //Y.STEP
const int dirPinY = 18;       // Y.DIR
const int endSwitchY_1 = 26;  // End Switch

// End-Switch Motor X2
const int stepPinX_2 = 4;     //Z.STEP
const int dirPinX_2 = 5;      // Z.DIR
const int endSwitchX_2 = 27;  // End Switch

// Schritt Winkel
const float StepAngle = 1.8;  //In degrees
const float Rotation = 360;
const float Full_Rotation = Rotation / StepAngle;  // Number of degrees for one Rotation
const int Tolerance_Rotation = 3;

// Distance travelled
const float lead = 8;  // mm for full rotation
const float distancePerSteps = lead / Full_Rotation; // mm/steps

// Maximal Distance
#define MAXDISTANCEX 260
#define MAXROTATIONSX 6400

// Struct für ISR Button
struct Button {
  const uint8_t PIN;
  uint32_t numberKeyPresses;
  bool pressed;
};

// Struct for Position Handling
struct AxisData {
  bool homed;
  float positionMM;
  float rotation;
  bool direction;
};

// State machine parameters
typedef enum {
  STATE_INIT,
  STATE_HOMING_X,
  STATE_HOMING_Y,
  STATE_READY,
  STATE_OFF
} loop_state_control;  ///< States of the main state machine


/******************************************************************************
   Functions
 *****************************************************************************/

#endif