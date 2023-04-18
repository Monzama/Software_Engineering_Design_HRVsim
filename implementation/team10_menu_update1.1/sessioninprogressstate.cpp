#include "sessioninprogressstate.h"

SessionInProgressState::SessionInProgressState(QString items, QString title) : State (items, title)
{
    contactSensor = new Sensor();
}

void SessionInProgressState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}

//state button press returns
int SessionInProgressState::handlePressPowerButton(){return -1;}
int SessionInProgressState::handlePressMenuButton(){return -1;}
int SessionInProgressState::handlePressBackButton(){return -1;}
int SessionInProgressState::handlePressLeftButton(){return -1;}
int SessionInProgressState::handlePressRightButton(){return -1;}
int SessionInProgressState::handlePressSelectorButton(){return -1;}
int SessionInProgressState::updateHRGraph(int breathRate){
    //change the session data,

    //return our new HR from sensor
    int HR = contactSensor->sendData(breathRate);

    //perform our calculations for all vars


    //update session with new data
    currentSession->update(HR);

    //send HR to be displayed on graph
    return  HR;
}

void SessionInProgressState::setSensor(bool enable){
    if (enable){contactSensor->connectSensor();}
    else{contactSensor->simulateDisconnectSensor();}
}

Session** SessionInProgressState::createNewSession(){
    currentSession = new Session();
    return &currentSession;
}


