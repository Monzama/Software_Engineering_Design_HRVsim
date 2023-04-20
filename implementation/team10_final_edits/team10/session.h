#ifndef SESSION_H
#define SESSION_H

#include <QListWidget>
class Session
{
public:
    Session(int id);

    ///may perform calculations for scores here, will remove the need for several set functions
    bool update(int HR=-1, float coherence = -1,float achievScore = -1);//updates session data


    QVector<float>* getHRData();//returns the hr data list to be processed
    QVector<float>* getTotalCoherence();//returns coherence list top be processed

    float getAvgCoherence();//returns averageCoherence
    float getAchievScore();//returns achievcementScore
    float getCurrentCoherence();//returns most recent coherence data

    int getLowTime();//returns lowTime
    int getMedTime();//returns medTime
    int getHighTime();//returns highTime
    int getTime();//returns elapsedTimeInSeconds
    int getID();//returns id
    QString getDate();//returns session date
    void timeIncrement(int level);//increments both lepsed time and level time

private:
    int id;
    int elapsedTimeInSeconds;//stores total session time
    QVector<float> heartRateData;//stores the list of reported HR data every second
    QVector<float> coherences;//stores list of coherence data every 5-seconds
    float averageCoherence;//as it says
    int lowTime, medTime, highTime; //time in seconds, at each coherence level
    float achievementScore;//as it says
    float currentCoherence;//as it says
    QString date;
};

#endif // SESSION_H
