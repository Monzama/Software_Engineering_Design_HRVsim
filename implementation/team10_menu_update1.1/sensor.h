#ifndef SENSOR_H
#define SENSOR_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include <QListWidget>
#include <iostream>
class Sensor
{
public:
    Sensor();
    //turns on HR contact sensor
    void connectSensor();
    //generates and returns next HR data point
    int sendData(int breathRate);
    //turns off HR contact sensor and returns null data(zeros)
    void simulateDisconnectSensor();


private:
    int currentDataPoint;
    int status;
    QVector<int> simData;
    QVector<int>::iterator i;
};

#endif // SENSOR_H
