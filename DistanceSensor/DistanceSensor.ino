const int EchoPin = 13;
const int TrigPin = 12;

void setup()
{
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  Serial.begin(9600);  

}

void loop()
{
  float A = GiveDistance();
  Serial.println(A);
  delay(1000);
}



/* 
 *  This function returns the distance at the time when it is called.
 *  It returns a float value. The units are centimeter.
 *
 *  EchoPin and TrigPin global variables are used.
 *  https://create.arduino.cc/projecthub/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-036380  
 *  https://www.tutorialspoint.com/arduino/arduino_ultrasonic_sensor.htm
 */
float GiveDistance()
{
  float Duration, Distance;
  
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  Duration = pulseIn(EchoPin, HIGH);
  Distance = (Duration*.0343)/2;
  delay(100); 

  return Distance;
}
