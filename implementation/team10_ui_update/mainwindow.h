#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QGraphicsDropShadowEffect>

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
    int breathState; // 1 = inhale, 2 = hold, 3 = exhalle

private slots:
    void realtimeDataSlot();
    void testingSlot();
    void isolateGraphStart();
    void isolateGraphEnd();
    void drainBattery();
    void chargeBattery();
    void updateBreathState();
    void updateBreathBar();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
