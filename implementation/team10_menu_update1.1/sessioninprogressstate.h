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
    void printSelection(QString selectedItem);

    //state button press returns
    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressLeftButton();
    int handlePressRightButton();
    int handlePressSelectorButton();
    void setSensor(bool enable);

    //session functions
    Session** createNewSession();//as it says, returns the session for reference in mainWindow
    int updateHRGraph(int breathRate);//returns next HR and updates session object
private:
    Sensor* contactSensor;
    Session* currentSession;
    int nextSessionId;//stores incremental session id
    QString startTime;
    QTimer sessionTimer;
    int hrTimer;//how many seconds have passed since last hr update (probably redundant)
    int coherenceTimer;//how many seconds have passed since last coherence calcultation

    //breath pacer timer removed, stored as a ui element since it is optional for user.
};

#endif // SESSIONINPROGRESSSTATE_H
