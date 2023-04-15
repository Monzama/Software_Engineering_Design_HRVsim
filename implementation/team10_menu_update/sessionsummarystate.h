
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
};

#endif // SESSIONSUMMARYSTATE_H
