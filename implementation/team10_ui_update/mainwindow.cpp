#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));
 
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-1.2, 1.2);
 
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    //dataTimer.start(0);

    //------

    connect(ui->powerButton, SIGNAL(released()), this, SLOT(testingSlot()));
    connect(ui->upButton, SIGNAL(released()), this, SLOT(isolateGraphStart()));
    connect(ui->downButton, SIGNAL(released()), this, SLOT(isolateGraphEnd()));

    ui->batteryBar->setValue(100);
    connect(&batteryTimer, SIGNAL(timeout()), this, SLOT(drainBattery()));
    batteryTimer.start(1000);
    connect(ui->backButton, SIGNAL(released()), this, SLOT(chargeBattery()));

    ui->breathBar->reset();
    ui->breathBar->setFormat("default");
    breathState = 3;
    connect(&breathTimer, SIGNAL(timeout()), this, SLOT(updateBreathState()));
    breathTimer.start(10000);
    connect(&breathingTimer, SIGNAL(timeout()), this, SLOT(updateBreathBar()));
    breathingTimer.start(100); // make adjustable + creates a delay at the beginning

}

void MainWindow::updateBreathBar(){

    if(breathState == 1){
        int lastVal = ui->breathBar->value();
        ui->breathBar->setValue(lastVal+1); // make adjustable
    } else if(breathState == 3) {
        int lastVal = ui->breathBar->value();
        ui->breathBar->setValue(lastVal-1); // make adjustable
    }

}

void MainWindow::updateBreathState(){

    if(breathState == 1){
        ui->breathBar->setFormat("hold");
        ++breathState;
    } else if(breathState == 2){
        ui->breathBar->setFormat("exhale");
        ++breathState;
    } else if(breathState == 3){
        ui->breathBar->setFormat("inhale");
        breathState = 1;
    }

}

void MainWindow::drainBattery(){
    int lastVal = ui->batteryBar->value();
    ui->batteryBar->setValue(lastVal-1);
}

void MainWindow::chargeBattery(){
    ui->batteryBar->setValue(100);
}

void MainWindow::isolateGraphStart(){
    dataTimer.start(0);
    ui->customPlot->setVisible(true);
    if( ui->customPlot->isVisible() ) { qInfo("yes we can see it");}
}

void MainWindow::isolateGraphEnd(){
    dataTimer.stop();
    ui->customPlot->setVisible(false);
}

void MainWindow::realtimeDataSlot(){

    static QTime time(QTime::currentTime());

    double key = time.elapsed()/1000.0;
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002)
    {
          ui->customPlot->graph(0)->addData(key, qSin(8*key));
          ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));

          lastPointKey = key;
    }

    ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
    ui->customPlot->replot();

    static double lastFpsKey;
    static int frameCount;
    ++frameCount;

    if (key-lastFpsKey > 2)
    {
      lastFpsKey = key;
      frameCount = 0;
    }

}

void MainWindow::testingSlot(){

    if( ui->blueButton->isChecked() ){

        auto noGlow = new QGraphicsDropShadowEffect(); // mem leak potential
        noGlow->setOffset(0);
        noGlow->setBlurRadius(0);

        ui->blueButton->setChecked(false);
        ui->greenButton->setChecked(false);
        ui->redButton->setChecked(false);

        ui->blueButton->setGraphicsEffect(noGlow);
        ui->greenButton->setGraphicsEffect(noGlow);
        ui->redButton->setGraphicsEffect(noGlow);

    }else{

        ui->blueButton->setChecked(true);
        ui->greenButton->setChecked(true);
        ui->redButton->setChecked(true);

        auto effectB = new QGraphicsDropShadowEffect();
        effectB->setOffset(.0);
        effectB->setBlurRadius(20.0);
        effectB->setColor(Qt::blue);
        ui->blueButton->setGraphicsEffect(effectB);

        auto effectG = new QGraphicsDropShadowEffect();
        effectG->setOffset(.0);
        effectG->setBlurRadius(20.0);
        effectG->setColor(Qt::green);
        ui->greenButton->setGraphicsEffect(effectG);

        auto effectR = new QGraphicsDropShadowEffect();
        effectR->setOffset(.0);
        effectR->setBlurRadius(20.0);
        effectR->setColor(Qt::red);
        ui->redButton->setGraphicsEffect(effectR);

    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

