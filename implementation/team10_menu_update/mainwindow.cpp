#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setUpGraph();
    setUpBattery();
    setUpBreath();
    setUpStatesMenus();

    //connect(ui->powerButton, SIGNAL(released()), this, SLOT());
    //connect(ui->leftButton, SIGNAL(released()), this, SLOT());
    //connect(ui->rightButton, SIGNAL(released()), this, SLOT());

    connect(ui->upButton, SIGNAL(released()), this, SLOT(pressUpButton()));
    connect(ui->downButton, SIGNAL(released()), this, SLOT(pressDownButton()));
    connect(ui->okButton, SIGNAL(released()), this, SLOT(pressSelectionButton()));
    connect(ui->menuButton, SIGNAL(released()), this, SLOT(pressMenuButton()));
    connect(ui->backButton, SIGNAL(released()), this, SLOT(pressBackButton()));

    connect(ui->ledButton, SIGNAL(released()), this, SLOT(lightLEDs()));

}

void MainWindow::setUpGraph(){

    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-1.2, 1.2);

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(onGraphRefresh()));
    ui->customPlot->setVisible(false);

}

void MainWindow::setUpBattery(){

    ui->batteryBar->setValue(100);
    connect(&batteryTimer, SIGNAL(timeout()), this, SLOT(drainBattery()));
    batteryTimer.start(1000);
    connect(ui->chargeButton, SIGNAL(released()), this, SLOT(chargeBattery()));

}

void MainWindow::setUpBreath(){

    ui->breathBar->reset();
    ui->breathBar->setFormat("default");
    breathState = 3;
    connect(&breathTimer, SIGNAL(timeout()), this, SLOT(updateBreathState()));
    breathTimer.start(10000);
    connect(&breathingTimer, SIGNAL(timeout()), this, SLOT(updateBreathBar()));
    breathingTimer.start(100); // make adjustable + creates a delay at the beginning
}

void MainWindow::setUpStatesMenus(){

    states = new QVector<State*>();
    states->append(new OffState("", "Off"));
    states->append(new MainMenuState("Start New Session, Settings, Log/History", "Main Menu"));
    states->append(new SessionInProgressState("", "Session X In Progress"));
    states->append(new SessionSummaryState("", "Session X Summary"));
    states->append(new SessionSettingsState("Reset Device, Set Breath Timer", "Settings"));
    states->append(new SessionHistoryListState("Session 1, Session 2, Session 3", "Session History")); // LOAD ACTUAL SESSION FILES
    states->append(new SessionActionsState("View, Delete", "Session X Actions"));

    currentState = (*states)[1]; // initial state

    activeQListWidget = ui->mainMenuListView;
    activeQListWidget->addItems(currentState->menuOptions);
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(currentState->menuTitle);

    history = new QVector<State*>();

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
    dataTimer.start(1000);
    ui->customPlot->setVisible(true);
    if( ui->customPlot->isVisible() ) { qInfo("yes we can see it");}
}

void MainWindow::isolateGraphEnd(){
    dataTimer.stop();
    ui->customPlot->setVisible(false);
}

void MainWindow::onGraphRefresh(){

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

void MainWindow::lightLEDs(){

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

void MainWindow::pressUpButton() { // copied from denas

    int nextIndex = activeQListWidget->currentRow() - 1;
    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }
    activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::pressDownButton() { // copied from denas

    int nextIndex = activeQListWidget->currentRow() + 1;
    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }
    activeQListWidget->setCurrentRow(nextIndex);

}

void MainWindow::pressSelectionButton() {

    int index = activeQListWidget->currentRow();
    if (index < 0) return;

    if(history->size() == 3){ history->removeFirst(); }
    history->append(currentState);

    if(currentState->menuOptions[index] == "Start New Session"){ currentState = (*states)[2]; currentState->printSelection(currentState->menuOptions[index]); isolateGraphStart(); activeQListWidget->setVisible(false); }
    else if(currentState->menuOptions[index] == "Settings"){ currentState = (*states)[4]; updateMenu(currentState->menuTitle, currentState->menuOptions); }
    else if(currentState->menuOptions[index] == "Log/History"){ currentState = (*states)[5]; updateMenu(currentState->menuTitle, currentState->menuOptions); }
    else if(currentState->menuOptions[index] == "Session 1"){ currentState = (*states)[6]; updateMenu(currentState->menuTitle, currentState->menuOptions); } // SAMPLE
    else if(currentState->menuOptions[index] == "View"){ currentState->printSelection(currentState->menuOptions[index]); }
    else{ currentState->printSelection(currentState->menuOptions[index]); }

}

void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) { // copied from denas

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(selectedMenuItem);

}

void MainWindow::pressBackButton() {

    currentState = history->last();
    history->removeLast();
    updateMenu(currentState->menuTitle, currentState->menuOptions);

}

void MainWindow::pressMenuButton() {

    if(currentState == (*states)[2]){ isolateGraphEnd(); activeQListWidget->setVisible(true); }
    currentState = (*states)[1]; updateMenu(currentState->menuTitle, currentState->menuOptions);

}


MainWindow::~MainWindow()
{
    delete ui;
}

