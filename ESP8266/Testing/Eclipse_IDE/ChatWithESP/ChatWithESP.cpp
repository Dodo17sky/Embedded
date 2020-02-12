#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "ESP8266_AP";
const char *password = "testpass";

// Set web server port number to 80
WiFiServer EspServer(80);

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
	Serial.begin(115200);
	WiFi.softAP(ssid, password);

	Serial.println();
	Serial.print("Server IP address: ");
	Serial.println(WiFi.softAPIP());
	Serial.print("Server MAC address: ");
	Serial.println(WiFi.softAPmacAddress());

	EspServer.begin();
	Serial.println("Server listening");
}

void loop(){
  WiFiClient client = EspServer.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
        	  String chatmsg = "";
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            if (header.indexOf("GET /cmd/") >= 0) {
            	int endIdx = header.indexOf(" HTTP/1");
            	String msg = header.substring(9,endIdx);
            	chatmsg += "<p>Msg is:{";
            	chatmsg += msg;
            	chatmsg += "}</p>";
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta charset=\"UTF-8\"><title>ESP server</title></head>");
            client.println("<body><h2>Send message to ESP</h2>");
            client.println("<input id=\"Inp1\" type=\"text\"><br><br>");
            client.println("<button onclick=\"goo()\">Send</button>");
            client.println(chatmsg);
            client.println("<script>function goo(){window.location.href='/cmd/'+document.getElementById(\"Inp1\").value;}</script>");
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
