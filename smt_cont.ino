#include <ESP8266WiFi.h>
const int trigPin = D5;
const int echoPin = D6;
long duration;
int distance; 
float level;
const char* ssid = "Vinayak's Galaxy A12"; 
const char* password = "jqfd1052522";
void send_event(const char *event);
const char *host = "maker.ifttt.com";
/*192.168.50.134*/
const char *privateKey = "Xgsivyi9rvHOkyJTIKhoC";
WiFiServer server(80);
void setup() {
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 Serial.begin(9600);
 Serial.print("Connecting to Wifi Network");
 Serial.println(ssid);
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  Serial.println("");
  Serial.println("Successfully connected to WiFi.");
  Serial.println("IP address is : ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("Server started");
 }
}
void loop() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0340 / 2;
  Serial.println("Distance");
  Serial.println(distance);
  level =((14-distance)/14.0)*100;
  Serial.println("level");
  Serial.println(level);
  
  delay(1000);
  WiFiClient client = server.available();
  if (client) 
{ 
Serial.println("Web Client connected ");
String request = client.readStringUntil('\r'); 
client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("Connection: close");  // the connection will be closed after completion of the response
client.println("Refresh: 10");  // update the page after 10 sec
client.println();
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<style>html { font-family: Cairo; display: block; margin: 0px auto; text-align: center;color: #333333; background-color: ##f3ffee;}");
client.println("body{margin-top: 50px;}");
client.println("h1 {margin: 50px auto 30px; font-size: 50px;  text-align: center;}");
client.println(".side_adjust{display: inline-block;vertical-align: middle;position: relative;}");
client.println(".text1{font-weight: 180; padding-left: 5px; font-size: 50px; width: 170px; text-align: left; color: #3498db;}");
client.println(".data1{font-weight: 180; padding-left: 1px; font-size: 50px;color: #3498db;}");
client.println(".data{padding: 1px;}");
client.println("</style>");
client.println("</head>");
client.println("<body>");
client.println("<div id=\"webpage\">");   
client.println("<h1>Smart Container</h1>");
client.println("<div class=\"data\">");
client.println("<div class=\"side_adjust text1\">Status:</div>");
client.println("<div class=\"side_adjust data1\">");
client.print(level);
client.println("<div class=\"side_adjust text1\">% filled</div>");
client.println("</div>");
client.println("</div>");
client.println("</body>");
client.println("</html>");
//client.println("<h1>Level Indicator</h1>");
   if ( level <= 20) {
        send_event("jar_event");
        }
} 
}
void send_event(const char *event)
{
  Serial.print("Connecting to "); 
  Serial.println(host);
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("Connection failed");
    return;
  }
  // We now create a URI for the request
  String url = "/trigger/";
  url += event;
  url += "/with/key/";
  url += privateKey;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  while(client.connected())
  {
    if(client.available())
    {
      String line = client.readStringUntil('\r');
      Serial.print(line);
    } else {
      // No data yet, wait a bit
      delay(50);
    };
  }
  Serial.println();
  Serial.println("closing connection");
  client.stop();
}
