#include "state.h"

State::State(QString items, QString title) {
    if (items.operator [](0) ==""){
        this->menuOptions.push_back(NULL);
    }else{
        this->menuOptions = items.split(", ");
    }
    this->menuTitle = title;
}

int State::handlePressPowerButton(){return 1;}
int State::handlePressMenuButton(){return 1;}

Session** State::createNewSession(){return NULL;}
void State::setSensor(bool enable){return;}
void State::setBreath(bool enable){return;}
void State::setDelete(bool enable){return;}
void State::setReset(bool enable){return;}
void State::endSession(){return;}

int State::getCurrentIndex(){return -1;}
int State::setCurrentIndex(int i){return -1;}
int State::State::updateHRGraph(int breathRate){return -1;}
