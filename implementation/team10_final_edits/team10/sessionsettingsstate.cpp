#include "sessionsettingsstate.h"

SessionSettingsState::SessionSettingsState(QString items, QString title) : State(items, title){
    breathSetting = false;
    resetConfirmation = false;
}

void SessionSettingsState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}

//state button press returns
int SessionSettingsState::handlePressPowerButton(){return 1;}
int SessionSettingsState::handlePressMenuButton(){return 1;}

int SessionSettingsState::handlePressBackButton(){
    if (breathSetting||resetConfirmation){
        return 1;
    }
    return 0;
}

int SessionSettingsState::handlePressSelectorButton(){
    if (breathSetting){
        return 6;
    }
    if (resetConfirmation){
        return 7;
    }
    return 2;
}

void SessionSettingsState::setBreath(bool enable){
    breathSetting = enable;
}

void SessionSettingsState::setReset(bool enable){
    resetConfirmation = enable;
}
