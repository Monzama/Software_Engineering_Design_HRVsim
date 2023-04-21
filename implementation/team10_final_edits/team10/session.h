#ifndef SESSION_H
#define SESSION_H

#include <QListWidget>

class Session{
    
public:
    
    Session(int id);

    ///may perform calculations for scores here, will remove the need for several set functions
    bool update(int HR = -1, float coherence = -1,float achievScore = -1); //updates session data

    QVector<float>* getHRData(); // returns the hr data list to be processed
    QVector<float>* getTotalCoherence(); // returns coherence list top be processed

    float getAvgCoherence();
    float getAchievScore();
    float getCurrentCoherence();

    int getLowTime();
    int getMedTime();
    int getHighTime();
    int getTime(); // returns elapsedTimeInSeconds
    int getID();
    QString getDate(); 
    void timeIncrement(int level); // increments elapsed time and level time

private:
    
    int id;
    int elapsedTimeInSeconds; // stores total session time
    QVector<float> heartRateData; // stores the list of reported HR data every second
    QVector<float> coherences; // stores list of coherence data every 5-seconds
    float averageCoherence;
    int lowTime, medTime, highTime; // time in seconds, at each coherence level
    float achievementScore;
    float currentCoherence;
    QString date;
    
};

#endif // SESSION_H
