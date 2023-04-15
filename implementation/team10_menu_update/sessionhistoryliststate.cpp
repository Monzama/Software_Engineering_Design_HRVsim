#include "sessionhistoryliststate.h"

SessionHistoryListState::SessionHistoryListState(QString items, QString title) : State(items, title)
{

}

void SessionHistoryListState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}
