#ifndef STATE_H
#define STATE_H

#include <QString>
#include <QListWidget>

class State
{

public:

    State(QString items, QString title);

    virtual void printSelection(QString selectedItem) = 0;

    QStringList menuOptions;
    QString menuTitle;

};

#endif // STATE_H
