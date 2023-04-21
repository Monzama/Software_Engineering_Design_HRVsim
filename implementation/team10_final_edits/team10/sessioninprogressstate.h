#ifndef SESSIONINPROGRESSSTATE_H
#define SESSIONINPROGRESSSTATE_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include <QListWidget>
#include <iostream>
#include <QString>
#include <QDebug>
#include "state.h"
#include "session.h"
#include "sensor.h"

class SessionInProgressState: public State
{
public:

    SessionInProgressState(QString items, QString title);

    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressSelectorButton();
    void setSensor(bool enable);
    void endSession();

    Session** createNewSession(); 
    int updateHRGraph(int breathRate); //returns next HR, updates session object
    
private:

    float calculateCoherence(QVector<float>* hrData);

    Sensor* contactSensor;
    Session* currentSession;
    int nextSessionId;
    QString startTime;
    QTimer sessionTimer;
    int totalSessions;
    int coherenceTimer; //seconds passed since last coherence calcultation

};

#endif // SESSIONINPROGRESSSTATE_H
