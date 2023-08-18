// You MUST change the SSID and PASSWORD.
// IP is assigned by DHCP.
// Developed on an ESP8266-201

#include <ESP8266WiFi.h>
 
const char* ssid = "YOUR_SSID"; // <--- fill in here your router or wifi SSID
const char* password = "YOUR_WIFI_PASSWORD"; //<--- fill in here your router or wifi password
 #define RELAY1 12 // relay connected to  GPIO12
 #define RELAY2 13 // relay connected to  GPIO13
 #define RELAY3 14 // relay connected to  GPIO14
 #define RELAY4 15 // relay connected to  GPIO15
 String R1;
 String R2;
 String R3;
 String R4;
 String IP;
 int REFRESH = 0;
WiFiServer server(80);
 
void setup() 
{
  Serial.begin(115200); // must be same baudrate with the Serial Monitor
 
  pinMode(RELAY1,OUTPUT);
  digitalWrite(RELAY1, LOW);
  pinMode(RELAY2,OUTPUT);
  digitalWrite(RELAY2, LOW);
  pinMode(RELAY3,OUTPUT);
  digitalWrite(RELAY3, LOW);
  pinMode(RELAY4,OUTPUT);
  digitalWrite(RELAY4, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.hostname("ESP_Relay_Controller");
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
  IP = WiFi.localIP().toString().c_str();
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(IP);
  Serial.println("/");
 
}
 
void loop() 
{
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request
  if (request.indexOf("/RELAY1=ON") != -1)  
  {
    Serial.println("RELAY1=ON");
    digitalWrite(RELAY1,HIGH);
    REFRESH = 1;
  }
  if (request.indexOf("/RELAY1=OFF") != -1)  
  {
    Serial.println("RELAY1=OFF");
    digitalWrite(RELAY1,LOW);
    REFRESH = 1;
  }
  
  if (request.indexOf("/RELAY2=ON") != -1)  
  {
    Serial.println("RELAY2=ON");
    digitalWrite(RELAY2,HIGH);
    REFRESH = 1;
  }
  if (request.indexOf("/RELAY2=OFF") != -1)  
  {
    Serial.println("RELAY2=OFF");
    digitalWrite(RELAY2,LOW);
    REFRESH = 1;
  }

  if (request.indexOf("/RELAY3=ON") != -1)  
  {
    Serial.println("RELAY3=ON");
    digitalWrite(RELAY3,HIGH);
    REFRESH = 1;
  }
  if (request.indexOf("/RELAY3=OFF") != -1)  
  {
    Serial.println("RELAY3=OFF");
    digitalWrite(RELAY3,LOW);
    REFRESH = 1;
  }

  if (request.indexOf("/RELAY4=ON") != -1)  
  {
    Serial.println("RELAY4=ON");
    digitalWrite(RELAY4,HIGH);
    REFRESH = 1;
  }
  if (request.indexOf("/RELAY4=OFF") != -1)  
  {
    Serial.println("RELAY4=OFF");
    digitalWrite(RELAY4,LOW);
    REFRESH = 1;
  }
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  this is a must
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>ESP8266 RELAY Control</title></head>");
 if(REFRESH == 1)
 {
  client.println("<meta http-equiv=\"refresh\" content=\"0; URL=status\" >");
  REFRESH = 0;
 }
 else
 {
  client.println("<meta http-equiv=\"refresh\" content=\"60; URL=status\" >");
 }

 
  if(digitalRead(RELAY1) == 1) 
  {
    R1="RELAY1: ON <a href=\"/RELAY1=ON\">[ON]</a><a href=\"/RELAY1=OFF\">[OFF]</a><br>";
  } 
  else 
  {
    R1="RELAY1: OFF <a href=\"/RELAY1=ON\">[ON]</a><a href=\"/RELAY1=OFF\">[OFF]</a><br>";
  }
  
  if(digitalRead(RELAY2) == 1) 
  {
    R2="RELAY2: ON <a href=\"/RELAY2=ON\">[ON]</a><a href=\"/RELAY2=OFF\">[OFF]</a><br>";
  } 
  else 
  {
    R2="RELAY2: OFF <a href=\"/RELAY2=ON\">[ON]</a><a href=\"/RELAY2=OFF\">[OFF]</a><br>";
  }
  
  if(digitalRead(RELAY3) == 1) 
  {
    R3="RELAY3: ON <a href=\"/RELAY3=ON\">[ON]</a><a href=\"/RELAY3=OFF\">[OFF]</a><br>";
  } 
  else 
  {
    R3="RELAY3: OFF <a href=\"/RELAY3=ON\">[ON]</a><a href=\"/RELAY3=OFF\">[OFF]</a><br>";
  }
  
  if(digitalRead(RELAY4) == 1) 
  {
    R4="RELAY4: ON <a href=\"/RELAY4=ON\">[ON]</a><a href=\"/RELAY4=OFF\">[OFF]</a><br>";
  } 
  else 
  {
    R4="RELAY4: OFF <a href=\"/RELAY4=ON\">[ON]</a><a href=\"/RELAY4=OFF\">[OFF]</a><br>";
  }
  
  client.println("<center>Relay Controller</center><hr><br>");
  client.println(R1);
  client.println(R2);
  client.println(R3);
  client.println(R4);
  client.println("<a href=\"/status\">[REFRESH PAGE]</a>  ***DO NOT USE YOUR BROWSER BACK/FORWARD BUTTON!***");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
