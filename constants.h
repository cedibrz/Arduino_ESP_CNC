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
const int endSwitchX_1 = 12;  // End Switch

// End-Switch Motor Y
const int stepPinY = 16;      //Y.STEP
const int dirPinY = 18;       // Y.DIR
const int endSwitchY_1 = 14;  // End Switch

// End-Switch Motor X2
const int stepPinX_2 = 4;     //Z.STEP
const int dirPinX_2 = 5;      // Z.DIR
const int endSwitchX_2 = 13;  // End Switch

// SPI Pins


// Schritt Winkel
const float StepAngle = 1.8;  //In degrees
const float Rotation = 360;
const float Full_Rotation = Rotation / StepAngle;  // Number of degrees for one Rotation
const int Tolerance_Rotation = 3;
const float Tolerance_Distance = 1;

// Distance travelled
const float lead = 8;  // mm for full rotation
const float distancePerSteps = lead / Full_Rotation; // mm/steps

// SPI Constants for SD-Module
#define SCK 25   // 14 oder 18
#define MISO 26  // 12 oder 19
#define MOSI 32  // 13 oder 23
#define CS 23     // 15 oder 5

// Indizes for array spalten
#define XDIST 0
#define YDIST 1
#define XROT 2
#define YROT 3
#define XDIR 4
#define YDIR 5

// Maximal Distance
#define MAXDISTANCEX 260
#define MAXROTATIONSX 6400

#define MAXDISTANCEY 260
#define MAXROTATIONSY 6400

#define POSITIVEDIRECTION 1
#define NEGATIVEDIRECTION 0

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
  STATE_CENTER,
  STATE_OFF,
  STATE_OFF_BUFFER,
  STATE_FIGURE
} loop_state_control;  ///< States of the main state machine


/******************************************************************************
   Functions
 *****************************************************************************/

#endif