#include "SoilMoistureSensor.h"

SoilMoistureSensor::SoilMoistureSensor(int pin, int dryValue, int wetValue, bool invert) {
    this->pin = pin;
    this->dryValue = dryValue;
    this->wetValue = wetValue;
    this->invert = invert;
    this->lastReadTime = 0;  // Inicializar tiempo de última lectura

}

int SoilMoistureSensor::readHumidity() {
    int rawValue = analogRead(pin);
    if (invert) {
        return constrain(map(rawValue, wetValue, dryValue, 100, 0), 0, 100);
    } else {
        return constrain(map(rawValue, dryValue, wetValue, 0, 100), 0, 100);
    }
}

float SoilMoistureSensor::readVoltage() {
    int rawValue = analogRead(pin);
    return (rawValue / 4095.0) * 3.3;
}
void SoilMoistureSensor::printValues() {
    int rawValue = analogRead(pin);
    float voltage = readVoltage();
    int humidity = readHumidity();

    Serial.print("Sensor en PIN ");
    Serial.print(pin);
    Serial.print(" -> RAW: ");
    Serial.print(rawValue);
    Serial.print(" | Voltaje: ");
    Serial.print(voltage);
    Serial.print("V | Humedad: ");
    Serial.print(humidity);
    Serial.println("%");
}

void SoilMoistureSensor::update(unsigned long interval) {
    unsigned long currentMillis = millis();
    if (currentMillis - lastReadTime >= interval) {
        lastReadTime = currentMillis;
        printValues();  // 📌 Solo imprime cuando pasan "interval" milisegundos
    }
}
