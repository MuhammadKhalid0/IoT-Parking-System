

#define MAX_CARS 3
int carsNum = 0;

//Serial with  ESP
#include <SoftwareSerial.h>

//IR sensors
#define IR1_pin A0
#define IR2_pin A1
#define IR1_threshold 40
#define IR2_threshold 80
#define US_threshold 25
#define US_threshold2 10
float IR1_value, IR2_value;
const int trigger1 = 22, echo1 = 24, trigger2 = 26, echo2 = 28, trigger3 = 30, echo3 = 32;
int distance1, distance2, distance3;
long duration1, duration2, duration3;

//LCD
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2; //LCD pins
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//stepper motor
#include <AccelStepper.h>
const int stepsPerRevolution = 512;
const int IN1 = 9, IN2 = 8, IN3 = 7, IN4 = 6; //stepper pins
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4); //initialize the stepper library

void setup()
{

  //initializing LCD params to 16 cols * 2 rows
  lcd.begin(16, 2);

  //stepper max speed and acceleration
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(200);
  
  //Begin serial monitor
  Serial.begin(115200);
  
  //initializing ultrasonic sensors pins
  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);
  pinMode(trigger3, OUTPUT);
  pinMode(echo3, INPUT);

}

void loop()
{
  //Reading values of IR sensors
  IR1_value = analogRead(IR1_pin);
  IR2_value = analogRead(IR2_pin);

  //Car trying to enter
  if(IR1_value < IR1_threshold) //you might need to flip the comparison sign
  {
    if(carsNum < MAX_CARS)//car is allowed to enter
    {
      
      //display on LCD
      lcd.setCursor(0, 0);
      lcd.print("Welcome!");
      
      //open the gaaaaaaaates
      stepper.moveTo(stepsPerRevolution);
      while(stepper.distanceToGo() != 0)
      {
        stepper.run();
      }
      
      //close the gates after a small delay
      delay(1500);
      // check current stepper motor position to invert direction
      stepper.moveTo(-stepsPerRevolution);
      
      while(stepper.distanceToGo() != 0)
      {
        stepper.run();
      }

      delay(3000);
    }
    else //car is not allowed to enter
    {
      //display on LCD you are not allowed to enter
      lcd.setCursor(0, 0);
      lcd.print("FULL!");
      delay(2000);
    }
  }
  else //No car is at the front sensor
  {
    //Car trying to leave
    if(IR2_value < IR2_threshold) //you may need to flip the comparator sign
    {
      
      
      //display on LCD
      lcd.setCursor(0, 0);
      lcd.print("Bye!");
      
      //open the gaaaaaaaates
      stepper.moveTo(stepsPerRevolution);
      while(stepper.distanceToGo() != 0)
      {
        stepper.run();
      }
      
      //close the gates after a small delay
      delay(1500);
      // check current stepper motor position to invert direction
      stepper.moveTo(-stepsPerRevolution);
      
      while(stepper.distanceToGo() != 0)
      {
        stepper.run();
      }

      delay(3000);
    }
    else //no car is trying to leave or enter
    {
      //close the gates
      stepper.moveTo(-stepsPerRevolution);
      
      while(stepper.distanceToGo() != 0)
      {
        stepper.run();
      }
      
    }
  }


  \\Serial communication with ESP
  if (Serial.available()) {
    carsNum = Serial.read();
  }
  
  
}
