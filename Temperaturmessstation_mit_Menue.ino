//#define S_LM35
//#define S_DS18B20
//#define S_DHT11

// -------------------------------------------------------------------------------------------------------------------------

// ----------LCD----------
#include <Wire.h> // Wire Bibliothek einbinden
#include <LiquidCrystal_I2C.h> // Vorher hinzugefügte LiquidCrystal_I2C Bibliothek einbinden
LiquidCrystal_I2C lcd(0x27, 16, 2); //Hier wird festgelegt um was für einen Display es sich handelt. In diesem Fall eines mit 16 Zeichen in 2 Zeilen und der HEX-Adresse 0x27. Für ein vierzeiliges I2C-LCD verwendet man den Code "LiquidCrystal_I2C lcd(0x27, 20, 4)" 

// ----------Taster----------
int TasteUP = 2,    // Pin vom Taster
    StatusTasteUP,    // Zustand vom Taster
    TasteDOWN = 3,
    StatusTasteDOWN,
    TasteMODE = 4,
    StatusTasteMODE,
    TasteENTER = 5,
    StatusTasteENTER;

// -------------------------------------------------------------------------------------------------------------------------
    
void TasterAuslesen(){
  StatusTasteUP = digitalRead(TasteUP);
  StatusTasteDOWN = digitalRead(TasteDOWN);
  StatusTasteMODE = digitalRead(TasteMODE);
  StatusTasteENTER = digitalRead(TasteENTER);
}

// -------------------------------------------------------------------------------------------------------------------------

int Hauptmenuezaehler = 1,
    Hauptmenue;

// -------------------------------------------------------------------------------------------------------------------------

void HauptMenu(){
  TasterAuslesen();
  
  if(StatusTasteMODE == HIGH){
    Hauptmenue = 1;
    delay(100);
    Hauptmenuezaehler++;
    if(Hauptmenuezaehler >= 5){
      Hauptmenuezaehler = 1;
    }
  }

  switch(Hauptmenuezaehler){
    case 1:
      SensorLM35();
      // ---
      if(StatusTasteMODE == HIGH){
        delay(100);
        Hauptmenuezaehler++;
        if(Hauptmenuezaehler >= 5){
          Hauptmenuezaehler = 1;
        }
        return;
      }
      break;
    case 2:
      SensorDS18B20();
      // ---
      if(StatusTasteMODE == HIGH){
        delay(100);
        Hauptmenuezaehler++;
        if(Hauptmenuezaehler >= 5){
          Hauptmenuezaehler = 1;
        }
        return;
      }
      break;
    case 3:
      SensorDHT();
      // ---
      if(StatusTasteMODE == HIGH){
        delay(100);
        Hauptmenuezaehler++;
        if(Hauptmenuezaehler >= 5){
          Hauptmenuezaehler = 1;
        }
        return;
      }
      break;
    case 4:
      // ---
      if(StatusTasteMODE == HIGH){
        delay(100);
        Hauptmenuezaehler++;
        if(Hauptmenuezaehler >= 5){
          Hauptmenuezaehler = 1;
        }
        return;
      }
      break;
     }
}

// -------------------------------------------------------------------------------------------------------------------------

// ----------LM35----------
#ifdef S_LM35
  int LM35 = A0; //Der Sensor soll am analogen Pin A0 angeschlossen werden. Wir nennen den Pin ab jetzt "LM35"
  int sensorwert;
  int temperatur = 0; //Unter der Variablen "temperatur" wird später der Temperaturwert abgespeichert.
  int t=500; //Der Wert für „t“ gibt im Code die zeitlichen Abstände zwischen den einzelnen Messungen vor.
#endif

// ----------DS18B20----------
#ifdef S_DS18B20
  #include <OneWire.h>
  #include <DallasTemperature.h>
  #define ONE_WIRE_BUS A0   // Pin für DS18B20
  OneWire oneWire(ONE_WIRE_BUS);
  DallasTemperature sensors(&oneWire);
#endif

// ----------DHT11----------
#ifdef S_DHT11
  #include "DHT.h" //DHT Bibliothek laden
  #define DHTPIN 2 //Der Sensor wird an PIN 2 angeschlossen    
  #define DHTTYPE DHT11    // Es handelt sich um den DHT11 Sensor
  DHT dht(DHTPIN, DHTTYPE); //Der Sensor wird ab jetzt mit „dth“ angesprochen
#endif

// -------------------------------------------------------------------------------------------------------------------------

void SensorLM35(){
  #ifdef S_LM35
    sensorwert=analogRead(LM35); //Auslesen des Sensorwertes.
    temperatur= map(sensorwert, 0, 307, 0, 150); //Umwandeln des Sensorwertes mit Hilfe des "map" Befehls.
    lcd.setCursor(0, 0);//Hier wird die Position des ersten Zeichens festgelegt. In diesem Fall bedeutet (0,0) das erste Zeichen in der ersten Zeile. 
    lcd.print("Temperatur: ");
    lcd.setCursor(0, 1); // In diesem Fall bedeutet (0,1) das erste Zeichen in der zweiten Zeile. 
    lcd.print(temperatur);
    lcd.print(" C");
    delay(t); // Nach jeder Messung ist je eine kleine Pause mit der Dauer „t“ in Millisekunden.
  #endif
}

// -------------------------------------------------------------------------------------------------------------------------

void SensorDS18B20(){
  #ifdef S_DS18B20
    sensors.requestTemperatures();
    lcd.setCursor(0, 0);//Hier wird die Position des ersten Zeichens festgelegt. In diesem Fall bedeutet (0,0) das erste Zeichen in der ersten Zeile. 
    lcd.print("Temperatur: ");
    lcd.setCursor(0, 1); // In diesem Fall bedeutet (0,1) das erste Zeichen in der zweiten Zeile. 
    lcd.print(sensors.getTempCByIndex(0));
    lcd.print(" C");
    delay(500);
  #endif
}

// -------------------------------------------------------------------------------------------------------------------------

void SensorDHT(){
  #ifdef S_DHT11
    delay(2000); //Zwei Sekunden Vorlaufzeit bis zur Messung (der Sensor ist etwas träge)
    
    float Luftfeuchtigkeit = dht.readHumidity(); //die Luftfeuchtigkeit auslesen und unter „Luftfeutchtigkeit“ speichern
    float Temperatur = dht.readTemperature();//die Temperatur auslesen und unter „Temperatur“ speichern
    
    lcd.setCursor(0,0);
    lcd.print("Luftfeu: ");
    lcd.print(Luftfeuchtigkeit);
    lcd.print("%");
    
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(Temperatur);
    lcd.print(" Grad");
  #endif
}

// -------------------------------------------------------------------------------------------------------------------------

void setup() {
  lcd.init(); //Im Setup wird der LCD gestartet 
  lcd.backlight(); //Hintergrundbeleuchtung einschalten (lcd.noBacklight(); schaltet die Beleuchtung aus). 

  #ifdef S_DS18B20
    sensors.begin();
  #endif

  #ifdef S_DHT11
    dht.begin(); //DHT11 Sensor starten
  #endif
}

// -------------------------------------------------------------------------------------------------------------------------

void loop() {
  HauptMenu();
}
