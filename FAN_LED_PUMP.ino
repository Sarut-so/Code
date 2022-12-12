#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_AM2315.h>

#define FAN1 D0
#define LED1 D4
#define PUMP1 D5
#define PUMP2 D6

Adafruit_AM2315 am2315;

unsigned long previousMillis = 0;        // will store last time LED was updated

unsigned long previousMillis2 = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 2000;           // interval at which to blink (milliseconds)

const long interval2 = 2000;           // interval at which to blink (milliseconds)

char auth[] = "i4SAsKfpdcSVfkf42COWlgp_EJKSN1fd";
char ssid[] = "Router_2.4GHz";// Your WiFi credentials.
char pass[] = "593321Ab";// Set password to "" for open networks.

void setup()
{
  Serial.begin(9600);
  //OUTPUT
  pinMode(LED1,OUTPUT);
  pinMode(PUMP1,OUTPUT); 
  pinMode(PUMP2,OUTPUT);  
  pinMode(FAN1,OUTPUT);
  
  Blynk.begin(auth, ssid, pass);
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
WidgetLED LED(V5);//LED
WidgetLED PUMPP(V8);//PUMP2

BLYNK_WRITE(V6){ 
  Serial.println(param.asInt()); 
    if (param.asInt()){
        digitalWrite(LED1, HIGH); 
        LED.on();
    } else {
        LED.off();
        digitalWrite(LED1, LOW); 
    }
}
BLYNK_WRITE(V7){ 
  Serial.println(param.asInt()); 
    if (param.asInt()){ 
        digitalWrite(LED1, HIGH); 
        LED.on();
    } else {
        LED.off();
        digitalWrite(LED1, LOW); 
    }
}
BLYNK_WRITE(V9){ 
  Serial.println(param.asInt()); 
    if (param.asInt()){
        digitalWrite(PUMP2, HIGH); 
        PUMPP.on();
    } else {
        PUMPP.off();
        digitalWrite(PUMP2, LOW); 
    }
}
BLYNK_WRITE(V10){ 
  Serial.println(param.asInt()); 
    if (param.asInt()){ 
        digitalWrite(PUMP2, HIGH); 
        PUMPP.on();
    } else {
       PUMPP.off();
        digitalWrite(PUMP2, LOW); 
    }
}

void loop()
{
  Blynk.run();

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

    if(temperature>=30){
      digitalWrite(FAN1,HIGH);
      Serial.print("FanON");
    }
    else if (temperature<30){
      digitalWrite(FAN1,LOW);
      Serial.print("FanOFF");
    }
    if(humidity<70){
      digitalWrite(PUMP1,HIGH);
    Serial.print("pumpON");
      }
    else if (humidity>80){
      digitalWrite(PUMP1,LOW);
      Serial.print("PumpOFF");
    }
  }

  if (currentMillis - previousMillis2 >= interval2) {
    // save the last time you blinked the LED
    previousMillis2 = currentMillis;

  }

}
