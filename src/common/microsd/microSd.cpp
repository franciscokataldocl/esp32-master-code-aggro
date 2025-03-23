#include "microSd.h"

MicroSD::MicroSD() {}  // Constructor vacío, usa MICROSD_PIN automáticamente

bool MicroSD::begin() {
    if (!SD.begin(MICROSD_PIN)) {
        Serial.println(F("⚠️ SD CARD FAILED, OR NOT PRESENT!"));
        return false;
    }
    Serial.println(F("✅ SD CARD INITIALIZED."));
    return true;
}

bool MicroSD::writeToFile(const char* filename, const String &data) {
    File file = SD.open(filename, FILE_WRITE);
    if (!file) {
        Serial.print(F("⚠️ Error abriendo el archivo para escritura: "));
        Serial.println(filename);
        return false;
    }
    
    file.println(data);
    file.close();
    Serial.print(F("✅ Datos guardados en: "));
    Serial.println(filename);
    return true;
}

String MicroSD::readFromFile(const char* filename) {
    if (!SD.exists(filename)) {
        Serial.print(F("⚠️ El archivo no existe, creando: "));
        Serial.println(filename);
        
        File newFile = SD.open(filename, FILE_WRITE); // Crear archivo nuevo vacío
        if(newFile) {
            newFile.println("{}"); // Inicializar con contenido válido vacío (o ajusta según tu formato JSON)
            newFile.close();
            Serial.print(F("✅ Archivo creado: "));
            Serial.println(filename);
        } else {
            Serial.print(F("⚠️ Error creando el archivo: "));
            Serial.println(filename);
            return "";
        }
    }

    // Ahora sí puedes abrirlo en modo lectura
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        Serial.print(F("⚠️ Error abriendo el archivo para lectura: "));
        Serial.println(filename);
        return "";
    }

    String fileContent = "";
    while (file.available()) {
        fileContent += (char)file.read();
    }
    file.close();
    
    Serial.print(F("✅ Datos leídos de: "));
    Serial.println(filename);
    return fileContent;
}

bool MicroSD::deleteFile(const char* filename) {
    if (SD.exists(filename)) {
        SD.remove(filename);
        Serial.print(F("🗑️ Archivo eliminado: "));
        Serial.println(filename);
        return true;
    } else {
        Serial.print(F("⚠️ No se encontró el archivo: "));
        Serial.println(filename);
        return false;
    }
}
