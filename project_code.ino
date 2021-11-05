#include <ESP32Tone.h>

#define LDR_SENSOR_PIN 26
#define ECHO_PIN 13
#define TRIGGER_PIN 12
#define PIR_MOTION_SENSOR_PIN 27

#define SERVO_PIN 32
#define BUZZER_PIN 33

#define LED_RED_PIN 15
#define LED_WHITE_PIN 2
#define LED_GREEN_PIN 4

int white_led_on;
int red_led_on;
int green_led_on;

int security_mode;

void setup()
{

  security_mode = 0;
  // sensor pins setup
  pinMode(LDR_SENSOR_PIN, INPUT);

  // led pins
  pinMode(LED_WHITE_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);

  digitalWrite(LED_WHITE_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);

  white_led_on = false;
  red_led_on = false;
  green_led_on = false;

  Serial.begin(115200);
}

void loop()
{
  if(security_mode)
  {
    // PIR_motion_sensing();
  }
  else
  {
    // Distance_sensing
    LDR_sensing();
  }
  delay(2000);
}


void LDR_sensing()
{
  // Turns on leds when light intensity is low.
  int ldr_reading = digitalRead(LDR_SENSOR_PIN);
  Serial.print("LDR reading = ");
  Serial.println(ldr_reading);
  if ( ldr_reading == HIGH)
  {
    if (!white_led_on)
    {
      digitalWrite(LED_WHITE_PIN, HIGH);
      Serial.println("WHITE LED ON");
      white_led_on = true;
    }
  }
  else
  {
    if (white_led_on)
    {
      digitalWrite(LED_WHITE_PIN, LOW);
      Serial.println("WHITE LED OFF");
      white_led_on = false;
    }
  }

}
