#include <Wire.h>
#include "ESP8266WiFi.h"

#define I2C_SDL D1
#define I2C_SDA D2

unsigned int outputs = 0;
unsigned long timerStartTime = 0;
unsigned long timerDuration = 10000; // Set the timer duration in milliseconds (10 seconds)
const int resistorValue = 10000;  // Resistor value in ohms (e.g., 10k ohms at room temperature)
unsigned int tempsensor = 0;
unsigned int readAnalogSensor(int ANALOG_CH);
const int switchPin = 2;     // DI 0 - Connect switch to digital pin 2
const int fanPin = 4;        // DO 4 - Connect fan to digital pin 4
const int peltierPin = 5;    // D5 - Connect Peltier to digital pin 5
const int ntcPin0 = 0;      // AI 0 - Connect NTC 0 to analog pin A0
const int ntcPin1 = 1;      // AI 1 - Connect NTC 1 to analog pin A1
const int thresholdTemperature = 25;  // Adjust the threshold temperature as needed

const char* ssid = "Richardiscringe";
const char* password = "minecraft123";
WiFiServer server(8080);

void setup() {
  Serial.begin(9600);
  delay(10);
  pinMode(switchPin, INPUT);
  pinMode(D4, OUTPUT);
  pinMode(peltierPin, OUTPUT);
  IPAddress ip(192, 168, 137, 251);  // Static IP opzetten
  IPAddress gateway(192, 168, 137, 1); // Gateway opzetten
  IPAddress subnet(255, 255, 0, 0); // Subnet opzetten
  Wire.begin();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  server.begin();
   WiFi.config(ip, gateway, subnet);
  Serial.println("Server started.");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
// flash mosfet output
  digitalWrite(D4, HIGH);
  delay(1000);
  digitalWrite(D4, LOW);
  delay(1000);
  tempsensor = readAnalogSensor(0);
  Serial.println(tempsensor);
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x03));
  Wire.write(byte(0x0F));
  if (tempsensor > 320) {
     client.println("b");
      Wire.write(byte(0x1F));
   }
  Wire.endTransmission();
     
  Wire.beginTransmission(0x38);
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);
  unsigned int inputs = Wire.read();

   if ((inputs & 0x01) == 0x01) {  // DI 0 is pressed
     client.println("a");

    delay(500); // Debounce delay
   }
  Wire.beginTransmission(0x36);
  Wire.write(byte(0xA2));
  Wire.write(byte(0x03));
  Wire.endTransmission();
  }
}

unsigned int readAnalogSensor(int ANALOG_CH) {
  unsigned int sensorValue0;
  unsigned int sensorValue1;
  Wire.requestFrom(0x36, 4);
  // Lees MAX11647

  if (ANALOG_CH == 0) {
    sensorValue0 = Wire.read() & 0x03;
    sensorValue0 = sensorValue0 << 8;
    sensorValue0 = sensorValue0 | Wire.read();
    return sensorValue0;
  }
  if (ANALOG_CH == 1) {
    Wire.read();
    Wire.read();
    sensorValue1 = Wire.read() & 0x03;
    sensorValue1 = sensorValue1 << 8;
    sensorValue1 = sensorValue1 | Wire.read();
    return sensorValue1;
  }
  return 0;
}
    //Close the connection
    // client.stop();
    // Serial.println("Client disconnected.");