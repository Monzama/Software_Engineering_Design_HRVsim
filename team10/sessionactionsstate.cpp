#include "sessionactionsstate.h"

SessionActionsState::SessionActionsState(QString items, QString title) : State(items, title) {}

int SessionActionsState::handlePressPowerButton(){return -1;}
int SessionActionsState::handlePressMenuButton(){return -1;}
int SessionActionsState::handlePressBackButton(){return -1;}
int SessionActionsState::handlePressSelectorButton(){return -1;}
