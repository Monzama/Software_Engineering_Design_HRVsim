#ifndef SESSIONSETTINGSSTATE_H
#define SESSIONSETTINGSSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class SessionSettingsState: public State
{
public:
    SessionSettingsState(QString items, QString title);
    void printSelection(QString selectedItem);
};

#endif // SESSIONSETTINGSSTATE_H
