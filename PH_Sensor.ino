#define BLYNK_PRINT Serial
#define SensorPin A0              //pH meter Analog output to Arduino Analog Input 0
#define Offset 0.00            //deviation compensate
#define samplingInterval 20
#define printInterval 800
#define ArrayLenth  40    //times of collection

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
char auth[] = "i4SAsKfpdcSVfkf42COWlgp_EJKSN1fd";
char ssid[] = "Router_2.4GHz";
char pass[] = "593321Ab";


// Use Virtual pin  for display values
#define PIN_PH V4

float ph;

BLYNK_READ(PIN_PH)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_PH, ph);
  Serial.println(ph);
}

int pHArray[ArrayLenth];   //Store the average value of the sensor feedback
int pHArrayIndex = 0;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("pH meter experiment!");    //Test the serial monitor
  Blynk.begin(auth, ssid, pass);
   lcd.begin();
   lcd.backlight();
}

void loop(void)
{
   lcd.setCursor(0, 1);
  lcd.print("ph :     ");
  lcd.setCursor(4, 1);
  lcd.print(ph);
  lcd.setCursor(12, 1);
  lcd.print("[pH]");
  delay(10);
  Blynk.run();
  static unsigned long samplingTime = millis();
  static unsigned long printTime = millis();
  static float voltage;
  if (millis() - samplingTime > samplingInterval)
  {
    pHArray[pHArrayIndex++] = analogRead(SensorPin);
    if (pHArrayIndex == ArrayLenth)pHArrayIndex = 0;
    voltage = avergearray(pHArray, ArrayLenth) * 3.5 / 1024;
    ph = 3.5 * voltage + Offset;
    samplingTime = millis();
  }
  if (millis() - printTime > printInterval)  //Every 800 milliseconds, print a numerical, convert the state of the LED indicator
  {
    Serial.print("Voltage:");
    Serial.print(voltage, 2);
    Serial.print("    pH value: ");
    Serial.println(ph, 4);
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
