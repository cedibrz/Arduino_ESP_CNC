/** ***************************************************************************
  @file
  @brief  See File_Reader.cpp

  Prefix  FILE_READER

*****************************************************************************/
#ifndef FILE_READER_H_
#define FILE_READER_H_

/******************************************************************************
   Functions
 *****************************************************************************/
void FILE_init_SPIFFS();  // Initialization of the SPIFFS
void FILE_Read();         // Writes data to .csv file

#endif
