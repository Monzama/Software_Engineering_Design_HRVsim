#ifndef OFFSTATE_H
#define OFFSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class OffState: public State{

public:
    
    OffState(QString items, QString title);

    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressSelectorButton();

};

#endif // OFFSTATE_H
