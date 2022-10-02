/** ***************************************************************************
  @file
  @brief    File_Reader

  @note     Initialization of the SPIFFS.
  @n        Provides function to read from SPIFFS Files.

  @author   Kindler Cedric
  @date	    02.10.2022
*****************************************************************************/

/******************************************************************************
   Includes
 *****************************************************************************/
// Public Library's
#include "FS.h"        // FFPS
#include "SPIFFS.h"

/******************************************************************************
   Variables
 *****************************************************************************/
// File Variable
String filename = "Spirale";  ///< File Name


/******************************************************************************
   Functions
 *****************************************************************************/
void FILE_init_SPIFFS();  // Initialization of the SPIFFS
void FILE_Read();         // Writes data to .csv file

/** ***************************************************************************
  @brief  Initialization of the SPIFFS

  @note   Initialization of the SPIFFS
  @n      SPIFFS: SPI Flash-File-System

  @param  None
  @retval None
*****************************************************************************/
void FILE_init_SPIFFS() {
  // Initialisierung SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("DEBUG: Fehler bei Initialisierung von SPIFFS");
    return;
  }
  return;
}

/** ***************************************************************************
  @brief  Read data from .csv file

  @note   Reads all data from a .csv file.
  @n      Starts reading at the top.

  @param  None
  @retval None
*****************************************************************************/
void FILE_Read() {
  String bufferString;
  static int position = 0;
  int i = 0;

  File file = SPIFFS.open("/" + filename + ".csv", "r");
  if (!file) {
    Serial.println("Failed to open File");
  } else {
    Serial.println("Filed opened Succesfull");
  }

  while (i != position) {
    bufferString = file.readStringUntil('\n');
    i++;
  }
  position ++;
  Serial.println(bufferString);
  
  file.close();
  return;
}