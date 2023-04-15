#include "sessionsettingsstate.h"

SessionSettingsState::SessionSettingsState(QString items, QString title) : State(items, title)
{

}

void SessionSettingsState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}
