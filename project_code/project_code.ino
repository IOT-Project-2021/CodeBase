#include <ESP32Tone.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <ESP32Servo.h>
#include <HTTPClient.h>


#define LDR_SENSOR_PIN 26
#define PIR_MOTION_SENSOR_PIN 27
#define SERVO_PIN 5
#define BUZZER_PIN 33
#define LED_RED_PIN 15
#define LED_WHITE_PIN 2
#define LED_GREEN_PIN 4
const int EchoPin = 13;
const int TrigPin = 12;
#define POWER_PIN_RUNNING_MODE 34
#define POWER_PIN_SECURITY_MODE 35

const char* ssid = "Galaxy M21142D"; // use your wifi network name
const char *password = "idtg3709"; // use network password
String cnt = "node1"; // container i.e., "node" + node_number
String cse_ip = "127.0.0.1"; // YOUR IP from ipconfig/ifconfig
String cse_port = "8080";
String onem2m_server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";
float pir_to_onem2m, ldr_to_onem2m, distance_to_onem2m;

WiFiServer server(80);

Servo servol;

float Distance;
float safeDistance = 10; // Distance is in SI Units


bool white_led_on = false;
bool red_led_on = false;
bool green_led_on = false;
bool security_mode = false;

void setup()
{
  // sensor pins setup
  pinMode(LDR_SENSOR_PIN, INPUT);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  servol.attach(SERVO_PIN);
  servol.write(0);
  pinMode(PIR_MOTION_SENSOR_PIN, INPUT);
  
  // led pins
  pinMode(LED_WHITE_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  digitalWrite(LED_WHITE_PIN, LOW);
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_GREEN_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);


  Serial.begin(9600);

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
   PIR_motion_sensing();
   createCI("PIR_motion_sensor", pir_to_onem2m);
  }
  else  
  {
    Distance_sensing();
    LDR_sensing();
    createCI("LDR_sensor", ldr_to_onem2m);
    createCI("Ultrasonic_Distance_sensor", distance_to_onem2m);
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
            client.print("<h1> Security mode </h1>");
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
        
        if (currentLine.endsWith("GET /L"))
        {
          security_mode = false;
          digitalWrite(POWER_PIN_RUNNING_MODE, HIGH);
          digitalWrite(POWER_PIN_SECURITY_MODE, LOW);
          digitalWrite(LED_GREEN_PIN, HIGH);
          Serial.println("Security mode Turned off");
        }
        if (currentLine.endsWith("GET /H"))
        {
          security_mode = true;
          digitalWrite(POWER_PIN_RUNNING_MODE, LOW);
          digitalWrite(POWER_PIN_SECURITY_MODE, HIGH);
          digitalWrite(LED_GREEN_PIN, LOW);
          delay(100);
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
  // Turns on leds when light intensity is low.
  int pir_reading = digitalRead(PIR_MOTION_SENSOR_PIN);
  pir_to_onem2m=pir_reading;
  Serial.print("PIR reading = ");
  Serial.println(pir_reading);
  if (pir_reading == HIGH)
  {
    if (!red_led_on)
    {
      digitalWrite(LED_RED_PIN, HIGH);
      Serial.println("RED LED ON");
      tone(BUZZER_PIN,1000);
      red_led_on = true;
    }
  }
  else
  {
    if (red_led_on)
    {
      digitalWrite(LED_RED_PIN, LOW);
      Serial.println("RED LED OFF");
      noTone(BUZZER_PIN);
      red_led_on = false;
    }
  }
}

void LDR_sensing()
{
  // Turns on leds when light intensity is low.
  int ldr_reading = digitalRead(LDR_SENSOR_PIN);
  ldr_to_onem2m=ldr_reading;
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

void move_servo_dial(float distance)
{
   // angle = 0.45*distance  where distance is in cm
  servol.write(0.45*distance);
  Serial.print(0.45*distance);
}

void Distance_sensing()
{
    float Distance = GiveDistance();
    Serial.println(Distance);
    move_servo_dial(Distance);  //rotating servo motor

    if (Distance < safeDistance) //critical check
    {
      if (!red_led_on)
      {
        tone(BUZZER_PIN, 1000);
        digitalWrite(LED_RED_PIN, HIGH);
        red_led_on = true;
      }
    }
    else
    {
      if (red_led_on)
      {
        noTone(BUZZER_PIN);
        digitalWrite(LED_RED_PIN, LOW);
        red_led_on = false;
      }
    }
}

void createCI(String ae, float val) 
{
  // creates a client instance
  HTTPClient http;
  http.begin(onem2m_server + ae + "/" + cnt + "/");

  http.addHeader("X-M2M-Origin", "admin:admin");
  http.addHeader("Content-Type", "application/json;ty=4");

  int code = http.POST("{\"m2m:cin\": {\"cnf\":\"application/json\",\"con\": " + String(val) + "}}");

  Serial.println(code);
  if (code == -1) 
    Serial.println("UNABLE TO CONNECT TO THE SERVER");
  http.end();
}
