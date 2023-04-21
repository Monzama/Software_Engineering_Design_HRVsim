#ifndef SESSIONSETTINGSSTATE_H
#define SESSIONSETTINGSSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class SessionSettingsState: public State
{
public:

    SessionSettingsState(QString items, QString title);

    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressSelectorButton();

    void setReset(bool enable);
    void setBreath(bool enable);
    
private:

    bool breathSetting;
    bool resetConfirmation;
    
};

#endif // SESSIONSETTINGSSTATE_H
