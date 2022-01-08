#include <Arduino.h>
#include <constants.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NewRemoteTransmitter.h>

String ip;

int count;

ESP8266WebServer server(80);

NewRemoteTransmitter transmitter(ADDRESS, TX, 230, 3);

String IpAddress2String(const IPAddress& ipAddress)
{
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3])  ; 
}

void handle_menu() {
  String message = " \
    <!DOCTYPE html> \
    <html> \
    <head> \
      <title>Lights Menu</title> \
    </head> \
     \
    <body> \
     \
    <center> \
    <div style=\"font-size:50px\"> \
    All \
    <br> \
    <a href=\"http://" + ip + "/allLightsOn\">On</a> |  \
    <a href=\"http://" + ip + "/allLightsOff\">Off</a> \
    <br> \
    <br> \
     \
    Hall Downstairs \
    <br> \
    <a href=\"http://" + ip + "/light?id=0&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=0&state=0\">Off</a>  \
    <br> \
    <br> \
     \
    Kitchen \
    <br> \
    <a href=\"http://" + ip + "/light?id=1&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=1&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Dining Room \
    <br> \
    <a href=\"http://" + ip + "/light?id=2&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=2&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Living Room \
    <br> \
    <a href=\"http://" + ip + "/light?id=3&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=3&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Outside \
    <br> \
    <a href=\"http://" + ip + "/light?id=4&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=4&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Hall 1st floor \
    <br> \
    <a href=\"http://" + ip + "/light?id=5&state=1\">On</a> | \
    <a href=\"http://" + ip + "/light?id=5&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Bedroom \
    <br> \
    <a href=\"http://" + ip + "/light?id=6&state=1\">On</a> | \
    <a href=\"http://" + ip + "/light?id=6&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Bathroom \
    <br> \
    <a href=\"http://" + ip + "/light?id=7&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=7&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Shower Room \
    <br> \
    <a href=\"http://" + ip + "/light?id=8&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=8&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Hall 2nd floor \
    <br> \
    <a href=\"http://" + ip + "/light?id=9&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=9&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Nan's Room \
    <br> \
    <a href=\"http://" + ip + "/light?id=10&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=10&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Ruben's Room \
    <br> \
    <a href=\"http://" + ip + "/light?id=11&state=1\">On</a> |  \
    <a href=\"http://" + ip + "/light?id=11&state=0\">Off</a> \
    <br> \
    <br> \
     \
    Attic \
    <br> \
    <a href=\"http://" + ip + "/light?id=12&state=1\">On</a> | \
    <a href=\"http://" + ip + "/light?id=12&state=0\">Off</a> \
    <br> \
    <br> \
     \
     \
    </div> \
    </center> \
     \
    </body> \
    </html> \
  \
  ";
  server.send(200, "text/html", message); 
}

void handle_lightAllOn() {
  for (int i = 0; i < NoLIGHTS; i++) {
    digitalWrite(2, LOW);
    transmitter.sendUnit(i, 1);
    digitalWrite(2, HIGH);
    delay(50);
  }
  server.send(200, "text/html", "All lights are on.");
}

void handle_lightAllOff() {
  for (int i = 0; i < NoLIGHTS; i++) {
    digitalWrite(2, LOW);
    transmitter.sendUnit(i, 0);
    digitalWrite(2, HIGH);
    delay(50);
  }
  server.send(200, "text/html", "All lights are off."); 
}

void handle_light() {
  String message = "";
  int id;
  int state;
  message += "\
    <html> \
      <head> \
      <meta http-equiv=\"refresh\" content=\"0;url=http://" + ip + "/menu\" /> \
      <title>Lights Menu</title> \
    </head> \
    <body> \
  ";
  if (server.arg("id")== ""){     //Parameter not found
    message += "Light ID is not found";
  }else{     //Parameter found
    message += "Light ID = ";
    message += server.arg("id");     //Gets the value of the query parameter
    id = server.arg("id").toInt();
    if (server.arg("state")== ""){     //Parameter not found
      message += "Light state is not found";
    }else{
      message += "Light state = ";
      message += server.arg("state");
      state = server.arg("state").toInt();
      if (! (state == 0 || state == 1)){
         message += "is not acceptable.";
      } else {
        digitalWrite(2, LOW);
        transmitter.sendUnit(id, state);
        digitalWrite(2, HIGH);
      }
    }
    message += "\
    </body> \
    </html> \
    ";
    server.send(200, "text/html", message); 
  }
}

void handle_NotFound(){
 server.send(404, "text/plain", "Not found");
}

String SendHTML(){
  String ptr = "OK";
//  ptr +=(double)Temperaturestat;
//  ptr +=";";
//  ptr +=(double)Humiditystat;
  return ptr;
}

void setup() {
  Serial.begin(9600);
  delay(100);

  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  count = 0;

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
 
  Serial.println();
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  
  Serial.println(WiFi.localIP());
  ip = IpAddress2String(WiFi.localIP());

  if (!MDNS.begin("lightcontroller")) 
  {             // Start the mDNS responder for esp8266.local
    Serial.println("Error setting up MDNS responder!");
  }
  else 
  {
    Serial.println("mDNS responder started");
  }

  server.on("/menu", handle_menu);
  server.on("/light", handle_light);
  server.on("/allLightsOn", handle_lightAllOn);
  server.on("/allLightsOff", handle_lightAllOff);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");


}
void loop() {
  MDNS.update();
  if (count == 7200) {
    while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.print(".");
    }
    count = 0;
    ip = IpAddress2String(WiFi.localIP());
  } else {
    count++;
  }
  server.handleClient();
  delay(500);
}

