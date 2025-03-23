#ifndef SENSOR_COLOR_H
#define SENSOR_COLOR_H

#include <Arduino.h>

class SensorColor {
public:
    SensorColor();
    SensorColor(int s0, int s1, int s2, int s3, int out, int ledPin);
    void begin();
    void calibrate();
    void getRGB(int &r, int &g, int &b);
    void print();
    void setCalibration(int minRed, int minGreen, int minBlue, int maxRed, int maxGreen, int maxBlue);


private:
    int _s0, _s1, _s2, _s3, _out, _ledPin;
    int _minRed, _maxRed;
    int _minGreen, _maxGreen;
    int _minBlue, _maxBlue;
    unsigned long lastReadTime;

    int readRed();
    int readGreen();
    int readBlue();
};

#endif
