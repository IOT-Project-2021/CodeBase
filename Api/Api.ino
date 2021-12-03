#include <WiFi.h>
#include <WifiLocation.h>

const char* googleApiKey = "AIzaSyCw_DIRQ4beE1ZiH5I8mBEpojCxoss3tRs";
const char* ssid = "Galaxy M21142D"; // use your wifi network name
const char *password = "idtg3709"; // use network password

WifiLocation location (googleApiKey);

void setup() 
{
  Serial.begin(9600);

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

  setClock ();
   
  location_t loc = location.getGeoFromWiFi();

  Serial.println("Location request data");
  Serial.println(location.getSurroundingWiFiJson()+"\n");
  Serial.println ("Location: " + String (loc.lat, 7) + "," + String (loc.lon, 7));
  Serial.println ("Accuracy: " + String (loc.accuracy));
  Serial.println ("Result: " + location.wlStatusStr (location.getStatus ()));
}

void loop() 
{
  Serial.println("Done");
  delay(1000);
}


void setClock () 
{
  configTime (0, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print ("Waiting for NTP time sync: ");
  time_t now = time (nullptr);
  while (now < 8 * 3600 * 2)
  {
    delay (500);
    Serial.print (".");
    now = time (nullptr);
  }
  struct tm timeinfo;
  gmtime_r (&now, &timeinfo);
  Serial.print ("\n");
  Serial.print ("Current time: ");
  Serial.print (asctime (&timeinfo));
}
