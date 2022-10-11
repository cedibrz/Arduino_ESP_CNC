/** ***************************************************************************
  @file
  @brief  See File_Handler.cpp

  Prefix  FILE_HANDLER

*****************************************************************************/
#ifndef FILE_HANDLER_H_
#define FILE_HANDLER_H_

#include "FS.h"  // FFPS
#include "SD.h"
#include "SPI.h"

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
void setupSD();
#endif
