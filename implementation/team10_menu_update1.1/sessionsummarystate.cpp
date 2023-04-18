#include "sessionsummarystate.h"

SessionSummaryState::SessionSummaryState(QString items, QString title) : State(items, title)
{

}

void SessionSummaryState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}

//state button press returns
int SessionSummaryState::handlePressPowerButton(){return -1;}
int SessionSummaryState::handlePressMenuButton(){return -1;}
int SessionSummaryState::handlePressBackButton(){return -1;}
int SessionSummaryState::handlePressLeftButton(){return -1;}
int SessionSummaryState::handlePressRightButton(){return -1;}
int SessionSummaryState::handlePressSelectorButton(){return -1;}
