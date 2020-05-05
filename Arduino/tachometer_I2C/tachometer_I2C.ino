#include <Wire.h>

volatile byte revolutions = 0;       //  volatile to store number of revolutions

unsigned short rpm = 0; //  variable for storing rpm and max rpm

unsigned long time = 0;         //  variable for time per revolution
unsigned long stopTime = 0;
unsigned long startTime = 0;
byte buff[2]; 
int led = 0, prevRPM = 0; //  led state and previous rpm value

long currenttime = 0;

const int pinIR = 8;
const int pinLED = 13;

void setup()
{
  Serial.begin(9600);
  attachInterrupt(0, RPMCount, RISING);     //  Interrupt when sensor goes from low to high (interrupt 0 = pin 2 on UNO)
  pinMode(pinIR, INPUT);
  pinMode(pinLED, OUTPUT);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  Wire.onReceive(receiveEvent); // register event
  delay(2000);
}

void loop()
{

  if(stopTime != 0){
    rpm = (revolutions*60000)/(stopTime-startTime);
    //B1MSB = rpm >> 3;
    //B2 = rpm >> 2;
    buff[0] = (rpm >> 8) & 0xff;
    buff[1] = rpm  & 0xff;
    Serial.println(rpm);
    //Serial.println(B1MSB);
    //Serial.println(B2);
  //  Serial.println(buff[0]);
    //Serial.println(buff[1]);
    stopTime = 0;
  }
}


/* Interrupt function for counting revelutions 
 * Counts revelutions and blinks LED
 */
void RPMCount()                                
{
  revolutions++;                                         

  if (led == LOW)
  {

    led = HIGH;                                 
  }
  else
  {
    led = LOW;
  }
  digitalWrite(pinLED, led);
}

void requestEvent() {
  
  Wire.write(buff,2);
  // as expected by master
}

void receiveEvent(int howMany) {
  while (1 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer

  if(x == 16){
    stopTime = 0;
    startTime = millis();
    revolutions = 0;
    rpm = 0;}
  else if(x == 9){
    stopTime = millis();
    }
  }
