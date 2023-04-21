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
