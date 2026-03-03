#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DHT.h>

// OLED Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define i2c_Address 0x3C // If screen is blank

// Create SH1106 display object
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT Setup
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Pins
const int buzzerPin = 9;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Initializing System..."));
  
  dht.begin();
  pinMode(buzzerPin, OUTPUT);

  if(!display.begin(i2c_Address, true)) { 
    Serial.println(F("SH1106 allocation failed"));
    for(;;); 
  }
  
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(20, 25);
  display.print(F("SYSTEM STARTING")); 
  display.display();
  
  delay(3000); 
  Serial.println(F("Setup Complete. Entering Loop."));
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Print to Serial for debugging
  Serial.print(F("Humidity: ")); Serial.print(h);
  Serial.print(F("%  |  Temp: ")); Serial.print(t); Serial.println(F("C"));

  // Check if sensor reading failed
  if (isnan(h) || isnan(t)) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(25, 25);
    display.print(F("SENSOR ERROR!"));
    display.display();
    return; 
  }

  // Update Display
  display.clearDisplay();
  
  // Header
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("ENV MONITOR"));
  display.drawFastHLine(0, 12, 128, SH110X_WHITE);

  // Temperature
  display.setTextSize(2);
  display.setCursor(0, 22);
  display.print(F("T: "));
  display.print(t, 1); 
  display.print(F(" C"));

  // Humidity
  display.setCursor(0, 47);
  display.print(F("H: "));
  display.print(h, 1);
  display.print(F(" %"));

  display.display();

  // Buzzer Logic
  if (h > 70.0) {
    tone(buzzerPin, 1000);
    delay(200);
    noTone(buzzerPin);
  } else {
    noTone(buzzerPin);
  }

  // DHT11 should not be polled more than once every 2 seconds
  delay(2000); 
}
