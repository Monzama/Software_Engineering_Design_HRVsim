#include "sessioninprogressstate.h"

SessionInProgressState::SessionInProgressState(QString items, QString title) : State (items, title)
{

}

void SessionInProgressState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}
