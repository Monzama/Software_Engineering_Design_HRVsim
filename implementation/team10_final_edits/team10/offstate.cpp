#include "offstate.h"

OffState::OffState(QString items, QString title) : State (items, title) {}

int OffState::handlePressPowerButton(){return 0;}
int OffState::handlePressMenuButton(){return -1;}
int OffState::handlePressBackButton(){return -1;}
int OffState::handlePressSelectorButton(){return -1;}
