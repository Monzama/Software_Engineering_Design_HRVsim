#include "sessioninprogressstate.h"

SessionInProgressState::SessionInProgressState(QString items, QString title) : State (items, title){
    contactSensor = new Sensor();
    coherenceTimer = 0;
    totalSessions = 0;
    currentSession = NULL;
}

 SessionInProgressState::~SessionInProgressState(){
    delete contactSensor;
    currentSession = NULL;
}

int SessionInProgressState::handlePressPowerButton(){return 2;}
int SessionInProgressState::handlePressMenuButton(){return -1;}
int SessionInProgressState::handlePressBackButton(){return -1;}
int SessionInProgressState::handlePressSelectorButton(){return 1;}

int SessionInProgressState::updateHRGraph(int breathRate){
    //update any timers
    coherenceTimer++;
    //return our new HR from sensor
    int HR = contactSensor->sendData(breathRate);
    //update session with new HR
    currentSession->update(HR);
    //calculate coherence if 5-seconds have passed
    if (coherenceTimer ==5){
        float newCoherence = calculateCoherence(currentSession->getHRData());
        currentSession->update(-1, newCoherence);
    }
    //send HR to be displayed on graph
    return  HR;
}

//calculates coherence based on HR up to 64 seconds of data
float SessionInProgressState::calculateCoherence(QVector<float>* hrData){
    coherenceTimer =0;
    float avg = 0;
    int count=0;
    float variance= 0;
    for (int var = 0; var < 2; ++var) {
        int index = hrData->length()-1;
        //iterate over max of 64 seconds
        for (int x = 0;x <=63; ++x) {
            //if we reach the start of list, stop
            if (index <0){break;}
            if (var==0){
                count++;
                avg+= hrData->operator [](index);
            }else{
                float temp = hrData->operator [](index) -avg;
                variance+= temp*temp;
            }
            index--;
        }
        avg = avg/count;
    }
    variance= variance/count;
    return 30/(variance+1);
}

void SessionInProgressState::endSession(){
    currentSession = NULL;
    coherenceTimer =0;
}

void SessionInProgressState::setSensor(bool enable){
    if (enable){contactSensor->connectSensor();}
    else{contactSensor->simulateDisconnectSensor();}
}

Session** SessionInProgressState::createNewSession(){
    currentSession = new Session(totalSessions);
    totalSessions++;
    return &currentSession;
}
