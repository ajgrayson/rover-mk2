/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "robotdude";
const char *password = "";

//#define DEBUG false

ESP8266WebServer server(80);

#ifdef DEBUG
#define DBG(message)    Serial.print(message)
#define DBGL(message)    Serial.println(message)
#else
#define DBG(message)
#define DBGL(message)
#endif // DEBUG

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
 * connected to this access point to see it.
 */
void handleRoot() {
  server.send(200, "application/json", "{\"message\":\"Good morning sir, awaiting your orders!\"");
}

void handleCommand() {
  String cmd = server.arg("cmd");
  DBG("Command received: ");
  DBG(cmd);
  DBGL("");

  if(cmd[0] == 'L') {
    DBG("Left: ");
    DBG(cmd.substring(1));
    DBGL("");
  } 
  else if(cmd[0] == 'R') {
    DBG("Right: ");
    DBG(cmd.substring(1));
    DBGL("");
  }
  
  sendCommandToRobot(cmd);

  server.send(200, "application/json", "{\"message\":\"ack\"}");
}

void sendCommandToRobot(String cmd) {
  Serial.print(cmd);
}

void setup() {
  delay(1000);
  
  Serial.begin(115200);
  
  DBGL("");
  DBG("Configuring access point...");
  
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  
  DBG("AP IP address: ");
  DBGL(myIP);
  
  server.on("/", handleRoot);
  server.on("/action", handleCommand);
  server.begin();
  
  DBGL("HTTP server started");
  
  delay(1000);
}

void loop() {
  server.handleClient();
}
