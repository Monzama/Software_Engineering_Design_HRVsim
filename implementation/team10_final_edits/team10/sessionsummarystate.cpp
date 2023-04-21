#include "sessionsummarystate.h"

SessionSummaryState::SessionSummaryState(QString items, QString title) : State(items, title){
    deleteOption = false;
    currentIndex = -1;
}

int SessionSummaryState::handlePressPowerButton(){return 1;}
int SessionSummaryState::handlePressMenuButton(){return 1;}
int SessionSummaryState::handlePressBackButton(){
    if (deleteOption){return 3;}
    return 2;
}

int SessionSummaryState::handlePressSelectorButton(){
    if (deleteOption){return 5;}
    return 4;
}

void SessionSummaryState::setDelete(bool enable){
    deleteOption = enable;
}

int SessionSummaryState::getCurrentIndex(){return currentIndex;}
int SessionSummaryState::setCurrentIndex(int i){currentIndex = i;}
