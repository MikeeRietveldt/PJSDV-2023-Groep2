#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <FastLED.h>
#include <ArduinoJson.h>

#define I2C_SDL    D1
#define I2C_SDA    D2
#define NUM_LEDS 3 //benodigd voor de WS2812
const char* ssid = "FSOC";
const char* password = "fsociety";
WiFiServer wifiServer(8080);
#define ledstrip D4
CRGB leds[NUM_LEDS];

unsigned int anin0, anin1;

void setup(void) {
  pinMode(A0, INPUT);
  Wire.begin();
  Serial.begin(115200);
  StartWifi();
  wifiServer.begin();
  FastLED.addLeds<WS2812, 14, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.clear();
  FastLED.show();
  leds[0] = CRGB(100, 0, 0);
  leds[1] = CRGB(100, 0, 0);
  leds[2] = CRGB(100, 0, 0);
}

void StartWifi() {
  // Connect to Wi-Fi
  IPAddress host(192, 168, 137, 248);
  IPAddress gateway(192, 168, 137, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.begin(ssid, password);
  if (!WiFi.config(host, gateway, subnet)) {
    Serial.println("Not connected");
    delay(1000);
  }
  Serial.println("Connected to WiFi Local IP = ");
  Serial.println(WiFi.localIP());
}

void readLDR() {
  // Read LDR sensor value
  Wire.requestFrom(0x36, 4);
  anin0 = Wire.read() & 0x03;
  anin0 = anin0 << 8;
  anin0 = anin0 | Wire.read();
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

}

void readPotent(){
  // Read LDR sensor value
  Wire.requestFrom(0x36, 4);
  Wire.read();
  Wire.read();
  anin1 = Wire.read() & 0x03;
  anin1 = anin1 << 8;
  anin1 = anin1 | Wire.read();
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  Wire.endTransmission();

}

void setLED(int x, int y, int z) {
  FastLED.setBrightness(100);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(x, y, z);
    FastLED.show();
  }
}

void actLdr(int value){
  if (value == 1){
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(1 << 4));
    Wire.endTransmission();  
    setLED(0,0,0);
    FastLED.show();
    Serial.print("Het is nacht (LED uit)");
  }
  else if(value == 0){
    Wire.beginTransmission(0x38);
    Wire.write(byte(0x01));
    Wire.write(byte(0 << 4));
    Wire.endTransmission();
    Serial.print("Het is dag");
  }
  else {
    Serial.println("Error getting ldr value");
  }
}

void actPot(int value) {
  if (value == 5) {
    FastLED.setBrightness(100);
    setLED(255, 255, 255);
    FastLED.show();
  } else if (value == 4) {
    FastLED.setBrightness(80);
    setLED(200, 200, 200);
    FastLED.show();
  } else if (value == 3) {
    FastLED.setBrightness(60);
    setLED(150, 150, 150);
    FastLED.show();
  } else if (value == 2) {
    FastLED.setBrightness(40);
    setLED(100, 100, 100);
    FastLED.show();
  } else if (value == 1) {
    FastLED.setBrightness(20);
    setLED(50, 50, 50);
    FastLED.show();
  } else if (value == 0) {
    FastLED.setBrightness(0);
    setLED(0, 0, 0);
    FastLED.show();
  } else {
    Serial.println("Error getting pot value");
  }
}

void ledStatus(int value) {
  if (value == 1){
    setLED(255,255,255);
    FastLED.show();
  }
  else if (value == 0){
    setLED(0,0,0);
    FastLED.show();
  }
  else {
    Serial.println("Error getting ledStatus value");
  }
}

void rgbValue(int value){
  if(value == 0){
    setLED(0,0,0);
    FastLED.show();
  }
  else if (value == 1){
    setLED(0,255,0);
    FastLED.show();
  }
  else if (value == 2){
    setLED(0,0,255);
    FastLED.show();
  }
  else if (value == 3){
    setLED(255,0,0);
    FastLED.show();
    delay(500);
    setLED(0,255,0);
    FastLED.show();
    delay(500);
    setLED(0,0,255);
    FastLED.show();
    delay(500);
    setLED(255,0,0);
    FastLED.show();
    delay(500);
    setLED(0,255,0);
    FastLED.show();
    delay(500);
    setLED(0,0,255);
    FastLED.show();
    delay(500);
    setLED(100,100,0);
    FastLED.show();
    delay(500);
    setLED(255,255,255);
    FastLED.show();
    delay(1000);
    setLED(100,100,0);
    FastLED.show();
    delay(500);
    setLED(0,100,100);
    FastLED.show();
    delay(500);
    setLED(100,0,100);
    FastLED.show();
    delay(500);
    setLED(255,255,255);
    FastLED.show();
    delay(500);
  }  
  else if (value == 4){
    setLED(255,255,255);
    FastLED.show();    
  }  
  else {
    Serial.println("Error getting rgb value");
  }
}

int splitString(String input, char delimiter, String values[], int maxValues) {
  int valueCount = 0;
  int start = 0;
  int end = input.indexOf(delimiter);

  while (end >= 0 && valueCount < maxValues) {
    values[valueCount++] = input.substring(start, end);
    start = end + 1;
    end = input.indexOf(delimiter, start);
  }

  // Add the last part of the string
  if (start < input.length() && valueCount < maxValues) {
    values[valueCount++] = input.substring(start);
  }

  return valueCount;
}



void loop(void) {
  // Create a client that will connect to the server
  WiFiClient client = wifiServer.available();
  int ldr, pot, strip, rgb;
  int teller = 0;

  // Flash mosfet output
  digitalWrite(14, HIGH);
  delay(100);
  digitalWrite(14, LOW);
  delay(100);

  readLDR();
  readPotent();
//   Serial.println(anin0);
//   Serial.println(anin1);

  while (client.connected()) {
    int d = ldr;
    // If a client is connected
    Serial.println("Client connected");
    Serial.println(teller);
    DynamicJsonDocument jsonDoc(256);
    jsonDoc["ldr"] = anin0;
    jsonDoc["pot"] = anin1;

    // Serialize the JSON object to a string
    String jsonString;
    serializeJson(jsonDoc, jsonString);

    // Send the JSON string to the client
    client.println(jsonString);
    Serial.println(jsonString);

//    Serial.println(anin0);
//    Serial.println("meter");
//    Serial.println(anin1);

    // Read client input
    String input = client.readStringUntil('\n');

    // Check if input is non-empty
     if (!input.isEmpty()) {
      int ldr, pot, strip, rgb;

      // Split the input into an array of strings using ',' as the delimiter
      String values[4];
      int count = splitString(input, ',', values, 4);

      // Check the number of values obtained
      if (count == 2) {
        // Two values received (ldr and pot)
        ldr = values[0].toInt();
        Serial.print("ldr is: ");
        Serial.println(ldr);
        pot = values[1].toInt();
        Serial.println("pot is: ");
        Serial.println(pot);
        actLdr(ldr);
        if(ldr == 0){
          actPot(pot);
        }
      } else if (count == 4) {
        // Four values received (ldr, pot, strip, and rgb)
        ldr = values[0].toInt();
        Serial.println("4 waarden doorgaan");
        Serial.println(input);
        pot = values[1].toInt();
        strip = values[2].toInt();
        rgb = values[3].toInt();
        actLdr(d);
        actPot(pot);
        ledStatus(strip);
        if (d == 0) {
          rgbValue(rgb);
        }
      }
        else {
          Serial.println(input);
  
        
      }
    }
    client.flush();
    teller++;
  }
}


