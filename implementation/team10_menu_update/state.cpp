#include "state.h"

State::State(QString items, QString title)
{

    this->menuOptions = items.split(", ");
    this->menuTitle = title;

}
