#include "session.h"

Session::Session()
{    
    elapsedTimeInSeconds=0;
}
bool Session::update(int HR, int coherence){
    //add hr data to list

    //add coherence to list if there is one included in call

    //increment time
    elapsedTimeInSeconds++;
    //re-calculate averages
}


int Session::getTime(){
    return  elapsedTimeInSeconds;
}
