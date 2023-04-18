
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

};

#endif // SESSIONSUMMARYSTATE_H
