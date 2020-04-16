
volatile byte revolutions = 0;       //  volatile to store number of revolutions

unsigned long int rpm = 0, maxRPM = 0;  //  variable for storing rpm and max rpm

unsigned long time = 0;         //  variable for time per revolution

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
  delay(2000);
}

void loop()
{
  currenttime = millis();                 // get current time

  if (revolutions >= 20 )
  {
    rpm = 30 * 1000 / (millis() - time) * revolutions; //  calculate rpm (30 s/min * 1000 ms/s / (time [ms]) * revolutions [-])

    if (rpm > maxRPM)
      maxRPM = rpm;                             //  check for max rpm
    time = millis();

    revolutions = 0; // reset revolutions

    Serial.println(rpm); // print rpm
  }
  delay(500);
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
