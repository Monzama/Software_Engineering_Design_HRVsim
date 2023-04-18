#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsDropShadowEffect>
#include <QWidget>
#include <QListWidget>
#include <iostream>
#include"qcustomplot.h"
#include "state.h"
#include "offstate.h"
#include "mainmenustate.h"
#include "sessionactionsstate.h"
#include "sessionhistoryliststate.h"
#include "sessioninprogressstate.h"
#include "sessionsettingsstate.h"
#include "sessionsummarystate.h"

//
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QTimer dataTimer;
    QTimer batteryTimer;
    QTimer breathTimer;
    QTimer breathingTimer;

    int breathSetting;

    int breathState; // 1 = inhale, 2 = hold, 3 = exhalle
    State* currentState;
    Session* currentSession;
    QVector<State*> *states;
    QVector<State*> *history;
    QListWidget *activeQListWidget;
private slots:
    void onGraphRefresh();
    void lightLEDs();
    void isolateGraphStart();
    void updateBreathState(bool reset = false);
    void isolateGraphEnd();
    void drainBattery();
    void chargeBattery();

    void updateBreathBar();

    void pressUpButton();
    void pressDownButton();
    void pressSelectionButton();
    void pressBackButton();
    void pressMenuButton();
    void toggleSensor();
private:
    Ui::MainWindow *ui;

    void menuNavigation(int index);
    void settingSelection(int index);
    void processReset();
    void setUpGraph();
    void setUpBattery();
    void setUpBreath();
    void setUpStatesMenus();
    void pauseSession(bool enable);
    void updateMenu(const QString selectedMenuItem, const QStringList menuItems);
};
#endif // MAINWINDOW_H
