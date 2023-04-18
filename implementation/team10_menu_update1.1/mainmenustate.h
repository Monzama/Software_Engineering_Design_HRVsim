#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H
#include <QMainWindow>
#include <QString>
#include <QDebug>
#include "state.h"
class MainMenuState: public State
{
public:
    MainMenuState(QString items, QString title);
    void printSelection(QString selectedItem);

    //state button press returns
    int handlePressPowerButton();
    int handlePressMenuButton();
    int handlePressBackButton();
    int handlePressLeftButton();
    int handlePressRightButton();
    int handlePressSelectorButton();

private:
};

#endif // MAINMENUSTATE_H
