#include "sessionhistoryliststate.h"

SessionHistoryListState::SessionHistoryListState(QString items, QString title) : State(items, title) {}

int SessionHistoryListState::handlePressBackButton(){return 0;}
int SessionHistoryListState::handlePressSelectorButton(){return 3;}
