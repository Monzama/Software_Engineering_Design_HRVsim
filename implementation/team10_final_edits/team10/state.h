#ifndef STATE_H
#define STATE_H

#include <QString>
#include <QListWidget>
#include "session.h"

class State : public QObject{

public:

    State(QString items, QString title);
    QStringList menuOptions;
    QString menuTitle;

    virtual int handlePressPowerButton();
    virtual int handlePressMenuButton();
    virtual int handlePressBackButton() = 0
    virtual int handlePressSelectorButton() = 0;

    virtual Session** createNewSession();
    virtual void setSensor(bool enable);
    virtual void setBreath(bool enable);
    virtual void setDelete(bool enable);
    virtual void setReset(bool enable);
    virtual void endSession();

    virtual int getCurrentIndex();
    virtual int setCurrentIndex(int i);
    virtual int updateHRGraph(int breathRate);

};

#endif // STATE_H
