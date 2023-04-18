#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    breathSetting = 3333;//the time setting is actually this setting times 3
    ui->breathBar->setMaximum(breathSetting/100);

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
    ui->customPlot->clearPlottables();
    ui->customPlot->removeGraph(0);
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    //ui->customPlot->addGraph(); // red line
    //ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(0, 100);

    connect(ui->contactBox, SIGNAL(released()),this, SLOT(toggleSensor()));

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(onGraphRefresh()));
    ui->customPlot->setVisible(false);

}
void MainWindow::toggleSensor(){
    (*states)[2]->setSensor(ui->contactBox->isChecked());
    //pause and unpause if in a session
    if (currentState == (*states)[2]){
        pauseSession(ui->contactBox->isChecked());//toggles session
    }
}

void MainWindow::pauseSession(bool enable){
    //pause it
    if (!enable){
        breathingTimer.stop();
        breathTimer.stop();
        dataTimer.stop();
        qInfo("Session paused, please reconnect sensor to resume");
    }else{//unpause it
        breathingTimer.start();
        dataTimer.start();
        if (breathState==2){
            breathingTimer.start(breathSetting/3);
        }
    }
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
    breathState = 0;
    //set up breath timer for states
    connect(&breathTimer, SIGNAL(timeout()), this, SLOT(updateBreathState()));
    //updateBreathState();
    //set up breath bar timer
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
    ui->sessionTime->setVisible(false);

    history = new QVector<State*>();

}
//updates the breath progress bar, add smoothing here
void MainWindow::updateBreathBar(){
    //inhale
    if(breathState == 1){
        ui->breathBar->setValue(ui->breathBar->value()+1); // make adjustable
        //we've reached full inhale, switch to hold
        if (ui->breathBar->value()>=ui->breathBar->maximum()){
            updateBreathState();
            //turn on timer for hold duration
            breathTimer.start(breathSetting);
        }
    //exhale
    } else if(breathState == 3) {
        ui->breathBar->setValue(ui->breathBar->value()-1); // make adjustable
        //we've reached full exhale, switch to inhale
        if (ui->breathBar->value()<=0){
            //add a brief delay before inhaling again
            updateBreathState();
        }
    }

}
//cycles the breath state based on current breath state
void MainWindow::updateBreathState(bool reset){
    if (reset){
        breathState = 0;
        ui->breathBar->setValue(0);
        ui->breathBar->setFormat("");
        return;
    }
    breathTimer.stop();
    //inhale to hold
    if(breathState == 1){
        //set to max to account for errors
        //ui->breathBar->setValue(ui->breathBar->maximum());
        ui->breathBar->setFormat("hold");
        breathState = 2;

    //hold to exhale
    } else if(breathState == 2){
        ui->breathBar->setFormat("exhale");
        breathState = 3;
    //exhale to inhale
    } else if(breathState == 3||breathState == 0){
        //reset to zero, to account for errors
        //ui->breathBar->setValue(0);
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
//updates graph data and graphics, shift data generation into sensor
void MainWindow::onGraphRefresh(){
    static QTime time(QTime::currentTime());
    //get our data
    int HR = currentState->updateHRGraph(breathSetting);
    int t = currentSession->getTime();//get time from session

    //update time label...may be moved to another function
    char msg[18];
    sprintf(msg, "Time Elapsed: %ds" ,t);
    ui->sessionTime->setText(msg);

    //add data to the graph
    ui->customPlot->graph(0)->addData(t, HR);
    ui->customPlot->xAxis->setRange(t, 64, Qt::AlignRight);
    ui->customPlot->replot();
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
    if (activeQListWidget->isHidden()){return;}

    int nextIndex = activeQListWidget->currentRow() - 1;
    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }
    activeQListWidget->setCurrentRow(nextIndex);
}

void MainWindow::pressDownButton() { // copied from denas
    if (activeQListWidget->isHidden()){return;}

    int nextIndex = activeQListWidget->currentRow() + 1;
    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }
    activeQListWidget->setCurrentRow(nextIndex);

}

