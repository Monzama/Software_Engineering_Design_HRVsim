#include "sessionsummarystate.h"

SessionSummaryState::SessionSummaryState(QString items, QString title) : State(items, title)
{

}

void SessionSummaryState::printSelection(QString selectedItem){
    qInfo() << "you selected = " << selectedItem;
}
