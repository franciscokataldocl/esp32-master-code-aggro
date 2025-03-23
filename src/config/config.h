
#ifndef CONFIG_H
#define CONFIG_H


//HTTP DATA
#define BASE_URL "http://localhost:8080"

//MEMORY
#define EEPROM_SIZE 200

//DEVICES
#define WIFI_RESET_BUTTON_PIN 22
#define WIFI_RESET_BUTTON_PRESSED_TIME 3000

#define WIFI_LED_RED_PIN 17

//ESP32 GLOBALS
#define SERIAL_BEGIN 115200
#define ESP32_UNIQUE_ID_PART "aggro-esp32-"

//ESPACIOS DE MEMORIA CREDENCIALES WIFI
#define SSID_ADDRESS 0
#define PASSWORD_ADDRESS 40


// PINS DEL SENSOR DE COLOR
#define SENSOR_S0 25
#define SENSOR_S1 26
#define SENSOR_S2 27
#define SENSOR_S3 14
#define SENSOR_OUT 33
#define SENSOR_LED 32

//microsd
#define MICROSD_PIN 5


// Intervalos de tiempo
const unsigned long CHECK_INTERVAL = 10 * 1000;  // Verifica cada 10 segundos
const unsigned long RETRY_INTERVAL = 5 * 1000;   // Reintenta conectar cada 5 segundos
const unsigned long SMART_CONFIG_TIMEOUE = 30000; // 30 segundos para SmartConfig


#endif
