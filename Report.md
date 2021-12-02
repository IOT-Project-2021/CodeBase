## Introduction to IoT, Monsoon 2021

### Project : Smart Bike

### Team AUNV

**Objectives**

Our initial Objectives are:

* To alert driver of any obstacles near him, when parking or at moments, where it is required
* To automatically switch on the headlights, when the surroundings are dim
* To report any accident happened, by sending an email to friends of the user along with location.
* When Security mode is on, which can be controlled by user with an interface provided to him, we alert user of unwanted motions around his vehicle, to avoid any robberies.
* 
_Suggest more if any_

**Challenges**

We faced the following challenges during the implementation of project

* Notifying about unwanted motion: When security mode is on, we thought of notifying user
* Also about location details at the time of accidents We finally made sending email to as notification to appropriate people.
*    

_Suggest Some more_

**Code**

_**Arduino Code**_

1. First, include all the libraries and define some macros

```
#include <ESP32Tone.h>
#include <WiFi.h>
#include <WiFiServer.h>
#include <ESP32Servo.h>


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
```

2. Then, we initialise some wifi-credentials, details of om2m server e.t.c..
```
const char* ssid = "Galaxy M21142D"; // use your wifi network name
const char *password = "idtg3709"; // use network password
String cnt = "node1"; // container i.e., "node" + node_number
String cse_ip = "127.0.0.1"; // YOUR IP from ipconfig/ifconfig
String cse_port = "8080";
String onem2m_server = "http://" + cse_ip + ":" + cse_port + "/~/in-cse/in-name/";

WiFiServer server(80);

Servo servol;
```

3. In the `setup()` function, initialise modes of pin, and set them all to low.Then begin serial Communication and connected to WiFi
```
WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
```

4. In `loop()` function, we check for driver/owner's willing to turn on security mode in `handle_network()` function.

5. If security mode is on, we will sense any motion in `PIR_motion_sensing` function
```
if(security_mode)
  {
   PIR_motion_sensing();
   createCI("PIR_motion_sensor", pir_to_onem2m);
  }
```
6. Else, we calculate distance, Light intensity in surroundings, and send the same data to om2m Resource tree
```
else  
  {
    Distance_sensing();
    LDR_sensing();
    createCI("LDR_sensor", ldr_to_onem2m);
    createCI("Ultrasonic_Distance_sensor", distance_to_onem2m);
  }
```
7. In PIR_motion_sensing, we read the value from PIR_MOTION_SENSOE_PIN, and send the read value to om2m tree.
```
int pir_reading = digitalRead(PIR_MOTION_SENSOR_PIN);
  pir_to_onem2m=pir_reading;
```

8. If read value is high and red_led_on is false, we put it to high and tone the buzzer
```
int pir_reading = digitalRead(PIR_MOTION_SENSOR_PIN);
  pir_to_onem2m=pir_reading;
```

9. If not in security mode and red_led is on, we turn it off and turns off buzzer using `noTone()`
```
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
```

10. In `LDR-Sensing()` function, read the value of digital pin, send it to om2m server and turn required led's.

11. In `Distance_sensing()` function, we read the distance sensed from `GiveDistance()` function and set the servomotor dial to appropriate angle.

12. If distance is less than a threshold value, we start ringing buzzer and set the led to glow.

**_Python scripts_**

*Should we include description about python scripts also? What about API.ino*



