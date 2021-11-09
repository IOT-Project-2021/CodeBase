#include <ESP32Tone.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <Servo.h>

const char* ssid = "WiFiname"; // use your wifi network name
const char *password = "password"; // use network password
WiFiServer server(80);

#define LDR_SENSOR_PIN 26
#define PIR_MOTION_SENSOR_PIN 27

#define SERVO_PIN 32
#define BUZZER_PIN 33

#define LED_RED_PIN 15
#define LED_WHITE_PIN 2
#define LED_GREEN_PIN 4

Servo servol;
const int EchoPin = 13;
const int TrigPin = 12;
float Distance;
float safeDistance = 10; // Distance is in SI Units
bool white_led_on;
bool red_led_on;
bool green_led_on;

int security_mode;

void setup()
{

  security_mode = 0; // security mode off
  // sensor pins setup
  pinMode(LDR_SENSOR_PIN, INPUT);

  // led pins
  pinMode(LED_WHITE_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(LED_WHITE_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);

  white_led_on = false;
  red_led_on = false;
  green_led_on = false;

  Serial.begin(115200);
  servol.attach(SERVO_PIN);
  // network part
  Serial.println("Network Name : ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.print("\n");
  Serial.println("WiFi connected..!");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("Server started!!!");
  Serial.print("Wifi status = ");
  Serial.println(WiFi.status());
  
}

void loop()
{
  // network part
  handle_network();

  // main code part
  if(security_mode)
  {
    // PIR_motion_sensing();
  }
  else  
  {
    Distance_sensing();
    LDR_sensing();
  }
  delay(500);
}

void handle_network()
{
   WiFiClient client = server.available();

  if (client) 
  {
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n')
        {
          if (currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("content-type:text/html");
            client.println();
            client.print("<h1> Turn LED and BUZZER on and off </h1>");
            client.print(" <span style=\"background-color: rgb(0, 255, 0)\; border-radius: 10px\;padding: 10px\;font-size: 30px\;\" ><a href=\"H\" style=\"text-decoration: none\;\"> ON</a> </span><br><br><br><br>");
            client.print(" <span style=\"background-color: rgb(255, 0, 0)\; border-radius: 10px\;padding: 10px\;font-size: 30px\;\" ><a href=\"L\" style=\"text-decoration: none\;\"> OFF </a> </span><br>");
            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (c != '\r') {
          currentLine += c;
        }
        
        if (currentLine.endsWith("GET /L")) {
          // digitalWrite(LED_PIN, LOW);
          // noTone(BUZZER_PIN);
          // Serial.println("Turned off");
          security_mode = 0;
          Serial.println("Security mode Turned off");
        }
        if (currentLine.endsWith("GET /H"))
        {
          // digitalWrite(LED_PIN, HIGH);
          // tone(BUZZER_PIN, 600);
          // Serial.println("Turned on");
          security_mode = 1;
          Serial.println("Security mode Turned on");
        }
      }
    }
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
void PIR_motion_sensing()
{
  
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

void move_servo_dial(float diatance)
{


void Distance_sensing()
{
    float Distance = GiveDistance();

    move_servo_dial(Distance);  //rotating servo motor

    if(Distance < safeDistance)  //critical check
      tone(BUZZER_PIN, 2000, 2000);
}
