

const int echoPin = D6;
const int trigPin = D5;

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
#define BLYNK_PRINT Serial
// Use Virtual pin  for display values
#define PIN_HC_RS04 V3

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>


char auth[] = "i4SAsKfpdcSVfkf42COWlgp_EJKSN1fd";
char ssid[] = "Router_2.4GHz";// Your WiFi credentials.
char pass[] = "593321Ab";// Set password to "" for open networks.


long duration;
float distanceCm;

BLYNK_READ(PIN_HC_RS04)
{
  // This command writes Arduino's uptime in seconds to Virtual Pin (5)
  Blynk.virtualWrite(PIN_HC_RS04, distanceCm);
  Serial.println(distanceCm);
}


void setup() {
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
   Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
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
  distanceCm = duration * SOUND_VELOCITY/2;
  
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  
  
  delay(1000);
}
