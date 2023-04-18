#include "sessionhistoryliststate.h"

SessionHistoryListState::SessionHistoryListState(QString items, QString title) : State(items, title)
{

}

void SessionHistoryListState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}

//state button press returns
int SessionHistoryListState::handlePressPowerButton(){return -1;}
int SessionHistoryListState::handlePressMenuButton(){return -1;}
int SessionHistoryListState::handlePressBackButton(){return -1;}
int SessionHistoryListState::handlePressLeftButton(){return -1;}
int SessionHistoryListState::handlePressRightButton(){return -1;}
int SessionHistoryListState::handlePressSelectorButton(){return -1;}
