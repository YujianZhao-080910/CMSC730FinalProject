#include <WiFi.h>

const char* ssid     = "ESP32-Access-Point";
const char* password = "123456789";

WiFiServer server(80);

String header;
long height;

void setup() {
  Serial.begin(9600);

  Serial.println("Setting up Access Point…");
  //WiFi.softAP(ssid, password);
  WiFi.softAP(ssid);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("IP address: ");
  Serial.println(IP);
  
  server.begin();
}

void loop(){
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        header += c;
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            displayHTML(client);    
            int pos = header.indexOf("height=");
            if (pos > 0) {
              Serial.println("Get user's height data!");
              height = parseHeight(header, pos+7);
              Serial.println(height);
            }
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    header = "";
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

long parseHeight(String header, int pos) {
  int posStep = 0;
  for (; isDigit(header.charAt(pos+posStep)); posStep++);
  String heightStr = header.substring(pos, pos+posStep+1);
  return heightStr.toInt();
}

void displayHTML(WiFiClient client) {
  client.println("<!DOCTYPE html><html>");
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  
  client.println("<body><h1>ESP32 Web Server</h1>");
  client.println("<form method=\"get\">");
  client.println("<label for=\"height\"> Height (cm) </label>");
  client.println("<input type=\"number\" id=\"height\" name=\"height\" min=\"0\">");
  client.println("<input type=\"submit\" value=\"SUBMIT\">");
  client.println("</form>");
  client.println();

  
}
