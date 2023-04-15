#include "sessionactionsstate.h"

SessionActionsState::SessionActionsState(QString items, QString title) : State(items, title)
{

}

void SessionActionsState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}
