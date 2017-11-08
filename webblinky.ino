#include <ESP8266WiFi.h>
 //put the ssid and wpa-psk below. 
const char* ssid = "";
const char* password = "";
 
int ledPin = 13; // d7
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
 
  // Connect
  Serial.print("Connecting to network: ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started\n");
 
  // Print the IP address
  Serial.print("IP : ");
  Serial.print(WiFi.localIP());
 
}
 
void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  Serial.println("client connected!\n");
  while(!client.available()){
    delay(1);
  }
 
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
 
  // Respond
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led pin is now: ");
 
  if(value == HIGH) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
