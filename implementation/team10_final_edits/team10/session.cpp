#include "session.h"
#include <QDateTime>
//constructor
Session::Session(int id)
{    
    elapsedTimeInSeconds=0;
    currentCoherence = 0;
    achievementScore =0;
    lowTime = 0;
    medTime = 0;
    highTime = 0;
    this->id = id;
    QDateTime d;
    date = d.currentDateTime().toString("dd.MM.yyyy  h:mm:ss ap");
}

///may perform calculations for scores here, will remove the need for several set functions
//updates session data, coherence and ahivescore are optional, default -1 otherwise
bool Session::update(int HR, float coherence,float achievScore){
    //add hr data to list
    if (HR!=-1){heartRateData.push_back(HR);}
    //may calculate coherence here
    //add coherence to list if there is one included in call
    if (coherence!=-1){
        coherences.push_back(coherence);
        currentCoherence = coherence;
        achievementScore+=coherence;
    }

    //re-calculate averages

    //update achievement score if included in call
    if (achievScore!=-1){achievementScore = achievScore;}//may need to store this in a list as well
    return true;
}

void Session::timeIncrement(int level){
    elapsedTimeInSeconds++;
    switch (level) {
    case 0:
        lowTime++;
        break;
    case 1:
        medTime++;
        break;
    case 2:
        highTime++;
        break;
    }
}

//returns the hr data list to be processed
QVector<float>* Session::getHRData(){return &heartRateData;}

//returns coherence list top be processed
QVector<float>* Session::getTotalCoherence(){return &coherences;}

//returns averageCoherence
float Session::getAvgCoherence(){return averageCoherence;}

//returns achievementScore
float Session::getAchievScore(){return achievementScore;}

//returns most recent coherence data
float Session::getCurrentCoherence(){return currentCoherence;}

//returns lowTime
int Session::getLowTime(){return lowTime;}

//returns id
int Session::getID(){return id;}

//returns medTime
int Session::getMedTime(){return medTime;}

//returns highTime
int Session::getHighTime(){return highTime;}

//returns elapsedTimeInSeconds
int Session::getTime(){return  elapsedTimeInSeconds;}

QString Session::getDate(){return date;}
