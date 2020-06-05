#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#define SSID        "---"
#define PASSWORD    "---"

ESP8266WebServer server(80);

void handleRoot() {
    String serverData;
    static int call = 0;
    call++;

    String clientIP = server.client().remoteIP().toString();
    Serial.print("Client IP:");
    Serial.println(clientIP);
    Serial.print("MSG: ");
    Serial.println(server.uri());

    if((call%2) == 0){
        serverData = String("Numar par: ") + String(call);
    }
    else {
        serverData = String("Numar impar: ") + String(call);
    }
    server.send(200, "text/plain", serverData);
}

void handleNotFound(){
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET)?"GET":"POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i=0; i<server.args(); i++){
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
    Serial.println(message);
}

void handle_led() {
	String cmdName = server.argName(0);
	String cmdValue = server.arg(0);

	Serial.printf("Received comand [%s] value [%s]\n", cmdName.c_str(), cmdValue.c_str());

	if(cmdName == "STATUS") {
		if(cmdValue == "ON") {
			digitalWrite(0, HIGH);
		}
		else if(cmdValue == "OFF"){
			digitalWrite(0, LOW);
		}
	}

	server.send(200, "text/plain", "Received LED command");
}

void setup(void){
	pinMode(0, OUTPUT);

    Serial.begin(115200);
    WiFi.begin(SSID, PASSWORD);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(SSID);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266")) {
      Serial.println("MDNS responder started");
    }

    server.on("/", handleRoot);
    server.on("/led", handle_led);

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
