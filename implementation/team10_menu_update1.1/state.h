#ifndef STATE_H
#define STATE_H

#include <QString>
#include <QListWidget>
#include "session.h"

class State : public QObject
{
public:

    State(QString items, QString title);

    virtual void printSelection(QString selectedItem) = 0;

    QStringList menuOptions;
    QString menuTitle;


    //state button press returns
    virtual int handlePressPowerButton() =0;
    virtual int handlePressMenuButton() = 0;
    virtual int handlePressBackButton() = 0;
    virtual int handlePressLeftButton() = 0;
    virtual int handlePressRightButton() = 0;
    virtual int handlePressSelectorButton() = 0;

    virtual Session** createNewSession(){}
    virtual void setSensor(bool enable){}
    virtual void setBreath(bool enable){}
    virtual void setReset(bool enable){}

    virtual int  updateHRGraph(int breathRate){}

};

#endif // STATE_H
