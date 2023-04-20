#include "state.h"

State::State(QString items, QString title)
{
    if (items.operator [](0) ==""){
        this->menuOptions.push_back(NULL);
    }else{
    this->menuOptions = items.split(", ");
    }
    this->menuTitle = title;

}

//state button press returns
int State::handlePressPowerButton(){
    return -1;
}
int State::handlePressMenuButton(){
    return -1;
}
int State::handlePressBackButton(){
    return -1;
}
int State::handlePressLeftButton(){
    return -1;
}
int State::handlePressRightButton(){
    return -1;
}
int State::handlePressSelectorButton(){
    return -1;
}
