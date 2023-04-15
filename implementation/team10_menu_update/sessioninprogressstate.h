#ifndef SESSIONINPROGRESSSTATE_H
#define SESSIONINPROGRESSSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class SessionInProgressState: public State
{
public:
    SessionInProgressState(QString items, QString title);
    void printSelection(QString selectedItem);
};

#endif // SESSIONINPROGRESSSTATE_H
