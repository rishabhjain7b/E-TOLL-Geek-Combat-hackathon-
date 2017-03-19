#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "GEEK COMBAT";
const char* password = "gcdefault";

int count = 0;                                          // count = 0
char input[12];                                         // character array of size 12 
boolean flag = 0;                                       // flag =0
bool read = false;

ESP8266WebServer server(80);

void setup()
{
   Serial.begin(9600);                                  // begin serial port with baud rate 9600bps
   read = false;
   WiFi.begin(ssid, password);
   Serial.print("CONNETING TO WIFI");
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   }
   Serial.println("CONNECTED to WIFI");
   Serial.println(WiFi.localIP());
   server.on("/", handleRoot);
   server.onNotFound(handleNotFound);
   server.begin();
}

void loop()
{
   if(Serial.available())
   {
      count = 0;
      while(Serial.available() && count < 12)          // Read 12 characters and store them in input array
      {
         input[count] = Serial.read();
         count++;
         Serial.print(input);
         delay(20);
        // Serial.println(count);
      }
      read = true;
      Serial.println(input);                             // Print RFID tag number 
   }
   else {
     read = true;
   }
   server.handleClient();
}

void handleRoot() {
  if(read)
    server.send(200, "text/plain", input);
  else
    server.send(200,"text/plain", "NOTHING");
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
}
