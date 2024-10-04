#include <iostream>
#include <vector>
#include <queue>
#include "job.h"

class queues {

    public:
        std::queue<Job> RR; //round robin queue, with time quantums of 10ms
        std::queue<Job> RRfiveten; //round robin queue, with time quantums of 15ms
        std::queue<Job> FCFS;// first come first served queue 
        std::queue<Job> Ready; //holds every thread as soon as it is created to be added to the 
    
    void addEvent(Job eve, int listNum){
        //checking which queue the event needs to be added to based on the integer passed in
        if (listNum == 1) { RR.push(eve); return; };
        if (listNum == 2) { RRfiveten.push(eve); return; };
        if (listNum == 3) { FCFS.push(eve); return; };
        //if the list number passed in was invalid, statement printed to notify user
        std::cout << "Event was not added properly!!\n"; 
    }
};
