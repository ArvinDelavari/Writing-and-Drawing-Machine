#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
//------------------------------------------------------------------------
const char* ssid = "Arvin";
const char* password = "252525de";
const char* mqtt_server = "mqtt.eclipseprojects.io";
//------------------------------------------------------------------------
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
//------------------------------------------------------------------------
void setup_wifi() 
{
  delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Subscribing to MQTT Server CallBack Function
//------------------------------------------------------------------------
void callback(char* topic, byte* payload, unsigned int length) 
{
  char INSTRUCTIONS[20];
  // Subscribing to writingmachine/instructions
  // Recieving data from MQTT server
  if (strcmp(topic,"writingmachine/instructions") == 0)
  {
    for(int i=0 ; i<=(length-1) ; i++)
      INSTRUCTIONS[i]=(char)payload[i];
    String DATA_REC = String(INSTRUCTIONS);
    Serial.write(INSTRUCTIONS);
  }
  Serial.println();
} //end callback


// Reconnecting Function
//------------------------------------------------------------------------
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    //if you MQTT broker has clientID,username and password
    if (client.connect(clientId.c_str()))
    {
      Serial.println("connected");
     //once connected to MQTT broker, subscribe command if any
      client.subscribe("writingmachine/instructions",1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() 
{
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("writingmachine/instructions",1);
}

void loop() 
{
  if (!client.connected()) 
  {
    reconnect();
  }
  client.loop(); 
}
