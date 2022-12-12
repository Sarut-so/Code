#include <Wire.h>
#include <BH1750.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_AM2315.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define BLYNK_PRINT Serial
#define SensorPin A0              //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection

#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
#define BLYNK_PRINT Serial
const int echoPin = D6;
const int trigPin = D5;

#define FAN1 D0
#define LED1 D4
#define PUMP1 D3
#define PUMP2 D7

char auth[] = "i4SAsKfpdcSVfkf42COWlgp_EJKSN1fd";
char ssid[] = "Router_2.4GHz";// Your WiFi credentials.
char pass[] = "593321Ab";// Set password to "" for open networks.

Adafruit_AM2315 am2315;
BH1750 lightMeter;

// Use Virtual pin  for display values
#define PIN_TEMP V0
#define PIN_HUMI V1
#define PIN_LUX V2
#define PIN_HC_RS04 V3
#define PIN_PH V4

long duration;
float distanceCm;
float temperature, humidity;
float lux;
float ph;

unsigned long previousMillis = 0;        // will store last time LED was updated

unsigned long previousMillis2 = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 2000;           // interval at which to blink (milliseconds)

const long interval2 = 2000;           // interval at which to blink (milliseconds)
// This function tells Arduino what to do if there is a Widget
// which is requesting data for Virtual Pin (5)
BLYNK_READ(PIN_TEMP)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_TEMP, temperature);
  Serial.println(temperature);
}

BLYNK_READ(PIN_HUMI)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_HUMI, humidity);
  Serial.println(humidity);
}
BLYNK_READ(PIN_LUX)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_LUX, lux);
  Serial.println(lux);
}

BLYNK_READ(PIN_PH)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_PH, ph);
  Serial.println(ph);
}
BLYNK_READ(PIN_HC_RS04)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_HC_RS04, distanceCm);
  Serial.println(distanceCm);
}
int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex = 0;

void setup() {
  pinMode(LED1, OUTPUT);
  pinMode(PUMP1, OUTPUT);
  pinMode(PUMP2, OUTPUT);
  pinMode(FAN1, OUTPUT);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.println("pH meter experiment!");    //Test the serial monitor
  Serial.println();
  //Serial.println("lux(lx)");
  Blynk.begin(auth, ssid, pass);
  lcd.begin();
  lcd.backlight();

  // Initialize the I2C bus (BH1750 library doesn't do this automatically)
  Wire.begin();
  // On esp8266 you can select SCL and SDA pins using Wire.begin(D4, D3);
  // For Wemos / Lolin D1 Mini Pro and the Ambient Light shield use Wire.begin(D2, D1);

  lightMeter.begin();

  Serial.println(F("BH1750 Test begin"));

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
  lightMeter.begin();
}
WidgetLED LED(V5);//LED
WidgetLED PUMPP(V8);//PUMP2

BLYNK_WRITE(V6) {
  Serial.println(param.asInt());
  if (param.asInt()) {
    digitalWrite(LED1, HIGH);
    LED.on();
  } else {
    LED.off();
    digitalWrite(LED1, LOW);
  }
}
BLYNK_WRITE(V7) {
  Serial.println(param.asInt());
  if (param.asInt()) {
    digitalWrite(LED1, HIGH);
    LED.on();
  } else {
    LED.off();
    digitalWrite(LED1, LOW);
  }
}
BLYNK_WRITE(V9) {
  Serial.println(param.asInt());
  if (param.asInt()) {
    digitalWrite(PUMP2, HIGH);
    PUMPP.on();
  } else {
    PUMPP.off();
    digitalWrite(PUMP2, LOW);
  }
}
BLYNK_WRITE(V10) {
  Serial.println(param.asInt());
  if (param.asInt()) {
    digitalWrite(PUMP2, HIGH);
    PUMPP.on();
  } else {
    PUMPP.off();
    digitalWrite(PUMP2, LOW);
  }
}

void loop() {
  Blynk.run();
  lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY / 2;


  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    am2315.readTemperatureAndHumidity(&temperature, &humidity);
  }
  //Serial.print("Temp *C: "); Serial.println(temperature);
  //Serial.print("Hum %: "); Serial.println(humidity);
  //Auto Mode
   if(temperature>=31){
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
   
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float voltage;
  if (millis() - samplingTime > samplingInterval)
  {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 5  / 1024;
    ph = 3.5 * voltage + Offset;
    samplingTime = millis();
  }
  if (millis() - printTime > printInterval)  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(ph, 4);

    lcd.setCursor(0, 0);
    lcd.print("Temp:    ");
    lcd.setCursor(5, 0);
    lcd.print(temperature);
    lcd.setCursor(11, 0);
    lcd.print("C");

    lcd.setCursor(0, 1);
    lcd.print("Humi:    ");
    lcd.setCursor(5, 1);
    lcd.print(humidity);
    lcd.setCursor(11, 1);
    lcd.print("%RH");



    Serial.print("Light: ");
    //Serial.print(lux);
    Serial.println(" lx");
    lcd.setCursor(0, 2);
    lcd.print("Lux:     ");
    lcd.setCursor(4, 2);
    lcd.print(lux);
    lcd.setCursor(11, 2);
    lcd.print("lx");

    lcd.setCursor(0, 3);
    lcd.print("ph:     ");
    lcd.setCursor(3, 3);
    lcd.print(ph);
    lcd.setCursor(8, 3);
    lcd.print("pH");

    lcd.setCursor(11, 3);
    lcd.print("T:     ");
    lcd.setCursor(13, 3);
    lcd.print(distanceCm);
    lcd.setCursor(18, 3);
    lcd.print("cm");


    printTime = millis();
  }
}
double avergearray(int* arr, int number) {
  int i;
  int max, min;
  double avg;
  long amount = 0;
  if (number <= 0) {
    Serial.println("Error number for the array to avraging!/n");
    return 0;
  }
  if (number < 5) { //less than 5, calculated directly statistics
    for (i = 0; i < number; i++) {
      amount += arr[i];
    }
    avg = amount / number;
    return avg;
  } else {
    if (arr[0] < arr[1]) {
      min = arr[0]; max = arr[1];
    }
    else {
      min = arr[1]; max = arr[0];
    }
    for (i = 2; i < number; i++) {
      if (arr[i] < min) {
        amount += min;      //arr<min
        min = arr[i];
      } else {
        if (arr[i] > max) {
          amount += max;  //arr>max
          max = arr[i];
        } else {
          amount += arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (double)amount / (number - 2);
  }//if
  return avg;


}
