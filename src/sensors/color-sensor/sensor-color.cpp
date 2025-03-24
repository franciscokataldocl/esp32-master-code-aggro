#include "sensor-color.h"
#include "../../config/config.h"



// 🔹 Constructor sin parámetros
SensorColor::SensorColor()


: _s0(SENSOR_S0), _s1(SENSOR_S1), _s2(SENSOR_S2), _s3(SENSOR_S3),
_out(SENSOR_OUT), _ledPin(SENSOR_LED), lastReadTime(0),
_minRed(0), _maxRed(1023),
_minGreen(0), _maxGreen(1023),
_minBlue(0), _maxBlue(1023) {}

// 🔹 Constructor con parámetros
SensorColor::SensorColor(int s0, int s1, int s2, int s3, int out, int ledPin)
    : _s0(s0), _s1(s1), _s2(s2), _s3(s3), _out(out), _ledPin(ledPin), lastReadTime(0),
      _minRed(0), _maxRed(1023),
      _minGreen(0), _maxGreen(1023),
      _minBlue(0), _maxBlue(1023)
{}

void SensorColor::begin() {
    pinMode(_s0, OUTPUT);
    pinMode(_s1, OUTPUT);
    pinMode(_s2, OUTPUT);
    pinMode(_s3, OUTPUT);
    pinMode(_out, INPUT);
    pinMode(_ledPin, OUTPUT);

    // Apagar los LEDs al inicio
    digitalWrite(_ledPin, LOW);

    // Configurar la escala de frecuencia al 20%
    digitalWrite(_s0, HIGH);
    digitalWrite(_s1, LOW);



  
}

void SensorColor::setCalibration(int minRed, int minGreen, int minBlue, int maxRed, int maxGreen, int maxBlue){
    _minRed = minRed;
    _minGreen = minGreen;
    _minBlue = minBlue;
    _maxRed = maxRed;
    _maxGreen = maxGreen;
    _maxBlue = maxBlue;
}



void SensorColor::calibrate() {
    Serial.println("🛠️ Iniciando calibración del sensor de color...");

    Serial.println("🔴 Paso 1: Cubre el sensor o apúntalo a una superficie negra.");
    esperarConIndicador(10000);  // Espera 10 seg

    _minRed = readRed();
    _minGreen = readGreen();
    _minBlue = readBlue();

    Serial.println("✅ Calibración en oscuridad completada.");
    Serial.print("   🔴 Min Red: "); Serial.println(_minRed);
    Serial.print("   🟢 Min Green: "); Serial.println(_minGreen);
    Serial.print("   🔵 Min Blue: "); Serial.println(_minBlue);

    Serial.println("\n⚪ Paso 2: Apunta el sensor a una hoja blanca bien iluminada.");
    esperarConIndicador(10000);  // Espera 10 seg

    _maxRed = readRed();
    _maxGreen = readGreen();
    _maxBlue = readBlue();

    Serial.println("✅ Calibración con luz blanca completada.");
    Serial.print("   🔴 Max Red: "); Serial.println(_maxRed);
    Serial.print("   🟢 Max Green: "); Serial.println(_maxGreen);
    Serial.print("   🔵 Max Blue: "); Serial.println(_maxBlue);

    Serial.println("\n🎯 Calibración finalizada correctamente.");

    
}


void SensorColor::print() {
    unsigned long currentTime = millis();

    if (currentTime - lastReadTime >= 10000) {
        lastReadTime = currentTime;

        int r, g, b;
        getRGB(r, g, b);

        Serial.print("🎨 RGB: ");
        Serial.print(r); Serial.print(", ");
        Serial.print(g); Serial.print(", ");
        Serial.println(b);
    }
}

void SensorColor::getRGB(int &r, int &g, int &b) {
    r = readRed();
    g = readGreen();
    b = readBlue();

    r = constrain(map(r, _minRed, _maxRed, 0, 255), 0, 255);
    g = constrain(map(g, _minGreen, _maxGreen, 0, 255), 0, 255);
    b = constrain(map(b, _minBlue, _maxBlue, 0, 255), 0, 255);
}

int SensorColor::readRed() {
    digitalWrite(_ledPin, HIGH);  // Enciende los LEDs
    delay(10);  // Pequeño retraso para estabilizar la luz

    digitalWrite(_s2, LOW);
    digitalWrite(_s3, LOW);
    int value = pulseIn(_out, LOW);

    digitalWrite(_ledPin, LOW);  // Apaga los LEDs
    return value;
}

int SensorColor::readGreen() {
    digitalWrite(_ledPin, HIGH);  // Enciende los LEDs
    delay(10);  

    digitalWrite(_s2, HIGH);
    digitalWrite(_s3, HIGH);
    int value = pulseIn(_out, LOW);

    digitalWrite(_ledPin, LOW);  // Apaga los LEDs
    return value;
}

int SensorColor::readBlue() {
    digitalWrite(_ledPin, HIGH);  // Enciende los LEDs
    delay(10);  

    digitalWrite(_s2, LOW);
    digitalWrite(_s3, HIGH);
    int value = pulseIn(_out, LOW);

    digitalWrite(_ledPin, LOW);  // Apaga los LEDs
    return value;
}

void SensorColor::esperarConIndicador(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) {
        delay(1);  // Deja respirar al sistema (WiFi, background, etc.)
        Serial.print(".");
    }
    Serial.println();  // Salto de línea al finalizar
}