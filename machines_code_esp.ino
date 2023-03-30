//What do we need?
  //1)IR -- 2)Stepper -- 3)ultrasonic -- 4)LCD --5)communication with firebase

 String ss = "Lane1";

 //Serial communication
 #include <SoftwareSerial.h>
 

//ultrasonic
int carsNum = 0;
const int trigger1 = 0, echo1 = 13, trigger2 = 14, echo2 = 2, trigger3 = 12, echo3 = 15;
int distance1, distance2, distance3;
long duration1, duration2, duration3;
#define US_threshold 25

//connecting to WiFi (MUST BE CHANGED FOR EACH DIFFERENT NETWORK)
#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#define WIFI_SSID "Oppo"
#define WIFI_PASSWORD "12344321"

//connecting to firebase (DO NOT CHANGE)
#define FIREBASE_HOST "https://parkingsystem-eab17-default-rtdb.firebaseio.com/"
#define FIREBASE_AUTH "omlcjcIFiWT1RCdQ6Lm6Vh81ne0LEUy35lleJawc"

FirebaseData firebaseData;

void setup()
{
  //Begin serial monitor
  Serial.begin(115200);
  Serial1.begin(115200);
  
  //initializing ultrasonic sensors pins
  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trigger3, OUTPUT);
  pinMode(echo3, INPUT);



  //connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  //connecting to firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}



void loop()
{
  
  //Ultrasonic sensor 1
  digitalWrite(trigger1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger1, LOW);
  duration1 = pulseIn(echo1, HIGH);
  distance1 = duration1 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  Serial.print("Sensor 1");
  Serial.println(distance1);
  if(distance1 < US_threshold) //if a car stands in the lane1
  {
    //setting the status of the lane to full
    carsNum++;
    Firebase.setString(firebaseData, "Lane1", "full");
  }
  else
  {
    carsNum--;
    //setting the status of the lane to empty
    Firebase.setString(firebaseData, "Lane1", "empty");
  }

  //Ultrasonic sensor 2
  digitalWrite(trigger2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger2, LOW);
  duration2 = pulseIn(echo2, HIGH);
  distance2 = duration2 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  Serial.print("Sensor 2");
  Serial.println(distance2);
  if(distance1 < US_threshold) //if a car stands in the lane2
  {
    //setting the status of the lane to full
    carsNum++;
    Firebase.setString(firebaseData, "Lane2", "full");
  }
  else
  {
    //setting the status of the lane to empty
    carsNum--;
    Firebase.setString(firebaseData, "Lane2", "empty");
  }

  //Ultrasonic sensor 3
  digitalWrite(trigger3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger3, LOW);
  duration3 = pulseIn(echo3, HIGH);
  distance3 = duration3 * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  Serial.print("Sensor 3`");
  Serial.println(distance3);
  if(distance1 < US_threshold) //if a car stands in the lane1
  {
    //setting the status of the lane to full
    carsNum++;
    Firebase.setString(firebaseData, "Lane3", "full");
    
  }
  else
  {
    carsNum--;
    //setting the status of the lane to empty
    Firebase.setString(firebaseData, "Lane3", "empty");
  }

  Serial1.println(carsNum);
}
