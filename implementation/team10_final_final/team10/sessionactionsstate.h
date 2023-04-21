#ifndef SESSIONACTIONSSTATE_H
#define SESSIONACTIONSSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class SessionActionsState: public State{

public:
    
    SessionActionsState(QString items, QString title);

    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressSelectorButton();
    
};

#endif // SESSIONACTIONSSTATE_H
