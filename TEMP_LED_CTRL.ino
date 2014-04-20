#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 0
/*Data wire on teensy*/
/*Setup oneWire instance to communicate with onewire devices*/
OneWire oneWire(ONE_WIRE_BUS);

//pass onewire reference to DallasTemperature.h
DallasTemperature sensors(&oneWire);

/* defining the color of the pins*/
const int redPin =  21;
const int greenPin =  19;
const int bluePin =  20;
const int greenPinb = 18;
const int bluePinb = 17;
const int redPinb = 16;
double setTemp =73.00;
double currentTemp = 99.00;

const double precision = .5; 
const int COOLING = 1;
const int RESTING = 0;
int minOn = 5;
int maxOn = 3600;
int minOff = 600;

int rest_count;
int cool_count;
int fridge_status;

boolean tooWarm(){
  return(currentTemp >= (setTemp + precision));
}
boolean tooCold(){
  return(currentTemp <= setTemp);
}
boolean restLongEnough(){
  return(rest_count >= minOff);
}
boolean onLongEnough(){
  return(cool_count >= minOn);
}
boolean onTooLong(){
  return(cool_count >= maxOn);
}
  
  //setup runs once, on startup

void setup()  {
  //initialize the pins as outputs or inputs
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(38400);
  pinMode(bluePinb, OUTPUT);
  pinMode(greenPinb, OUTPUT);
  pinMode(redPinb, OUTPUT);
  sensors.begin();
  fridge_status = RESTING;
  rest_count = minOff;
  cool_count = 0;
}

void loop()
{
  getTemp();
 if(fridge_status == RESTING){
   if(tooWarm() && restLongEnough()){
   digitalWrite(redPin, HIGH);
   digitalWrite(greenPin, LOW);
   delay(2000);
 cool_count = 0;
fridge_status = COOLING;
Serial.println("FRIDGE ON");
digitalWrite(bluePinb, HIGH);
   }
   rest_count++;
}else{
   if(onTooLong() || (onLongEnough() && tooCold()))
   {
     digitalWrite(redPin, LOW);
     digitalWrite(greenPin, HIGH);
     delay(2000);
     rest_count = 0;
     fridge_status = RESTING;
     Serial.println("FRIDGE OFF");
   }
   cool_count++;
 }
 delay(1000);
}

void getTemp(){
  sensors.requestTemperatures();
  double t = sensors.getTempFByIndex(0);
  if(t > 0 && t < 120){
    currentTemp = t;
  Serial.println(currentTemp);
  }
}
