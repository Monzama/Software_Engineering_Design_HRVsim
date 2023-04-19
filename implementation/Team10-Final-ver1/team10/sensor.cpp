#include "sensor.h"
#include <QMainWindow>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include <QListWidget>
#include <iostream>
#include"qcustomplot.h"
#include "state.h"
#include "offstate.h"
#include "mainmenustate.h"
#include "sessionactionsstate.h"
#include "sessionhistoryliststate.h"
#include "sessioninprogressstate.h"
#include "sessionsettingsstate.h"
#include <random>
#include "sessionsummarystate.h"
//initialize our variables (mainly the data set)
Sensor::Sensor(){
    currentDataPoint = 0;
    i = 39;
    status = 1;
}
//turns on HR contact sensor
void Sensor::connectSensor(){
    status = 1;
}
//generates and returns next HR data point
int Sensor::sendData(int breathRate){
    srand(time(NULL));
    int vLevel = (11-(breathRate*0.001));
    int vary = rand() %vLevel;
    if (i == 39){
        i = 43;
    }else{
        i = 39;
    }
    if (i==39){
        return i-vary +3*vLevel;
    }else{
        return i + vary+3*vLevel;
    }
}
//turns off HR contact sensor and returns null data(zeros)
void Sensor::simulateDisconnectSensor(){
    status = 0;
}

