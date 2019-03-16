#include <iostream>
#include <unistd.h> // usleep
#include "../../Progress.h"
int main(){
    Progress myProgress(0,120); // init progress with min and max values. In our case its '120' in seconds.
    std::cout << "Hey! Now doing some stuff for next 120 seconds\n";
    for (int secondsSoFar=0; secondsSoFar<=120; secondsSoFar++){
        myProgress.update(secondsSoFar);
        std::cout << "\r" << myProgress.toBar(80) << myProgress.toPaddedPercentage(1) << "% " << secondsSoFar << " seconds so far" << std::flush;
        usleep(1000*1000);
    }
    std::cout << "\nHey Done!\n";
    return 0;
} 
