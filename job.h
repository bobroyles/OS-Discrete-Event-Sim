
#include "pcb.h"
#ifndef event


class Job {
    
    public:
        int PID; //represents the process ID 
        int burstTime; //represents how long a process needs to complete task, random between 1-20;
        int arrivalNums; //checking to see if this is the first arrival event for the job
        int arrivalTime; //holds the time that the job arrived 
        int recentArrival; //holds the most recent arrival time, used to calculate the total wait time
        int finishTime; //the time that the job is finished 
        
        int holdBurst;

        float TurnAroundTime;
        float WaitTime;
        float ResponseTime;
    
        //constructors

        //default constructor
        Job(){
            PID = 0;
            burstTime = 0;
            arrivalNums = 0;
            TurnAroundTime = 0;
            WaitTime = 0;
            ResponseTime = 0;

        }
        //constructor with only id
        Job(int id){
            PID = id;
            burstTime = rand() % 30 + 5; 
            arrivalNums = 0;
            TurnAroundTime = 0;
            WaitTime = 0;
            ResponseTime = 0;
            recentArrival = 0;
            holdBurst = burstTime;
        }
        //argumented constructor
        // Job(int id, float turnTime, float wait, float response){
        //     PID = id;
        //     TurnAroundTime = turnTime;
        //     WaitTime = wait;
        //     ResponseTime = response;
        // }

        //getters, return value
        float getTurnAround() { return TurnAroundTime; };
        float getWaitTime() { return WaitTime; };
        float getResponseTime() { return ResponseTime; };
        int getBurstTime() { return burstTime; };
        int getPID() { return PID; };
        int getArrivalNum() { return arrivalNums; };
        int getArrivalTime() { return arrivalTime; };
        int getFinishTime() { return finishTime; };
        int getRecArrival() { return recentArrival; };
        //setters, set value
        void setTurnAroundTime(float turnAround) { TurnAroundTime = turnAround; };
        void setWaitTime(float waitTime) { WaitTime = waitTime; };
        void setResponseTime(float response) { ResponseTime = response; };
        void setPID(int _pid) { PID = _pid; };
        void setArrivalNum(int anum) { arrivalNums = anum; };
        void setArrivalTime(int time) { arrivalTime = time; };
        void setFinishTime(int fin) { finishTime = fin; };
        void setrecentArrival(int recArrival) { recentArrival = recArrival; };
        //special addition and subtraction functions
        void incArrival() { arrivalNums += 1; };
        void subBurstTime(int dec) { burstTime = burstTime - dec; };
        void zeroBurst() { burstTime = 0; };
        void addWaitTime(float wait) { WaitTime += wait; };

        

};

#endif