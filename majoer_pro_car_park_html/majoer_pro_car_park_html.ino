#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LiquidCrystal.h>

#include <Servo.h>

const char* ssid = "PARKING";  
const char* password = "0000";
IPAddress local_ip(192,168,0,1);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);

int servoPin1 = A2;
int servoPin2 = A3;
Servo Servo1;
Servo Servo2;

 LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int Ir1 = A0;
 int Ir1val = 0;
 float fltIrc1 = 0;
 const int Ir2 = A1;
 int Ir2val = 0;
 float fltIrc2 = 0;
 
const int LED1 = 8;
const int LED2 = 7;
const int LED3 = 13;
const int LED4 = 10;
const int LED5 = 9;
const int LED6 = 6;

void setup() 
{
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/SLOT1 AVA", handle_led1on);
  server.on("/SLOT1 OCC", handle_led1off);
  server.on("/SLOT2 AVA", handle_led2on);
  server.on("/SLOT2 OCC", handle_led2off);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
  
  pinMode(fltIrc1 , OUTPUT);
  pinMode(LED1 , OUTPUT);
  pinMode(LED2 , OUTPUT);
  pinMode(LED6 , OUTPUT);
  pinMode(fltIrc2 , OUTPUT);
  pinMode(LED3 , OUTPUT);
  pinMode(LED4 , OUTPUT);
  pinMode(LED5 , OUTPUT);
   lcd.begin(16,2);
   Serial.begin(9600);
   Servo1.attach(servoPin1);
   Servo2.attach(servoPin2);
}
  void loop()
{
  server.handleClient(); 
{
  lcd.setCursor(2,1);
        lcd.print(" welcome ");
  Ir1val = analogRead(Ir1);       // reads the value of the sharp sensor
  fltIrc1 = (6787.0/(Ir1val - 3.0)) -4.0;
  Serial.print(fltIrc1);
  Serial.println(" cm I1"); 
   
  delay(400);     // prints the value of the sensor to the serial monitor
  
  Ir2val = analogRead(Ir2);       // reads the value of the sharp sensor
  fltIrc2 = (6787.0/(Ir2val - 3.0)) -4.0;
  Serial.println(fltIrc2);  
  Serial.println(" cm I2");
  delay(400);     // prints the value of the sensor to the serial monitor
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  LED4status = LOW;
  LED5status = LOW;
  LED6status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,LED2status)); 
}
if ( fltIrc1 <= 30 )
  {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED6, LOW);
   lcd.print(" slot1 avail ");
    Servo1.write(90);
    delay(1000);
    void handle_SLOT1 AVAIL() {
  LED1status = HIGH;
  Serial.println("GPIO7 Status:AVAIL");
  server.send(200, "text/html", SendHTML(true,LED1status)); 
}
    
  }
   if ( fltIrc1 > 30 )
  {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED6, LOW);
    lcd.print(" slot1 no ");
    Servo1.write(90);
    Servo1.write(0);
    delay(1000);
    void handle_SLOT1 AVAIL() {
  LED1status = HIGH;
  Serial.println("GPIO7 Status:OCCUPIED");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}
    
  }
  if ( fltIrc1 <= 15) 
  { 
    digitalWrite(LED6, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED1, HIGH);
    lcd.setCursor(1,1);
        lcd.print(" Ir1caution ");
        void handle_SLOT1 AVAIL() {
  LED1status = HIGH;
  Serial.println("GPIO7 Status:BOOKED");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}
    
  }
  
  if ( fltIrc2 <= 30 )
  {
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
    Servo2.write(90);
    lcd.print(" slot2 avail ");
    delay(1000);
    
   
  }
   if ( fltIrc2 > 30 )
  {
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, LOW);
    Servo2.write(90);
    Servo2.write(0);
    lcd.print(" slot2 no ");
    delay(1000);
    
  }
   if (  fltIrc2 <= 15) 
  { 
    digitalWrite(LED5, HIGH);
    digitalWrite(LED4, LOW);
    digitalWrite(LED3, HIGH);
    lcd.print(" Ir2caution ");
    
  }
  
}


void handle_SLOT1 AVAIL() {
  LED1status = HIGH;
  Serial.println("GPIO7 Status:AVAIL");
  server.send(200, "text/html", SendHTML(true,LED2status)); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO7 Status: OFF");
  server.send(200, "text/html", SendHTML(false,LED2status)); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO6 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status,true)); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO6 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat)
{
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;
  text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #1abc9c;}\n";
  ptr +=".button-on:active {background-color: #16a085;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP8266 Web Server</h1>\n";
  if(led1stat)
  {ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";}
  else
  {ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";}

  if(led2stat)
  {ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";}
  else
  {ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}


























  
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
