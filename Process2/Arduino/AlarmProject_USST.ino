/*
 * Arduino Ultrasonic Alarm
 * by Mark Ladoing
 * 
 * Created April 19, 2017
 * Updated May 7, 2022
 * - Corrected code descriptions
 * Altered during June 2, 2024
 * Accepts multiple UIDS
 */
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Set constant variables for pins used by the sensor, buzzer, and LEDs:
const int trigPin = 6; // Note: set these numbers to what ever pin you attached them to
const int echoPin = 7;
const int ledPin = 5;
const int actLedPin = 2;
const int buzzPin = 4;
boolean alarmActive = false;
const String targetUID = ["90 7E DF 87", "40 C2 FF 30"];
const int listSize =  2;

void setup() {
  // Initialize serial communication:
  Serial.begin(9600);

  // Initialize RFID Reader:
  SPI.begin();
  mfrc522.PCD_Init();

  // Initialize LEDs:
  pinMode(ledPin, OUTPUT);
  pinMode(actLedPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);

  // Initialize ultrasonic-sensor:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Ensure alarm is set to off to prevent premature activation:
  alarmActive = false;
  digitalWrite(actLedPin, LOW);
}

void loop()
{
  readForCard();
  runSensor();
}

void readForCard()
{
  // Check if there is a Tag/Card present infront of the RFID Reader:
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Check if there is a UID when a Tag/Card is presented to the RFID Reader:
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  
  // Get UID from Tag/Card:
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  // Check UID for match and set alarm:
  content.toUpperCase();
  bool accepted = false;
  String uid = content.substring(1);

  for (int i = 0; i < listSize; i++){
    if (uid == targetUID[i])
    {
      accepted = true;
      break;
    }
  }
  if (accepted) 
  {
    if(alarmActive == true)
    {
      alarmActive = false;
      digitalWrite(actLedPin, LOW);
    }
    else
    {
      alarmActive = true;
      digitalWrite(actLedPin, HIGH);
    }
    // Set off a short tone and an LED to signal a sucessful activation/deactivation:
    tone(buzzPin, 500);
    delay(50);
    noTone(buzzPin);
    delay(50);
    tone(buzzPin, 500);
    delay(50);
    noTone(buzzPin);
  }
  else
  {
    // Make a different short tone to indicate that the alarm has failed to activate:
    Serial.println("ACCESS DENIED - ERROR");
    tone(buzzPin, 300);
    delay(200);
    tone(buzzPin, 100);
    delay(200);
    noTone(buzzPin);
  }
}

void runSensor()
{
  // Reset alarm at initialization to avoid premature activation:
  noTone(buzzPin);
  digitalWrite(ledPin, LOW);
  
  // Establish variables for duration of the ultrasonic pulse and the distance:
  long duration, inches, cm;

  // Allow the Ultrasonic Sensor to remain off for a short period to prevent immediately triggering the alarm:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Then send out an ultrasonic sound pulse that will bounce off any object that passes infront of it:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // If the pulse bounces off something and returns to the Ultrasonic Sensor, the time it took is captured and can be used for display:
  duration = pulseIn(echoPin, HIGH);

  // Convert the time it took for the pulse to back into a distance:
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  // Print off distance data to the Serial Monitor:
  printSensor(inches, cm);

  // Check if alarm has been triggered:
  checkAlarm(inches);

  // Delay for 50 milliseconds before looping over again
  // This ensures that the sensor is pulsing fast enough to detect fast-moving objects
  // but still slow enough for the RFID Reader to check if a tag/card has been presented:
  delay(50);
}

void printSensor(long inches, long cm)
{
  Serial.print(inches);
  Serial.print("in / ");
  Serial.print(cm);
  Serial.print("cm");
}

// Checks if the sensor has detected an object and triggers the alarm
void checkAlarm(long inches)
{
  if(inches < 32 && alarmActive == true)
  {
    Serial.println(" TRIGGERED");
    alarm(600);
  }
  else if(alarmActive == false)
  {
    Serial.println(" DISARMED");
  }
  else
  {
    Serial.println(" ARMED");
  }
}

// Triggers the buzzer/alarm for a set amount of time
void alarm(int seconds)
{
  // Convert the pre-set amount of time into a repeated series of alarm beeps:
  int duration = seconds * 4;
  
  for(int i=0; i <= duration; i++)
  {
    // Check if a Tag/Card has been presented to deactivate the alarm:
    readForCard();
    
    if(alarmActive == true)
    {
      digitalWrite(ledPin, HIGH);
      tone(buzzPin, 1000);
      delay(150);
      digitalWrite(ledPin, LOW);
      noTone(buzzPin);
      delay(150);
    }
    else
    {
      i=0;
      break;
    }
  }
}

// Converts the time taken for the ultrasonic pulse to bounce back into a distance
// Both Imperial and Metric measurement conversions have been included below:
long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
