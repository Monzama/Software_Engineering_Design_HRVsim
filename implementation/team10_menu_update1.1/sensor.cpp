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
#include "sessionsummarystate.h"
//initialize our variables (mainly the data set)
Sensor::Sensor(){
    currentDataPoint = 0;
    simData = {65,56,54,77,43,65,45,64,24};//simulated data set, may change to be read from a file or to random or to based on breathing rate
    i = simData.begin();
}
//turns on HR contact sensor
void Sensor::connectSensor(){
    status = 1;
}
//generates and returns next HR data point
int Sensor::sendData(int breathRate){
    if (status ==1){//send normal data
        if (i == simData.end()){
            i = simData.begin();
        }
        return *i++;
    }else if(status == 0){//send 0, sensor is disconnected
        return 0;
    }
    return -1;
}

//turns off HR contact sensor and returns null data(zeros)
void Sensor::simulateDisconnectSensor(){
    status = 0;
}

