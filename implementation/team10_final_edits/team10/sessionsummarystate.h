
#ifndef SESSIONSUMMARYSTATE_H
#define SESSIONSUMMARYSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class SessionSummaryState: public State{

public:

    SessionSummaryState(QString items, QString title);

    int handlePressBackButton();
    int handlePressSelectorButton();

    void setDelete(bool enable);
    int setCurrentIndex(int i);
    int getCurrentIndex();
    
private:

    bool deleteOption;
    int currentIndex;
    
};

#endif // SESSIONSUMMARYSTATE_H
