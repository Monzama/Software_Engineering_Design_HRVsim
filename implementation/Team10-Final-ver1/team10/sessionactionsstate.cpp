#include "sessionactionsstate.h"

SessionActionsState::SessionActionsState(QString items, QString title) : State(items, title)
{

}

void SessionActionsState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}
//state button press returns
int SessionActionsState::handlePressPowerButton(){return -1;}
int SessionActionsState::handlePressMenuButton(){return -1;}
int SessionActionsState::handlePressBackButton(){return -1;}
int SessionActionsState::handlePressLeftButton(){return -1;}
int SessionActionsState::handlePressRightButton(){return -1;}
int SessionActionsState::handlePressSelectorButton(){return -1;}
