#include <WiFi.h>
#include <HardwareSerial.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include <BluetoothSerial.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define TRIG_PIN 5
#define ECHO_PIN 18
#define SOIL_PIN 32

HardwareSerial gpsSerial(2);
TinyGPSPlus gps;

HardwareSerial gsmSerial(1);

BluetoothSerial SerialBT;

float temperature = 0;
float humidity = 0;
float distance = 0;
int soilMoisture = 0;
float lat = 0;
float lng = 0;

void setup() {
  Serial.begin(115200);
  
  dht.begin();
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);
  gsmSerial.begin(9600, SERIAL_8N1, 26, 27);
  
  SerialBT.begin("AgroRoboBot");
  
  WiFi.begin("SSID", "PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  
  soilMoisture = analogRead(SOIL_PIN);
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  distance = pulseIn(ECHO_PIN, HIGH) * 0.034 / 2;

  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
      lat = gps.location.lat();
      lng = gps.location.lng();
    }
  }

  String payload = String(temperature) + "," + 
                   String(humidity) + "," + 
                   String(soilMoisture) + "," + 
                   String(distance) + "," + 
                   String(lat, 6) + "," + 
                   String(lng, 6);

  SerialBT.println(payload);

  if (WiFi.status() == WL_CONNECTED) {
    // API logic omitted
  }

  sendSMS("+1234567890", "Status update: " + payload);
  
  delay(2000);
}

void sendSMS(String number, String text) {
  gsmSerial.println("AT+CMGF=1");
  delay(500);
  gsmSerial.println("AT+CMGS=\"" + number + "\"");
  delay(500);
  gsmSerial.print(text);
  delay(500);
  gsmSerial.write(26);
  delay(500);
}
