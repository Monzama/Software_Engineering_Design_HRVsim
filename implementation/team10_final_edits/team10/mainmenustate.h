#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include "state.h"

class MainMenuState: public State{
    
public:
    
    MainMenuState(QString items, QString title);

    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressSelectorButton();

};

#endif // MAINMENUSTATE_H
