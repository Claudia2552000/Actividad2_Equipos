#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <math.h>

// Configuración del DHT22
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LCD: dirección I2C y tamaño
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Sensor de luz (LDR)
const int LDR_AO = A0;
const float GAMMA = 0.7;
const float RL10 = 50;

// Potenciómetros
const int POT_WIND = A1;
const int POT_AIR = A2;

// Pines del LED RGB calidad del aire
const int LED_R = 5;
const int LED_G = 6;
const int LED_B = 7;

// LEDs indicadores
const int LED_LUZ = 10;    // LED asociado al LDR
const int LED_FRIO = 8;    // LED azul (frío)
const int LED_CALOR = 9;   // LED rojo (calor)

// Pulsador
const int BUTTON_PIN = 4;

// Variables para controlar estados de los LED
bool frioActivo = false;
bool calorActivo = false;
bool luzActiva = false;

//Variables para controlar el tiempo de muestra en LCD
bool mostrarDatos = false;
unsigned long ultimaLectura = 0;
unsigned long tiempoMostrar = 10000; 

void setup() {
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_LUZ, OUTPUT);
  pinMode(LED_FRIO, OUTPUT);
  pinMode(LED_CALOR, OUTPUT);
  lcd.noBacklight();
}

void setLEDColor(String calidad) {
  if (calidad == "buena") {
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, HIGH);
  } else if (calidad == "regular") {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
    digitalWrite(LED_B, HIGH);
  } else if (calidad == "mala") {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
    digitalWrite(LED_B, HIGH);
  }
}

void loop() {
  // Lectura de temperatura y humedad
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();

  //Lectura y cálculo de los lux
  int analogValue = analogRead(LDR_AO);
  float voltage = analogValue / 1024.0 * 5;
  float resistance = 2000 * voltage / (1 - voltage / 5);
  float lux = pow(RL10 * 1000 * pow(10, GAMMA) / resistance, 1 / GAMMA);

  //lectura viento
  int potWind = analogRead(POT_WIND);
  float windSpeed = map(potWind, 0, 1023, 0, 200);

  //lectura calidad aire
  int airValue = analogRead(POT_AIR);
  int airQuality = map(airValue, 0, 1023, 0, 100);

  // calidad aire 
  String calidadTexto;
  if (airQuality < 33) calidadTexto = "mala";
  else if (airQuality < 66) calidadTexto = "regular";
  else calidadTexto = "buena";

  setLEDColor(calidadTexto);

  //Control de LED (luz, frio y calor)
  bool nuevoFrio = temp < 18;
  bool nuevoCalor = temp > 28;
  bool nuevaLuz = lux < 300;

// mostrar mensaje cuando cambia el estadado de clima
  if (nuevoFrio && !frioActivo) {
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Calefaccion");
    lcd.setCursor(0, 1);
    lcd.print("encendida");
    delay(3000);
    lcd.noBacklight();
    lcd.clear();
  }
  if (nuevoCalor && !calorActivo) {
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ventilador");
    lcd.setCursor(0, 1);
    lcd.print("encendido");
    delay(3000);
    lcd.noBacklight();
    lcd.clear();
  }
  if (nuevaLuz && !luzActiva) {
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Luz encendida");
    delay(3000);
    lcd.noBacklight();
    lcd.clear();
  }

  //actulizo el estado de los LED y variables
  frioActivo = nuevoFrio;
  calorActivo = nuevoCalor;
  luzActiva = nuevaLuz;

  digitalWrite(LED_FRIO, frioActivo);
  digitalWrite(LED_CALOR, calorActivo);
  digitalWrite(LED_LUZ, luzActiva);

  // Pulsador para mostrar datos en LCD
  if (digitalRead(BUTTON_PIN) == LOW) {
    mostrarDatos = true;
    ultimaLectura = millis();
    lcd.backlight();
  }

  if (mostrarDatos) {
    lcd.clear();
    if (isnan(temp) || isnan(hum)) {
      lcd.setCursor(0, 0);
      lcd.print("Error sensor DHT");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Temp:");
      lcd.print(temp, 1);
      lcd.print(" C");
      lcd.setCursor(0, 1);
      lcd.print("Hum:");
      lcd.print(hum, 0);
      lcd.print("%");
      delay(3000);

      lcd.clear();
      lcd.setCursor(0, 0);
      if (isfinite(lux)) {
        lcd.print("Luz:");
        lcd.print((int)lux);
        lcd.print(" lux");
      } else {
        lcd.print("Luz: Error");
      }
      lcd.setCursor(0, 1);
      lcd.print("Viento:");
      lcd.print((int)windSpeed);
      lcd.print("km/h");
      delay(3000);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Calidad del Aire:");
      lcd.setCursor(0, 1);
      lcd.print(calidadTexto);
      delay(3000);
    }
    mostrarDatos = false;
    lcd.clear();
    lcd.noBacklight();
  }

  delay(100); // Pequeño retardo para evitar sobrecarga de CPU
}






















