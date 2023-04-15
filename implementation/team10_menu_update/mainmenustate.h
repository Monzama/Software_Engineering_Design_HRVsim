#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <QString>
#include <QDebug>
#include "state.h"

class MainMenuState: public State
{
public:
    MainMenuState(QString items, QString title);
    void printSelection(QString selectedItem);
};

#endif // MAINMENUSTATE_H
