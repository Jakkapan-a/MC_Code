#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "Internet";
const char* password = "0987654321qw";
const char* mqtt_server = "broker_ip_address";

// Set your static IP address
IPAddress local_IP(192, 168, 1, 184); // your static ip
IPAddress gateway(192, 168, 1, 1);    // your gateway (router) ip
IPAddress subnet(255, 255, 255, 0);   // your network subnet
IPAddress primaryDNS(8, 8, 8, 8);     // optional: your preferred DNS server
IPAddress secondaryDNS(8, 8, 4, 4);   // optional: secondary DNS server

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Hostname defaults to esp8266-[ChipID]
  String hostname = "ESP8266-Device-01";
  WiFi.hostname(hostname); 

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void serialEvent()
{
  Serial.print("ESP :");
  while (Serial.available())
  {
    byte incomeByte = Serial.read();
  }
    Serial.println("");
}
void setup() {
  Serial.begin(115200);
  setup_wifi();
}

void loop() 
{
  // Serial.println("1");
  // delay(1000);
}