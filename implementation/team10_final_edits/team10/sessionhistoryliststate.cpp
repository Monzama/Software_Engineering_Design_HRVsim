#include "sessionhistoryliststate.h"

SessionHistoryListState::SessionHistoryListState(QString items, QString title) : State(items, title) {}

int SessionHistoryListState::handlePressPowerButton(){return 1;}
int SessionHistoryListState::handlePressMenuButton(){return 1;}
int SessionHistoryListState::handlePressBackButton(){return 0;}
int SessionHistoryListState::handlePressSelectorButton(){return 3;}
