#include "offstate.h"

OffState::OffState(QString items, QString title) : State (items, title)
{

}

void OffState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}
