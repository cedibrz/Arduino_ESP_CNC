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
#include <string.h>

#include "FS.h"  // FFPS
#include "SD.h"
#include "SPI.h"
#include "constants.h"
#include "File_Handler.h"

/******************************************************************************
   Variables
 *****************************************************************************/
// File Variable
String filename = "/Spirale.csv";  ///< File Name

SPIClass spi = SPIClass(VSPI);

extern float Data[3500][2];

/******************************************************************************
   Functions
 *****************************************************************************/
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
void createDir(fs::FS &fs, const char *path);
void removeDir(fs::FS &fs, const char *path);
void readFile(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);

/** ***************************************************************************
  @brief  Initialization of the SPIFFS

  @note   Initialization of the SPIFFS
  @n      SPIFFS: SPI Flash-File-System

  @param  None
  @retval None
*****************************************************************************/
void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char *path) {
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path)) {
    Serial.println("Dir created");
  } else {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char *path) {
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path)) {
    Serial.println("Dir removed");
  } else {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char *path) {
  int fileSize = 0;
  String bufferString;
  char bufferChar[20];
  char *split;
  float x,y;

  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.println("Read from file: ");
  while (file.available()) {
    //Serial.write(file.read());
    bufferString = file.readStringUntil('\n');
    Serial.println(bufferString);
    bufferString.toCharArray(bufferChar, 20);
    split = strtok(bufferChar,";");
    x = String(split).toFloat();
    split = strtok(NULL,";");
    y = String(split).toFloat();
    Serial.printf("X Value: %f und Y-Value: %f\n",x,y);
    Data[fileSize][0] = x;
    Data[fileSize][1] = y;
    fileSize++;
  }
  Serial.printf("File groesse: %d \n",fileSize);
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message) {
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2) {
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2)) {
    Serial.println("File renamed");
  } else {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void setupSD() {
  spi.begin(SCK, MISO, MOSI, CS);

  if (!SD.begin(CS, spi, 4000000)) {
    Serial.println("Card Mount Failed");
    return;
  }

  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.println("MMC");
  } else if (cardType == CARD_SD) {
    Serial.println("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}