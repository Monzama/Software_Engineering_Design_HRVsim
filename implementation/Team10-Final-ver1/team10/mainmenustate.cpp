#include "mainmenustate.h"

MainMenuState::MainMenuState(QString items, QString title) : State(items, title)
{

}

void MainMenuState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}

//state button press returns
int MainMenuState::handlePressPowerButton(){return 1;}
int MainMenuState::handlePressMenuButton(){return -1;}
int MainMenuState::handlePressBackButton(){return -1;}
int MainMenuState::handlePressLeftButton(){return -1;}
int MainMenuState::handlePressRightButton(){return -1;}
int MainMenuState::handlePressSelectorButton(){return 0;}
