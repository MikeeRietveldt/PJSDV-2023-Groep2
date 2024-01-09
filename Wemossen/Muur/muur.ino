#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <FastLED.h>

#define I2C_SDL    D1
#define I2C_SDA    D2
#define NUM_LEDS 3 //benodigd voor de WS2812
const char* ssid = "FSOC";
const char* password = "fsociety";
WiFiServer wifiServer(1234);
#define ledstrip D4
CRGB leds[NUM_LEDS];

unsigned int anin0;

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

void setLED(int x, int y, int z) {
  FastLED.setBrightness(100);
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(x, y, z);
//    Serial.print("led:");
//    Serial.print(i);
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


void loop(void) {
  // Create a client that will connect to the server
  WiFiClient client = wifiServer.available();

  int teller = 0;
  // Flash mosfet output
  digitalWrite(14, HIGH);
  delay(100);
  digitalWrite(14, LOW);
  delay(100);

  readLDR();

  while (client.connected()) {
    // If a client is connected
    Serial.println("Client connected");
    Serial.println(teller);
    readLDR();
    client.println(anin0);
    Serial.println(anin0);

    // Set a timeout for reading from the client (e.g., 5 seconds)
    //client.setTimeout(5000);

    // Read client input
    String input = client.readStringUntil('\n');

    // Check if input is non-empty
    if (!input.isEmpty()) {
      // Perform actions based on client input
      Serial.println(input);

      // Extract three integer values from the received string
      int ldr, strip, rgb;
      if (sscanf(input.c_str(), "%d,%d,%d", &ldr, &strip, &rgb) == 3) {
        // Do something with the three values
        Serial.print("Received values: ");
        Serial.print(ldr);
        Serial.print(", ");
        Serial.print(strip);
        Serial.print(", ");
        Serial.println(rgb);
        ledStatus(strip);
        actLdr(ldr);
        if (ldr == 0){
          rgbValue(rgb);
        }
      } else {
        Serial.println("Invalid input format");
      }

      client.flush();
    }
    teller++;
  }

  delay(500);
}