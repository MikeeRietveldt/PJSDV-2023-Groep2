#include "Servo.h"  // Deze library kan de servo op de deur aansturen
#include "ESP8266WiFi.h"
#include "Wire.h"  // Voor de I2C communicatie
#include "String.h"

const char* ssid = "BeamersWifi";   // Connectie naar de wifi SSID
const char* password = "112233ja";  // Password naar de wifi

bool isFlitsActief = false;
unsigned long vorigeFlitsTijd = 0;
const unsigned long flitsInterval = 500; // 500 milliseconden voor elke flits
int flitsTeller = 0;
const int maxFlitsen = 6; // 3 keer aan en 3 keer uit



unsigned long vorigeGedrukt;  // Voor de timer
int knopjes;

WiFiServer wifiServer(8080);  // Poort van de server


#define I2C_SDL D1
#define I2C_SDA D2

Servo myservo;

void setup() {
  myservo.attach(D5);    // Wijs de pin aan waarop de servo staat.
  Serial.begin(115200);  // Aantal baudrates
  Wire.begin();
  StartWifi();              // Hier wordt de wifi gestart, pas als deze verbonden is gaat hij door.
  initialisatie_knoppen();  // initialisatie van de I2C functie knoppen
  wifiServer.begin();       // start een server
  deurSluiten();            // sluit de deur direct
}


void loop() {
  // verbind de client aan de verbonden wifiserver
  WiFiClient client = wifiServer.available();

  // stukje voor de client verbinding
  if (client) {
        knopjes = knoppen();  // Read button states

        // Send the button state to the client
        if(knopjes != 0) {
            client.write('0' + knopjes); // Convert to char and send
        }


    // Wachten tot de client bytes vertstuurd
    // Ondertussen checken of de knop ingedrukt wordt
    while (client.available() == 0)
      ;

    char c = client.read();  // Lezen van het bericht dat de bewaker heeft gestuurd.
    Serial.write(c);         // Bewaker zijn reactie printen.
    switch (c) {
      case '0': deurSluiten(); break;
      case '1': deurOpenen(); break;
      case '2': startFlitsRoutine(); break;
      default: Serial.println("Bewaker heeft een ongeldig bericht meegegeven");
    }

    if (isFlitsActief) {
    if (millis() - vorigeFlitsTijd >= flitsInterval) {
      vorigeFlitsTijd = millis();

      if (flitsTeller % 2 == 0) {
        flitsAan();  // Implementeer deze functie om de flitser aan te zetten
      } else {
        flitsUit();  // Implementeer deze functie om de flitser uit te zetten
      }

      flitsTeller++;

      if (flitsTeller >= maxFlitsen) {
        isFlitsActief = false;
      }
    }
  }
    


    // stop de verbinding met de client en geef melding wanneer de client disconnect
    client.stop();
  }
}


// verbinding maken met wifi
void StartWifi() {
  IPAddress ip(192, 168, 137, 251);  // Static IP opzetten
  IPAddress gateway(192, 168, 137, 1); // Gateway opzetten
  IPAddress subnet(255, 255, 0, 0); // Subnet opzetten
  // statisch ip vaststellen
  Serial.println("Hier moet het gebeuren");
  if(!WiFi.config(ip, gateway, subnet)){
    Serial.println("Het gaat hartstikke mis");
    Serial.println(WiFi.status());
  }
  Serial.println("Hier zou het gebeurd moeten zijn");
  

  WiFi.begin(ssid, password);  // Start een wifi connectie op dit SSID en het Password


  // om de 2 seconde een Connecting.. bericht printen
  while (WiFi.status() != WL_CONNECTED) {
    delay(2000);
    Serial.println("Connecting...");
  }

  // geef een melding wanneer we verbonden zijn met de wifi en print het ip uit.
  Serial.print("Connected to WiFi connection with IP: ");
  Serial.println(WiFi.localIP());  // print het ip adress uit.
}


//Datasheet PCA9554A -- https://www.ti.com/lit/ds/symlink/pca9554.pdf?ts=1701616510905&ref_url=https%253A%252F%252Fwww.ti.com%252Fproduct%252FPCA9554%253Futm_source%253Dgoogle%2526utm_medium%253Dcpc%2526utm_campaign%253Dasc-null-null-GPN_EN-cpc-pf-google-wwe%2526utm_content%253DPCA9554%2526ds_k%253DPCA9554%2BDatasheet%2526DCM%253Dyes%2526gad_source%253D1%2526gclid%253DCj0KCQiA67CrBhC1ARIsACKAa8RMXkRB8i11iU4NZU7381eMy745RWiE6xRzJQhy8dROGHtH003oUy8aAsPGEALw_wcB%2526gclsrc%253Daw.ds
void initialisatie_knoppen() {
  //Inputs instellen (DI0 - DI3)
  Wire.beginTransmission(0x38);  // slave adres meegeven
  Wire.write(byte(0x03));        //command byte instellen (DDR register)
  Wire.write(byte(0x0F));        // Eerste 4 bits op 1 zetten (input)
  Wire.endTransmission();
}

unsigned int knoppen() {
  //Uitlezen van inputs D0 & D1
  Wire.beginTransmission(0x38);  // slave adres meegeven
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Wire.requestFrom(0x38, 1);                // 1 byte uitlezen (alle pinnen die op input staan)
  unsigned int inputWaardes = Wire.read();  // hier lees je waarde van knopje met I2C


  return inputWaardes & 0x03;  // return de waardes van de input
}

void deurOpenen() {
  int deurOpen = 175;  // Waarde waarop de deur netjes open staat

  myservo.write(deurOpen);  // Deur openen
  Serial.println("Deur geopent");

}

void deurSluiten() {
  int deurDicht = 70;  // Waarde waarop de deur netjes dicht is
  Serial.println("Deur gesloten.");
  myservo.write(deurDicht);  // Deur sluiten
}

//Routine starten
void startFlitsRoutine() {
  isFlitsActief = true;
  flitsTeller = 0;
  vorigeFlitsTijd = millis();
}

//Lichtje aan
void flitsAan() {
  //Waarde geven aan (IO4-IO7)
  Wire.beginTransmission(0x38); // slave adress
  Wire.write(byte(0x01)); // juiste register kiezen
  Wire.write(byte(3 << 4)); // waarde schrijven
  Wire.endTransmission(); // eindigen van transmissie
  Serial.println("flits");
}

//Lichtje uit
void flitsUit() {
  //Waarde geven aan (IO4-IO7)
  Wire.beginTransmission(0x38); // slave adress
  Wire.write(byte(0x01)); // juiste register kiezen
  Wire.write(byte(0)); // waarde schrijven
  Wire.endTransmission(); // eindigen van transmissie
}

