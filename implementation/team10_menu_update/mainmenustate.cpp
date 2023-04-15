#include "mainmenustate.h"

MainMenuState::MainMenuState(QString items, QString title) : State(items, title)
{

}

void MainMenuState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}
