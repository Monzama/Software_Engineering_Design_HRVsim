#ifndef SESSIONHISTORYLISTSTATE_H
#define SESSIONHISTORYLISTSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class SessionHistoryListState: public State{

public:
    
    SessionHistoryListState(QString items, QString title);
    void printSelection(QString selectedItem);

    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressSelectorButton();

};

#endif // SESSIONHISTORYLISTSTATE_H
