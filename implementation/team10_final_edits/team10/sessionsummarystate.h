
#ifndef SESSIONSUMMARYSTATE_H
#define SESSIONSUMMARYSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class SessionSummaryState: public State
{
public:
    SessionSummaryState(QString items, QString title);
    void printSelection(QString selectedItem);

    //state button press returns
    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressLeftButton();
    int handlePressRightButton();
    int handlePressSelectorButton();

    void setDelete(bool enable);
    int setCurrentIndex(int i);
    int getCurrentIndex();//return the index of the viewed session
private:
    bool deleteOption;
    int currentIndex;
};

#endif // SESSIONSUMMARYSTATE_H
