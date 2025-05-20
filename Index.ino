#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>
#include "index.h"

int enA = D1;
int in1 = D2;
int in2 = D3;

int in3 = D4;
int in4 = D5;
int enB = D6;

String command;
int SPEED = 1023;
int speed_Coeff = 3;

const int trigPin = D7;
const int echoPin = D8;
int distanceCm = 0;
int distanceHold = 20;

const char* ssid = "ESP8266_AP";
const char* password = "12345678";

ESP8266WebServer server(80);

void HOMEPAGE() {
  String page = FPSTR(MAIN_PAGE);
  server.send(200, "text/html", page);
  if( server.hasArg("State") ){
     Serial.println(server.arg("State"));
  }
}

void CONTROL() {
  server.send(200, "text/html", "ok");
  if( server.hasArg("State") ){
     Serial.println(server.arg("State"));
  }
}


void setup() {
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // Set the speed to start, from 0 (off) to 1023 (max speed)
  analogWrite(enA, SPEED);
  analogWrite(enB, SPEED);
  
  // motor mati pada saan pertama kali di aktifkan
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);


  Serial.begin(115200);

  // Membuat access point
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // OTA update
  
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_SPIFFS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });

  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  ArduinoOTA.begin();

  // Inisialisasi mDNS
  if (MDNS.begin("controler")) {
    Serial.println("MDNS responder started");
  }
  
  // Routing HTTP
  server.on("/", HOMEPAGE);
  server.on("/control", CONTROL);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  ArduinoOTA.handle();
  
  MDNS.update();
  server.handleClient();
  
  //mengisi String command
  command = server.arg("State");
  
  // mengecek command dan menjalankan sebuah function
      if (command == "F") Forward();
      else if (command == "B") Backward();
      else if (command == "R") TurnRight();
      else if (command == "L") TurnLeft();
      else if (command == "S") Stop();
      else if (command == "A") ObsAvoidance();
      else if (command == "0") SPEED = 330;
      else if (command == "1") SPEED = 400;
      else if (command == "2") SPEED = 470;
      else if (command == "3") SPEED = 540;
      else if (command == "4") SPEED = 610;
      else if (command == "5") SPEED = 680;
      else if (command == "6") SPEED = 750;
      else if (command == "7") SPEED = 820;
      else if (command == "8") SPEED = 890;
      else if (command == "9") SPEED = 960;
      else if (command == "10") SPEED = 1023;
}

void Forward(){
  analogWrite(enA, SPEED);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, SPEED);
}

// function to move backward
void Backward(){
  analogWrite(enA, SPEED);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, SPEED);
}

// function to turn right
void TurnRight(){
  analogWrite(enA, SPEED);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, SPEED);
}

// function to turn left
void TurnLeft(){
  analogWrite(enA, SPEED);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, SPEED);
}

// function to stop
void Stop(){  
  analogWrite(enA, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}

void ObsAvoidance() {
  static unsigned long lastCheck = 0;
  unsigned long currentMillis = millis();

  // Periksa jarak setiap 100 ms
  if (currentMillis - lastCheck >= 100) {
    lastCheck = currentMillis;
    distanceCm = getDistance();
    Serial.println(distanceCm);
  }

  if (distanceCm <= distanceHold && distanceCm > 0) {
    Stop(); // Berhenti sebentar sebelum mengambil tindakan
    delay(200);
    TurnLeft(); // Anda bisa menambahkan logika alternatif seperti TurnRight()
    delay(1000);
  } else {
    Forward();
  }
}


int getDistance() {
  int echoTime;
  int calculatedDistance;
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  echoTime = pulseIn(echoPin, HIGH, 20000); // Timeout setelah 20ms
  if (echoTime == 0) {
    // Jika tidak ada pantulan, anggap jarak sangat jauh
    return -1;
  }

  calculatedDistance = echoTime / 58.26;
  return calculatedDistance;
}