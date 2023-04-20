#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
//constructor, handles various initialization, may need to move some
{
    ui->setupUi(this);
    breathSetting = 3333;//the time setting is actually this setting times 3
    ui->breathBar->setMaximum(breathSetting/100);
    isHistorySummary = false;

    setUpGraph();
    setUpBattery();
    setUpBreath();
    setUpStatesMenus();

    connect(ui->leftButton, SIGNAL(released()), this, SLOT(noAction()));
    connect(ui->rightButton, SIGNAL(released()), this, SLOT(noAction()));
    connect(ui->upButton, SIGNAL(released()), this, SLOT(pressUpButton()));
    connect(ui->downButton, SIGNAL(released()), this, SLOT(pressDownButton()));
    connect(ui->okButton, SIGNAL(released()), this, SLOT(pressSelectionButton()));
    connect(ui->menuButton, SIGNAL(released()), this, SLOT(pressMenuButton()));
    connect(ui->backButton, SIGNAL(released()), this, SLOT(pressBackButton()));
    connect(ui->powerButton, SIGNAL(released()), this, SLOT(pressPowerButton()));

    setOffVisible(true);
}

//deconstructor, handles memory freeing
MainWindow::~MainWindow()
{
    delete ui;
}

//initial graph setup for starting a sesssion
void MainWindow::setUpGraph(){

    ui->customPlot->addGraph();
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(30, 100);

    connect(ui->contactBox, SIGNAL(toggled(bool)),this, SLOT(toggleSensor(bool)));
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(onGraphRefresh()));
    ui->customPlot->setVisible(false);

}

//initial battery setup
void MainWindow::setUpBattery(){

    ui->batteryBar->setValue(100);
    connect(&batteryTimer, SIGNAL(timeout()), this, SLOT(drainBattery()));
    batteryTimer.start(1000);
    connect(ui->chargeButton, SIGNAL(released()), this, SLOT(chargeBattery()));

}

//initial breath pacer setup
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

//state instantiation and setup
void MainWindow::setUpStatesMenus(){

    states = new QVector<State*>();
    states->append(new OffState("", "Off"));
    states->append(new MainMenuState("Start New Session, Settings, Log/History", "Main Menu"));
    states->append(new SessionInProgressState("", "Session X In Progress"));
    states->append(new SessionSummaryState("", "Session X Summary"));
    states->append(new SessionSettingsState("Reset Device, Set Breath Timer", "Settings"));
    states->append(new SessionHistoryListState("", "Session History"));

    currentState = (*states)[0]; // initial state

    activeQListWidget = ui->mainMenuListView;

}


////Updaters
///
//updates the breath progress bar based on breath state
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

