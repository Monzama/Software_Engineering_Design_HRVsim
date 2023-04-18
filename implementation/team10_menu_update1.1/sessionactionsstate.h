#ifndef SESSIONACTIONSSTATE_H
#define SESSIONACTIONSSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class SessionActionsState: public State
{
public:
    SessionActionsState(QString items, QString title);
    void printSelection(QString selectedItem);

    //state button press returns
    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressLeftButton();
    int handlePressRightButton();
    int handlePressSelectorButton();
};

#endif // SESSIONACTIONSSTATE_H
