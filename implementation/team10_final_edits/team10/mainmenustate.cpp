#include "mainmenustate.h"

MainMenuState::MainMenuState(QString items, QString title) : State(items, title) {}

int MainMenuState::handlePressPowerButton(){return 1;}
int MainMenuState::handlePressMenuButton(){return -1;}
int MainMenuState::handlePressBackButton(){return -1;}
int MainMenuState::handlePressSelectorButton(){return 0;}