//cycles the breath state
void MainWindow::updateBreathState(bool reset){
    if (reset){
        breathState = 0;
        ui->breathBar->setValue(0);
        ui->breathBar->setFormat("");
        breathingTimer.stop();
        breathTimer.stop();
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

//adjusts battery progress bar for battery drain
void MainWindow::drainBattery(){
    int lastVal = ui->batteryBar->value();
    ui->batteryBar->setValue(lastVal-1);
    if (lastVal <=6 && lastVal >2){
        qInfo("BATTERY CRITICAL, PLEASE CHARGE NOW");
    }else if (lastVal ==2){
        qInfo("BATTERY POWER FAILING, EMERGENCY SAVE IN PROGRESS");

    }else if (lastVal ==1){
        pressPowerButton();
    }
}

//charges the battery, default is to 100
void MainWindow::chargeBattery(){
    ui->batteryBar->setValue(100);
}


////UI event handlers
///
//handles all upbutton presses
void MainWindow::pressUpButton() { // copied from denas
    if (activeQListWidget->isHidden()){return;}

    int nextIndex = activeQListWidget->currentRow() - 1;
    if (nextIndex < 0) {
        nextIndex = activeQListWidget->count() - 1;
    }
    activeQListWidget->setCurrentRow(nextIndex);
}

//handles all downbutton presses
void MainWindow::pressDownButton() { // copied from denas
    if (activeQListWidget->isHidden()){return;}

    int nextIndex = activeQListWidget->currentRow() + 1;
    if (nextIndex > activeQListWidget->count() - 1) {
        nextIndex = 0;
    }
    activeQListWidget->setCurrentRow(nextIndex);

}

void MainWindow::noAction() { qInfo() << "nothing to select in this direction"; }

//handles all power button presses
void MainWindow::pressPowerButton(){
    //most states will do the same thing,
    //transition to off screen, does not save session

    int i = currentState->handlePressPowerButton();
    //if off, go to main menu
    if (i==0){
        if (ui->batteryBar->value()<=0){
            qInfo("Please charge to turn on");
        }else{
            setOffVisible(false);
            currentState = (*states)[1];
            setMenuVisible(true);
        }
    }else if (i==1){
        setOffVisible(true);
        currentState = (*states)[0];
    }else if (i == 2){
        //save the session amd power off device, same as battery outage
        pressSelectionButton();
        setOffVisible(true);
        currentState = (*states)[0];
    }
}

//handles all back button presses
void MainWindow::pressBackButton() {
    int i = currentState->handlePressBackButton();
    if (i ==0){
        //to main menu
        pressMenuButton();
    }else if (i ==1){
        //settings menu
        menuNavigation(2);
        currentState->setReset(false);
        currentState->setBreath(false);
    }else if (i==2){
        //return to history list
        if (isHistorySummary){
            menuNavigation(3);
            activeQListWidget->setVisible(true);
        }else{
            pressMenuButton();
        }
    }else if (i==3){
        currentState->setDelete(false);
        currentState =(*states)[3];
        setSummaryVisible(true);
        displaySummary(currentSession);
    }

}

//handles all menu button presses
void MainWindow::pressMenuButton() {
    switch (currentState->handlePressMenuButton()) {
    case -1:
        //do nothing
        break;
    case 1:
        //process menu
        currentState = (*states)[1];
        setMenuVisible(true);
        break;

    default:
        break;
    }


}

//handles all selectbutton presses
void MainWindow::pressSelectionButton() {
    int index = activeQListWidget->currentRow();
    switch(currentState->handlePressSelectorButton()) {
      case -1:
        // code block
        //qInfo("do nothing?off state");
        break;

      case 0:
        //qInfo("main menu state");//debug
        //process menu change
        menuNavigation(index+1);
        break;
      case 1:
        // code block
        //qInfo("in a session");
        //process save session if at least 5 seconds
        if (currentSession->getTime()>=5){
            savedSessions.push_back(currentSession);
        }
        currentState->endSession();
        //move to summary
        currentState =(*states)[3];
        setSummaryVisible(true);
        isHistorySummary = false;
        displaySummary(currentSession);
        qInfo() << "Session ended, to delete this session press OK";
        break;

      case 2:
        //qInfo("Settings state");//debug
        settingSelection(index);//process settings submenu selection
        break;

      case 3:
        //qInfo("history list state");
        currentState =(*states)[3];
        setSummaryVisible(true);
        isHistorySummary = true;
        currentSession = savedSessions.operator [](index);
        displaySummary(savedSessions.operator [](index));
        //probably add a function to process this for simplicity
        qInfo() << "Viewing Session" << index << "to delete this session press OK";
        break;

      case 4:
        //qInfo("summary state");
        updateMenu("Delete Session?", {"Yes","No"});
        activeQListWidget->setVisible(true);
        currentState->setDelete(true);
        //offer session delete
        break;

      case 5:
        //qInfo("Delete session confirmation");//probably delete option in summary state
        if (index ==1){
            activeQListWidget->setVisible(false);
            currentState->setDelete(false);
        }else{
            //actual delete action
            if (currentState->getCurrentIndex()!=-1){
                savedSessions.remove(currentState->getCurrentIndex());
            }
            currentState->setDelete(false);
            if (isHistorySummary){
                menuNavigation(3);
                setSummaryVisible(false);

            }else{
                pressMenuButton();
            }

            //return to history list
        }
        break;

      case 6:
        //qInfo("Breath selection");//debug
        breathSetting = ((index+1)*1000)/3;//change breath setting based on selection
        ui->breathBar->setMaximum(breathSetting/100);//adjust bar duration
        currentState->setBreath(false);
        pressMenuButton();//return to main menu
        //reset settings variable
        break;
      case 7:
        //qInfo("Reset confirmation");//debug
        if (index == 0){//process reset
            processReset();
            menuNavigation(2);
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

//handles contact sensor presses
void MainWindow::toggleSensor(bool checked){

    //if(checked){ ui->contactBox->setChecked(false); qInfo() << "nope";}
    //else{ ui->contactBox->setChecked(true); qInfo() << "in"; }
    (*states)[2]->setSensor(ui->contactBox->isChecked());
    //pause and unpause if in a session
    if (currentState == (*states)[2]){
        pauseSession(ui->contactBox->isChecked());//toggles session
    }
}

//toggles led bloom affect from button toggle
void MainWindow::lightLEDs(int level){
    auto noGlow = new QGraphicsDropShadowEffect(); // mem leak potential
    noGlow->setOffset(0);
    noGlow->setBlurRadius(0);

    ui->blueButton->setChecked(false);
    ui->greenButton->setChecked(false);
    ui->redButton->setChecked(false);

    ui->blueButton->setGraphicsEffect(noGlow);
    ui->greenButton->setGraphicsEffect(noGlow);
    ui->redButton->setGraphicsEffect(noGlow);
    auto effectG = new QGraphicsDropShadowEffect();
    auto effectR = new QGraphicsDropShadowEffect();
    auto effectB = new QGraphicsDropShadowEffect();
    switch (level) {
    case 0:

        effectR->setOffset(.0);
        effectR->setBlurRadius(20.0);
        effectR->setColor(Qt::red);
        ui->redButton->setGraphicsEffect(effectR);
        break;
    case 1:

        effectB->setOffset(.0);
        effectB->setBlurRadius(20.0);
        effectB->setColor(Qt::blue);
        ui->blueButton->setGraphicsEffect(effectB);
        break;
    case 2:

        effectG->setOffset(.0);
        effectG->setBlurRadius(20.0);
        effectG->setColor(Qt::green);
        ui->greenButton->setGraphicsEffect(effectG);
        break;
    default:
        break;
    }
}


////Menu navigation helpers
///
//process menu selection/navigation
void MainWindow::menuNavigation(int index){

    switch (index) {
    case 1:
        if (!ui->contactBox->isChecked()){
            qInfo("Please attach sensor to start session");
            return;
        }
        breathingTimer.start();
        currentState = (*states)[2];//change state to session
        currentSession = *currentState->createNewSession();
        char msg4[18];
        sprintf(msg4, "Session %d" ,currentSession->getID());
        ui->menuLabel->setText(msg4);
        setSessionVisible(true);
        qInfo() << "Session started, press OK when you are ready to end";
        break;
    case 2:
        currentState = (*states)[4];//change state to settings
        updateMenu(currentState->menuTitle, currentState->menuOptions);//update menu option
        break;

    case 3:
        currentState = (*states)[5]; //change state to history
        updateMenu(currentState->menuTitle, getHistoryList());//update menu options
        break;

    case 4://reset selection
        currentState = (*states)[5]; //state remains
        updateMenu(currentState->menuTitle, currentState->menuOptions);//update menu options
        break;

    default:
        break;
    }
}


QStringList MainWindow::getHistoryList(){
    QStringList hist;
    if (savedSessions.length()==0){
        hist.push_back("");
        return hist;
    }
    QString base = "Session ";
    QString actual = "Session ";
    for (int var = 0; var < savedSessions.length(); var++) {

        //append to list
        actual.append(48+savedSessions.operator [](var)->getID());
        actual.append("                           " + savedSessions.operator [](var)->getDate());
        actual.append("                                " + QString::number(savedSessions.operator [](var)->getTime()) + "s");
        hist.push_back(actual);
        actual = base;
    }
    return hist;
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
    ui->customPlot->graph(0)->data()->clear();
    ui->customPlot->replot();
    savedSessions.clear();
}

//when in a menu state, update the menu list options (menuItems) and menu name(selectedMenuItem)
void MainWindow::updateMenu(const QString selectedMenuItem, const QStringList menuItems) { // copied from denas
    activeQListWidget->clear();
    if (menuItems.operator [](0)!=NULL){
        activeQListWidget->addItems(menuItems);
    }
    activeQListWidget->setCurrentRow(0);
    ui->menuLabel->setText(selectedMenuItem);
}


////Active Session
///
//pauses session if enable is falses, otherwise unpauses
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
            breathTimer.start();
        }
    }
}

//conforms UI to display the summary of a given session
void MainWindow::displaySummary(Session* s){
    int total = s->getTime()-4;
    if (total <=0){
        total =1;
    }
    float l1 = s->getLowTime();
    float l2 = s->getMedTime();
    float l3 = s->getHighTime();
    char msg0[18];
    sprintf(msg0, "Level 1: %0.0f%%" ,100*(l1/total));
    ui->level1->setText(msg0);

    char msg1[18];
    sprintf(msg1, "Level 2: %0.0f%%" ,100*(l2/total));
    ui->level2->setText(msg1);

    char msg2[18];
    sprintf(msg2, "Level 3: %0.0f%%" ,100*(l3/total));
    ui->level3->setText(msg2);

    fillGraph(s->getHRData());
    //process UI
    int t = s->getTime();
    char msg3[18];
    sprintf(msg3, "Time Elapsed: %ds" ,t);
    ui->sessionTime->setText(msg3);

    char msg4[18];
    sprintf(msg4, "Session %d Summary" ,s->getID());
    ui->menuLabel->setText(msg4);

    //update achievement
    float ach = s->getAchievScore();
    char msg5[18];
    sprintf(msg5, "Ahievement: %0.2f" ,ach);
    ui->achievement->setText(msg5);

    //update coherence
    float avgcoh = ach/ s->getTotalCoherence()->length();
    char msg6[18];
    sprintf(msg6, "Avg Coherence: %0.2f" ,avgcoh);
    ui->coherence->setText(msg6);
    int i = savedSessions.indexOf(s);
    currentState->setCurrentIndex(i);
}

////Custom Plot functions
///
//switches screen to session mode by setting visibility, starts data timer
void MainWindow::isolateGraphStart(){
    dataTimer.start(1000);
    ui->customPlot->setVisible(true);
}

//populates entire graph with data in list
void MainWindow::fillGraph(QVector<float>* data){
    QVector<float>::iterator i;
    ui->customPlot->graph(0)->data()->clear();
    int count = 0;
    for (int i = 0; i < data->length(); ++i) {
        count++;
        //add data to the graph
        ui->customPlot->graph(0)->addData(count, data->operator [](i));
    }
    ui->customPlot->xAxis->setRange(0, count, Qt::AlignLeft);
    ui->customPlot->replot();
    ui->customPlot->setVisible(true);
}

//gets HR data from sensor and updates the graph
void MainWindow::onGraphRefresh(){
    static QTime time(QTime::currentTime());
    float prevCOH = currentSession->getCurrentCoherence();;
    //get our data
    int HR = currentState->updateHRGraph(breathSetting);
    //update coherence
    float coh = currentSession->getCurrentCoherence();
    char msg1[18];
    sprintf(msg1, "Coherence: %0.2f" ,coh);
    ui->coherence->setText(msg1);

    //update achievement
    float ach = currentSession->getAchievScore();
    char msg3[18];
    sprintf(msg3, "Ahievement: %0.2f" ,ach);
    ui->achievement->setText(msg3);

    if (coh<=1.5 &&coh>0){
        currentSession->timeIncrement(0);
        lightLEDs(0);
        if (prevCOH >1.5 ||prevCOH==0){
            qInfo("BEEP...New level reached");
        }

    }else if (coh <3 && coh>1.5){
        currentSession->timeIncrement(1);
        lightLEDs(1);
        if (prevCOH <1.5 || prevCOH >3){
            qInfo("BEEP...New level reached");
        }
    }else if (coh >=3){
        currentSession->timeIncrement(2);
        lightLEDs(2);
        if (prevCOH <3){
            qInfo("BEEP...New level reached");
        }
    }else{
        currentSession->timeIncrement(-1);
    }
    //update elapsed time
    int t = currentSession->getTime();//get time from session

    //update time label...may be moved to another function
    char msg2[18];
    sprintf(msg2, "Time Elapsed: %ds" ,t);
    ui->sessionTime->setText(msg2);


    //add data to the graph
    ui->customPlot->graph(0)->addData(t, HR);
    ui->customPlot->xAxis->setRange(t, 32, Qt::AlignRight);
    ui->customPlot->replot();
}

////other helpers
///
//process ui changes for main menu
void MainWindow::setMenuVisible(bool enable){
    if (enable){
        //process menu
        setSessionVisible(false);
        setSummaryVisible(false);
        activeQListWidget->setVisible(true);
        updateMenu(currentState->menuTitle, currentState->menuOptions);
    }else{
        activeQListWidget->setVisible(false);
    }

}
//process ui changes for summary
void MainWindow::setSummaryVisible(bool enable){
    if (enable){
        setSessionVisible(false);
        setMenuVisible(false);
        ui->level1->setVisible(true);
        ui->level2->setVisible(true);
        ui->level3->setVisible(true);
        ui->sessionTime->setVisible(true);
        ui->coherence->setVisible(true);
        ui->achievement->setVisible(true);
        ui->levelsHeader->setVisible(true);
        ui->customPlot->setVisible(true);
    }else{
        ui->level1->setVisible(false);
        ui->level2->setVisible(false);
        ui->level3->setVisible(false);
        ui->sessionTime->setVisible(false);
        ui->levelsHeader->setVisible(false);
        ui->customPlot->graph(0)->data()->clear();
        ui->customPlot->replot();
        ui->customPlot->setVisible(false);
    }
}
//process ui changes for off state
void MainWindow::setOffVisible(bool enable){
    if (enable){
        setSummaryVisible(false);
        setMenuVisible(false);
        setSessionVisible(false);
        currentSession = NULL;
        activeQListWidget->setVisible(true);
        activeQListWidget->clear();
        activeQListWidget->setEnabled(false);
        ui->redButton->setEnabled(false);
        ui->blueButton->setEnabled(false);
        ui->greenButton->setEnabled(false);
        ui->batteryBar->setVisible(false);
        ui->batteryLabel->setVisible(false);
        ui->menuLabel->setVisible(false);
        //ui->contactBox->setEnabled(false);
        ui->contactBox->setChecked(false);
        dataTimer.stop();
        batteryTimer.stop();
        breathingTimer.stop();
        breathTimer.stop();
    }else{
        activeQListWidget->setEnabled(true);
        ui->redButton->setEnabled(true);
        ui->blueButton->setEnabled(true);
        ui->greenButton->setEnabled(true);
        ui->batteryBar->setVisible(true);
        ui->batteryLabel->setVisible(true);
        ui->menuLabel->setVisible(true);
        //ui->contactBox->setEnabled(true);
        batteryTimer.start();
    }
}
//process ui changes for session
void MainWindow::setSessionVisible(bool enable){
    if (enable){
        setMenuVisible(false);
        setSummaryVisible(false);
        isolateGraphStart();//start the graph
        breathingTimer.start();
        updateBreathState();//start breath timer
        ui->sessionTime->setVisible(true);
        ui->coherence->setVisible(true);
        ui->achievement->setVisible(true);
    }else{
        dataTimer.stop();
        ui->sessionTime->setVisible(false);
        ui->sessionTime->setText("Time Elapsed: 0s");
        ui->coherence->setVisible(false);
        ui->coherence->setText("Coherence: 0.0");
        ui->achievement->setVisible(false);
        ui->achievement->setText("Achievement: 0.0");
        ui->customPlot->graph(0)->data()->clear();
        ui->customPlot->replot();
        ui->customPlot->setVisible(false);
        updateBreathState(true);
        lightLEDs(-1);
    }
}
