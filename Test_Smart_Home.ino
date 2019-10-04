

//PIR
int ledPin=3;   // led on garag
int inputPin=2;  // conect to Data pin on PIR
int pirState=LOW; 
int val=0; 


//DHT11
#include<dht.h>
dht DHT;
#define DHT11_PIN 4
int buzzerpin=5; //buzzer is conected on arduino pin 5 
int fanpin=6; // fan is conected on arduino pin 6 

//Ultrasonic
int ledpin=7; // //led is conected on arduino pin 7
int trigC=8;   //trig pin at center  ultrasonic
int echoC=9;   //echo pin at  center ultrasonic
long durationC ; // value of time center ultrasonic
long distanceC ; // value of distance center ultrasonic

//Servo Motor
#include <Servo.h> 
int servo1Pin = 10;
int input1 = A1;
int garag_door=0;
Servo Servo1; 

int servo2Pin = 12;
int input2 = A2;
int front_door=0;
Servo Servo2; 


// Gas Sensor 
const int gasPin = A0; //GAS sensor output pin to Arduino analog A0 pin
int Y= 0;
#define LEDFLASHER 11
#define ALARMSOUNDER 13 
int i = 0; 
int val1 = LOW; 
int pre_val = LOW; 
int state = 0; 


//--------------------------------------------------


void setup() {
  // PIR
  pinMode(ledPin,OUTPUT); 
  pinMode(inputPin,INPUT); 

  // DHT11
  pinMode(fanpin,OUTPUT); //define fan pin as output 
  pinMode(buzzerpin,OUTPUT); // define buzzer pin as output 

  //Ultrasonic
  pinMode(trigC,OUTPUT); //define trig pin as output
  pinMode(echoC,INPUT);   //define echo pin as input
  pinMode(ledpin,OUTPUT);

  // Servo Motor
  Servo1.attach(servo1Pin); 
  pinMode(garag_door,INPUT);

  Servo2.attach(servo2Pin); 
  pinMode(front_door,INPUT);

  //Gas Sensor
    pinMode(LEDFLASHER, OUTPUT); 
    pinMode(ALARMSOUNDER, OUTPUT); 

  // to print Value on Serial Monitor
  Serial.begin(9600);
}

void loop() {
  val = digitalRead(inputPin);

  
  // PIR
  PIR();

  // DHT11
  DHT11();
  
  //ultra
  ultra();

  //Servo
  servo1();
  servo2();

  // Gas
  Gas();

}

void PIR (){

if (val == 1 ) 
  {  
    digitalWrite (ledPin,HIGH);
    delay(100);
  }
  else 
  {
    digitalWrite (ledPin,LOW);
  }
}

//---------------------------------------------------

void DHT11(){
  int chk = DHT.read11(DHT11_PIN);
  
  Serial.print(" Temparature is      ");
  Serial.println(DHT.temperature);
  
  if (DHT.temperature>30) 
  {
  alarm(); // start alarm if no reading form sensor 
  return;
  }

//cheak if temperature 
  if (DHT.temperature>26) 
// if it greater than 27 turn on  the fan 
  {
  digitalWrite(fanpin,HIGH);
  return;
  }
// if it less than or equal 22 turn off the fan 
  else if (DHT.temperature <= 22 )
  {
  digitalWrite(fanpin,LOW);
  }
}

//----------------------------------------------------


void alarm()
{
  digitalWrite(buzzerpin,HIGH);
  delay(500);
  digitalWrite(buzzerpin,LOW);
  delay(1000); 
  digitalWrite(buzzerpin,HIGH);
  delay(500);
  digitalWrite(buzzerpin,LOW);
}

//-----------------------------------------------------


void ultra(){
  ultrasonicC () ;
  if(distanceC >= 200 || distanceC <= 0)
{
    Serial.println("Out of range");
}
  else {
    Serial.print(distanceC);
    Serial.println(" cm");
}
  if (distanceC <= 3 && distanceC >= 0 )
  {  
    digitalWrite(ledpin,HIGH);
    delay(1000);
  }
  else 
  {
    digitalWrite(ledpin,LOW);
  }
}

//-----------------------------------------------------


//ultrasonic operating function
void ultrasonicC ()
{
  digitalWrite(trigC, LOW); // turn off trig from sending any sound wave
  delayMicroseconds(2); // wait 2 micro second
  digitalWrite(trigC, HIGH); //turn on trig and send a sound wave
  delayMicroseconds(10); // delay 2 micro second
  digitalWrite(trigC, LOW); //trun off trig
  durationC = pulseIn(echoC, HIGH); // measure time for sound wave to come back
  distanceC = durationC / 58; //equation to calculate the distance from mesured time and fixed sound speed in air
}


//---------------------------------------------------------

//Servo Motor 1
void servo1()
{
  garag_door = analogRead(input1);  
  if (garag_door >= 1000 ) 
   {  
    // Make servo go to 0 degrees 
   Servo1.write(90); 
   }
   else 
    {
   Servo1.write(180); 
  
}
}


//Servo Motor 1
void servo2()
{
  garag_door = analogRead(input2);
  if (front_door >= 1000) 
   {  
    // Make servo go to 0 degrees 
   Servo2.write(90); 
   }
   else if(front_door == 0)
   {
   Servo2.write(180); 
  
}
}
// Gas Sensor

  void Gas()
  {
    Y= analogRead(gasPin);
  Serial.println(analogRead(gasPin));
  if(Y<5)
  {
    Alarm();
  }
      else
    {
        analogWrite(ALARMSOUNDER, 0);
        analogWrite(LEDFLASHER, 0);
    }
  }



////Alarm

void Alarm(){

        for(i = 0; i < 255; i = i + 2)
        {
            analogWrite(LEDFLASHER, i);
            analogWrite(ALARMSOUNDER, i);
            delay(10);
        }
        for(i = 255; i > 1; i = i - 2)
        {
            analogWrite(LEDFLASHER, i);
            analogWrite(ALARMSOUNDER, i);
            delay(5);
        }
        for(i = 1; i <= 10; i++)
        {
            analogWrite(LEDFLASHER, 255);
            analogWrite(ALARMSOUNDER, 200);
            delay(100);
            analogWrite(LEDFLASHER, 0);
            analogWrite(ALARMSOUNDER, 25);
            delay(100);
        }
 
    pre_val = val1; 
}

