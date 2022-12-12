/*

  Example of BH1750 library usage.

  This example initialises the BH1750 object using the default high resolution
  continuous mode and then makes a light level reading every second.

  Connection:

    VCC -> 3V3 or 5V
    GND -> GND
    SCL -> SCL (A5 on Arduino Uno, Leonardo, etc or 21 on Mega and Due, on esp8266 free selectable)
    SDA -> SDA (A4 on Arduino Uno, Leonardo, etc or 20 on Mega and Due, on esp8266 free selectable)
    ADD -> (not connected) or GND

  ADD pin is used to set sensor I2C address. If it has voltage greater or equal to
  0.7VCC voltage (e.g. you've connected it to VCC) the sensor address will be
  0x5C. In other case (if ADD voltage less than 0.7 * VCC) the sensor address will
  be 0x23 (by default).

*/
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;
char auth[] = "i4SAsKfpdcSVfkf42COWlgp_EJKSN1fd";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Router_2.4GHz";
char pass[] = "593321Ab";

// Use Virtual pin  for display values
#define PIN_LUX V2
float lux;

BLYNK_READ(PIN_LUX)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_LUX, lux);
  Serial.println(lux);
}

void setup(){

  Serial.begin(9600);
   Blynk.begin(auth, ssid, pass);
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);

  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);

  Serial.println(F("BH1750 Test begin"));

}


void loop() {
   Blynk.run();
  lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
 

}
