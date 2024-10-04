#include <iostream>
#ifndef PCB


class PCB {
    private:
        int PID; //represents the process ID 
        int state; //represents the state the process is in, 0 is ready, 1 is done
        int burstTime = rand() % 20 + 1; //represents how long a process needs to complete task, random between 1-20;
       
    
    public:
        //getters and setters
        int getPID() { return PID; };
        void setPID(int _pid) { PID = _pid; };

        int getState() { return state; };
        void setState(int _state) { state = _state; };

        int getBurstTime() { return burstTime; };
        void setBurstTime(int _burst) { burstTime = _burst; };
        void subBurstTime(int _burst) { burstTime -= _burst; };
        void zeroBurst() { burstTime = 0; };
        //constructors
        PCB() { PID = 0; state = 0; }
        PCB(int _pid, int _state){ PID = _pid; state = _state; }

};



#endif