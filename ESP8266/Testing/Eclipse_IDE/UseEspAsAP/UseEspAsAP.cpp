#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "ESP8266_AP";
const char *password = "testpass";

const char* hyHtml = "<!DOCTYPE html><html lang=\"en\"><head>"
					"<meta charset=\"UTF-8\">"
					"<title>Server Hy</title></head>"
					"<body><p>Hello from ESP8266</p></body></html>";

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", hyHtml);
}

void setup() {
  Serial.begin(115200);

  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("Server IP address: ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Server MAC address: ");
  Serial.println(WiFi.softAPmacAddress());

  server.on("/", handleRoot);
  server.begin();

  Serial.println("Server listening");
}

void loop() {
  server.handleClient();
}
