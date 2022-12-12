#include <Wire.h>
#include <Adafruit_AM2315.h>



Adafruit_AM2315 am2315;

unsigned long previousMillis = 0;        // will store last time LED was updated

unsigned long previousMillis2 = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 2000;           // interval at which to blink (milliseconds)

const long interval2 = 2000;           // interval at which to blink (milliseconds)

void setup() {
  Serial.begin(115200);
  //Wire.begin (D3, D2);   // sda= GPIO_21 /scl= GPIO_22
   
  // Wake up the sensor
  Wire.beginTransmission(AM2315_I2CADDR);
  delay(2);
  Wire.endTransmission();

  while (!Serial) {
    delay(10);
  }
  Serial.println("AM2315 Test!");

  if (! am2315.begin()) {
    Serial.println("Sensor not found, check wiring & pullups!");
    while (1);
  }
}

void loop() {

  float temperature, humidity;

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;


    am2315.readTemperatureAndHumidity(&temperature, &humidity);
    //  if (! am2315.readTemperatureAndHumidity(&temperature, &humidity)) {
    //    Serial.println("Failed to read data from AM2315");
    //    return;
    //  }
    Serial.print("Temp *C: "); 
    Serial.println(temperature);
    Serial.print("Hum %: "); 
    Serial.println(humidity);

  }

  if (currentMillis - previousMillis2 >= interval2) {
    // save the last time you blinked the LED
    previousMillis2 = currentMillis;

  }


}