//process menu selection
void MainWindow::menuNavigation(int index){
    //what is this>?initialize some history to browse
    if(history->size() == 3){ history->removeFirst(); }
    history->append(currentState);

    switch (index) {
    case 1:
        if (!ui->contactBox->isChecked()){
            qInfo("Please attatch sensor to start session");
            return;
        }
        currentState = (*states)[2];//change state to session
        currentSession = *currentState->createNewSession();
        isolateGraphStart();//start the graph
        updateBreathState();//start breath timer
        ui->sessionTime->setVisible(true);
        activeQListWidget->setVisible(false);//disable menu
        break;
    case 2:
        currentState = (*states)[4];//change state to settings
        updateMenu(currentState->menuTitle, currentState->menuOptions);//update menu option
        break;

    case 3:
        currentState = (*states)[5]; //change state to history
        updateMenu(currentState->menuTitle, currentState->menuOptions);//update menu options
        break;

    case 4://reset selection
        currentState = (*states)[5]; //state remains
        updateMenu(currentState->menuTitle, currentState->menuOptions);//update menu options
        break;

    default:
        break;
    }

    //implement this later


    //else if(currentState->menuOptions[index] == "Session 1"){
       // currentState = (*states)[6];
       // updateMenu(currentState->menuTitle, currentState->menuOptions);
   // } // SAMPLE
  //  else if(currentState->menuOptions[index] == "View"){currentState->printSelection(currentState->menuOptions[index]); }

  //  else{ currentState->printSelection(currentState->menuOptions[index]); }
}



void MainWindow::pressSelectionButton() {
    int index = activeQListWidget->currentRow();
    qInfo()<<"index:"<<index;
    switch(currentState->handlePressSelectorButton()) {
      case -1:
        // code block
        qInfo("do nothing?off state");
        break;

      case 0:
        qInfo("main menu state");//debug
        //process menu change
        menuNavigation(index+1);
        break;
      case 1:
        // code block
        qInfo("in a session");
        break;

      case 2:
        qInfo("Settings state");//debug
        settingSelection(index);//process settings submenu selection
        break;

      case 3:
        // code block
        qInfo("history state");
        //probably add a function to process this for simplicity
        break;

      case 4:
        // code block
        qInfo("summary state");
        //probably add a function to process this for simplicity
        break;

      case 5:
        // code block
        qInfo("Action state");//probably delete option in summary state
        break;

      case 6:
        qInfo("Breath selection");//debug
        breathSetting = ((index+1)*1000)/3;//change breath setting based on selection
        ui->breathBar->setMaximum(breathSetting/100);//adjust bar duration
        currentState->setBreath(false);
        pressMenuButton();//return to main menu
        //reset settings variable
        break;
      case 7:
        qInfo("Reset confirmation");//debug
        if (index == 0){//process reset
            processReset();
            pressMenuButton();
        }else{//process cancellation
            currentState->setReset(false);//reset settings variable
            menuNavigation(2);//return to settings menu
        }
        break;
      default:
        // code block
        qInfo("idk man");
    }
}
//process bool toggle for settings state sub menus and display new menu
void MainWindow::settingSelection(int index){
    //user has selected reset option
    if (index == 0){
        currentState->setReset(true);
        currentState->setBreath(false);
        //display new menu here
        updateMenu("Reset Confirmation", {"Confirm", "Cancel"});

    //user has selected breath settings option
    }else if(index ==1){
        currentState->setReset(false);
        currentState->setBreath(true);
        //display new  menu here
        //breath list is just hard coded in.
        updateMenu("Breath Setting", {"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30"});
    }
}

//process system reset
void MainWindow::processReset(){
    //reset variables to defaults
    currentState->setBreath(false);
    currentState->setReset(false);
    breathSetting = 3333;
    ui->breathBar->setMaximum(breathSetting/100);
    breathTimer.stop();
    currentState =(*states)[1];
    setUpGraph();
    ////TODO:delete the saved sessions
}

void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) { // copied from denas

    activeQListWidget->clear();
    activeQListWidget->addItems(menuItems);
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(selectedMenuItem);

}

void MainWindow::pressBackButton() {
    if (history->size()==0){
        return;
    }
    currentState = history->last();
    history->removeLast();
    updateMenu(currentState->menuTitle, currentState->menuOptions);

}

void MainWindow::pressMenuButton() {

    if(currentState == (*states)[2]){
        isolateGraphEnd();
        activeQListWidget->setVisible(true);
        ui->sessionTime->setVisible(false);
        ui->sessionTime->setText("Time Elapsed: 0s");
    }
    currentState = (*states)[1];
    updateMenu(currentState->menuTitle, currentState->menuOptions);
    updateBreathState(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}

