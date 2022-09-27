/** ***************************************************************************
  @file   WIFI_Handler.cpp
  @brief  WIFI_Handler

  @note   Provides functions for connecting to WIFI

  @author Kindler Cedric
  @date   30.09.2022
 *****************************************************************************/

/******************************************************************************
   Includes
 *****************************************************************************/
// Public Library's
#include <WiFi.h>

// Local Moduls
#include "WIFI_Handler.h"

/******************************************************************************
   Variables
 *****************************************************************************/
// SSID and Password to your ESP Access Point
const char* ssid = "ASUS-Verstaerker";        ///< Name of Accesspoint
const char* password = "cedric.397";  ///< Password of Accesspoint
extern bool wifiAP_OK;                // Defined in "WIFI_Modul"

WiFiServer server(80);

/******************************************************************************
   Functions
 *****************************************************************************/
void connect_WIFI();  // Create Wifi AccessPoint

/** ***************************************************************************
  @brief  Create Wifi AccessPoint

  @note   Creates a Wifi AccessPoint where you can connect to.
  @n      Name:     BA22_Nusi02
  @n      Password: 12345678

  @param  None
  @retval None
 *****************************************************************************/
void connect_WIFI() {
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  return;
}