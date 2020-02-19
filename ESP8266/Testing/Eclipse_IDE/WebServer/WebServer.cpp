#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "---"
#define STAPSK  "---"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

String hyHtml = "<!DOCTYPE html><html lang=\"en\"><head>"
				"<meta charset=\"UTF-8\">"
				"<title>Server Hy</title></head>"
				"<body><p>Hello from ESP8266</p></body></html>";

boolean newRequest = false;
String message;

void handleRoot() {
	message = "hello from esp8266!";
	server.send(200, "text/plain", message);
	newRequest = true;
}

void handle_hyHtml() {
	server.send(200, "text/html", hyHtml);
	message = "Html file hy.html requested";
	newRequest = true;
}

void handleNotFound() {
  message = "File Not Found\r\n";
  message += "URI: ";
  message += server.uri();
  message += "\r\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\r\nArguments: ";
  message += server.args();
  message += "\r\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\r\n";
  }
  server.send(404, "text/plain", message);
  newRequest = true;
}

void setup(void) {
	message.reserve(256);
	Serial.begin(115200);
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.println("");

	// Wait for connection
	while (WiFi.status() != WL_CONNECTED) {
	delay(500);
	Serial.print(".");
	}
	Serial.println("");
	Serial.print("Connected to ");
	Serial.println(ssid);
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	if (MDNS.begin("esp8266")) {
	Serial.println("MDNS responder started");
	}

	server.on("/", handleRoot);
	server.on("/hy.html", handle_hyHtml);

	server.onNotFound(handleNotFound);

	server.begin();
	Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();

  if(true == newRequest){
	  newRequest = false;
	  Serial.println(message);
	  message.clear();
  }
}
