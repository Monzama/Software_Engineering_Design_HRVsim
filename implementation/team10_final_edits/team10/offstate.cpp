#include "offstate.h"

OffState::OffState(QString items, QString title) : State (items, title)
{

}

void OffState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}
//state button press returns
int OffState::handlePressPowerButton(){return 0;}
int OffState::handlePressMenuButton(){return -1;}
int OffState::handlePressBackButton(){return -1;}
int OffState::handlePressLeftButton(){return -1;}
int OffState::handlePressRightButton(){return -1;}
int OffState::handlePressSelectorButton(){return -1;}
