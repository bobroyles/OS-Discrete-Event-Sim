//Bo Broyles
//Lab 7 Operating Systems
//Discrete Event Simulation 
//
// For my Lab 7 I decided to implement a Load Sharing system in which every queue is added to based on which one is free
// if none of the queues are free than a queue is randomly selected to be added to 


#include <iostream>
#include <fstream>
#include "cpu.h"
#include "queues.h"
#include "event.h"
#include <queue>
#include <vector>



std::ofstream dataFile; //creating a connection to write to a file
queues lists; //creating the queues to represent the different processors
std::vector<Event>FEL; //FEL, holds all of the events and can be sifted through to find the next event to happen


//prints information to the ProcessInfo file, each argument passed in takes up one line on the text file
void dataPrint(Job recentJob);
void scheduleNew();
void assignThread(Job hold);
//checks to see if any of the processors are empty 
int checkEmpty();
//need to write this event handler to do work on the processess for the first Round Robin Processor with time quantum of 10
void RRoneHandler();
//following floats are used for the calculating of the averages at the end of the simulation
float total_wait;
float total_response;
float total_turnAround;

void arrivalEvent(Job recent);
void departureEvent(Job ref);
int randArrivalTime();
int findImminent();
void calculateAvgs(Job done);

CPU one;
CPU two;
float curTime = 0; //curTime is the current time in the simulation
int done = 0; //done represents how many jobs have finished what they need to do 
int eventNum = 0; //this is the event number and it starts at 0, incremented every time a new event is created
int arrivalNum = 0; //keep track of how many arrivals have occured
const int bufferTime = 2; //this is how much time occurs between a job leaving a cpu and arriving at a queue 
int main(){
    
    srand(time(NULL)); //seeding time
    dataFile.open("ProcessInfo.txt"); //opening connection to write to file 
    int ids = 0; //represents the ids of all the jobs being created, starting from 0
    

    //creating 100 jobs with unique ids for the simulation and adding them to the ready queue
    for (int i = 1; i < 15; i++){
        Job hold = Job(i);
        lists.Ready.push(hold);
    }
    
    //Job holdFirst = lists.Ready.front();
    FEL.push_back(Event(eventNum, 0, 1, lists.Ready.front().getPID(), lists.Ready.front()));
    eventNum++;
    while(curTime < 250){
        int holdim = findImminent();
        curTime = FEL[holdim].time;
        Job working = FEL[holdim].getJob();
        int type = FEL[holdim].getType();
        if (type == 1) { arrivalEvent(FEL[holdim].getJob()); }
            else if (type == 2) { departureEvent(FEL[holdim].getJob()); }
        FEL.erase(FEL.begin() + holdim); //removing the FEL event that just got handled
    }
    
    //in here i need to print the process information 
    // while (ids < 51){
    //     // dataPrint(holdName); //printing thread information to the datafile
    //     // assignThread(holdName); //assigning thread to a processor or to a queue
    //     // if (ids % 25 == 0){
    //     //     lists.RR.pop_back();
    //     // }
    //     // if (!lists.Ready.empty() && checkEmpty() != 0){
    //     //     Event placeHolder;
    //     //     placeHolder = lists.Ready.front();
    //     //     assignThread(placeHolder);
    //     //     lists.Ready.erase(lists.Ready.begin());
    //     // }
    //     //may need to change the number input to figure out which thread to start with///////////////////////////////////////////////
    //     RRoneHandler(ids % 5);
    //     ids++; //increment id to ensure PID uniqueness
    // }
    dataFile.close(); //closing connection so file can't be written to anymore


    std::cout << "Avg. Wait Time: " << total_wait / done;
    std::cout << "\nAvg. Response Time: " << total_response / 100;
    std::cout << "\nAvg. Turn Around Time: " << total_turnAround / done << "\n";
    std::cout << "Jobs Completed: " << done << "\n";
    return 0;
    
}


/*
Thoughts:
When a new thread is created check all of the processors to see if any are empty
    if none empty randomly choose number 1-3 and insert into that queue
    if one is empty add to that one
Once in a queue then begin the handling based on what type of queue it is 
    The creation and assigning is assumed to take 5ms
    If all of the processors are taken up then the thread is added to the waiting queue 
    If a processor is available then the thread is added to that vector 
When the process has been allocated then the processors are all able to start doing their work on their queues 

Questions:
Am I creating all of the new threads at the same time or am I running through them while others are being created in the loop?
I'm not sure how the load sharing works with the RR processors 

*/

//purpose of this function is to find the event in the vector that is scheduled to occur next
//loops through the FEL to compare all of their scheduled times and find the lowest one
//the lowest one is the event that should occur next
//then return the index of the next scheduled event 
int findImminent(){
    int min = 1000000000; //supposed to hold the min time, need to start with very large number
    int temp; //hold time of the event 
    int hold; //the index of the event that needs to occur next
    for (int i=0; i < FEL.size(); i++){
        temp = FEL[i].getTime();
        if (temp < min){
            min = temp;
            hold = i;
        }
    }
    return hold;
}
//generate a random amount of time for the next arrival to take place
int randArrivalTime(){
    int arriveTime = rand() % 5 + 1;
    return arriveTime;
}
/*
If first arrival
    Check if more processes available in lists.Ready
        Create new arrival event if available 
            pop from ready queue
    Check if cpu is available
        If available schedule departure event for that job
        If not available add to lists.RR and return
If not first arrival 
    Check if cpu is available
        if not available then add to lists.RRfiveten or lists.FCFS and return



        What I think is broken right now is that I don't think my popping of the queues is happening properly and this is causing the 
        certain jobs to keep occuring 
*/
void arrivalEvent(Job recent){
    recent.incArrival(); //increment the arrival because the job has had another arrival
    recent.setrecentArrival(curTime); //setting most recent arrival time for the wait time calculations
   // int burstRequired = recent.getBurstTime(); //hold how long the job needs
    
    //for jobs arriving for the first time
    if (recent.getArrivalNum() == 1){
      std::cout << "Job " << recent .getPID() << " arrived at " << curTime << "\n";
      std::cout << "Burst Time Required: " << recent.getBurstTime() << "\n" << "\n";
        scheduleNew(); //THIS WAS THE MOST RECENT CHANGE, JUST ADDED A FUNCTION INSTEAD OF AN IF STATEMENT
        recent.setArrivalTime(curTime); //because this is the first arival time we need to record what time the job arrived
        
        //check if the first CPU is being used
        if (one.available != false){ 
        
                //if the time required to finish the job is less than 10 seconds then only need the processor for the remaining amount of time
                if (recent.getBurstTime() <= 10){
                    FEL.push_back(Event(eventNum, curTime + recent.getBurstTime(), 2, recent.getPID(), recent));
                }
                    else { FEL.push_back(Event(eventNum, curTime + 10, 2, recent.getPID(), recent)); }//create a new departure event
            eventNum++;
            one.setAvailable(false); //locking up the cpu because it will now be used by the recent job
            one.setJobid(recent.getPID()); //setting one's JobID equal to the job's id 
            recent.subBurstTime(10); //job will run for 10ms, sub 10ms from required burst time
            return;
        }
        //this runs if processor two is available
        else if (two.available != false){
            //std::cout << "FIrst time and open cpu 2. Job number: " << recent.getPID() << "\n"; 
            //if the time required to finish the job is less than 10 seconds then only need the processor for the remaining amount of time
                if (recent.getBurstTime() <= 10){
                    FEL.push_back(Event(eventNum, curTime + recent.getBurstTime(), 2, recent.getPID(), recent));
                }
                    else { FEL.push_back(Event(eventNum, curTime + 10, 2, recent.getPID(), recent)); }
            eventNum++;
            two.setAvailable(false);
            two.setJobid(recent.getPID());
            recent.subBurstTime(10);
            return;
        }

        else {
            lists.RR.push(recent); //adding the arrived Job to the first RR queue because this is the first time the job arrived
            return;
        }
        
    }
    //for arrivals to the second RR queue
    if (recent.getArrivalNum() == 2){
        
        if (one.available != false){
            //Because this time the job is coming from the second Round Robin Queue, need to see if the remaining time needed to finish job is greater than 15 ms
                if (recent.getBurstTime() <= 15){
                    FEL.push_back(Event(eventNum, curTime + recent.getBurstTime(), 2, recent.getPID(), recent));
                }
                    else { FEL.push_back(Event(eventNum, curTime + 15, 2, recent.getPID(), recent)); } 
            eventNum++;
            one.setAvailable(false);
            one.setJobid(recent.getPID());
            recent.subBurstTime(15);
            recent.addWaitTime(curTime - recent.getRecArrival());

            return;
        }
        else if (two.available != false){
            //Because this time the job is coming from the second Round Robin Queue, need to see if the remaining time needed to finish job is greater than 15 ms
            if (recent.getBurstTime() <= 15){
                FEL.push_back(Event(eventNum, curTime + recent.getBurstTime(), 2, recent.getPID(), recent));
            }
                    else { FEL.push_back(Event(eventNum, curTime + 15, 2, recent.getPID(), recent)); }
            eventNum++;
            two.setAvailable(false);
            two.setJobid(recent.getPID());
            recent.subBurstTime(15);
            recent.addWaitTime(curTime - recent.getRecArrival());
            return;
        }
        else {
            lists.RRfiveten.push(recent);
            return;
        }
    }
    //for arrivals to the FCFS queue
    else {
        if (one.available != false){
            //because this is the FCFS queue it will run until the job is finished
            FEL.push_back(Event(eventNum, curTime + recent.getBurstTime(), 2, recent.getPID(), recent));
            eventNum++;
            one.setAvailable(false);
            one.setJobid(recent.getPID());
            recent.zeroBurst();
            recent.addWaitTime(curTime - recent.getRecArrival());
            return;
        }
        else if (two.available != false){
            //because this is the FCFS queue the job will run until complete
                FEL.push_back(Event(eventNum, curTime + recent.getBurstTime(), 2, recent.getPID(), recent));
                eventNum++;
                two.setAvailable(false);
                two.setJobid(recent.getPID());
                recent.zeroBurst();
                recent.addWaitTime(curTime - recent.getRecArrival());
                return;
        }
        //if both of the other queues are not empty or neither of the processors is available then add job to the FCFS queue 
        else {
            lists.FCFS.push(recent);
            return;
        }

    }
}
void departureEvent(Job ref){
        //Testing to see what the specs are after departure
        std::cout << "Job: " << ref.getPID() << ". Burst Remaining: " << ref.getBurstTime() << ". Curtime: " << curTime << "\n";
       
        //first needto check if this job is now completed, if it is then need to calculate it's averages and finish the job
        if (ref.getBurstTime() < 1){
            ref.setFinishTime(curTime); //set the finish time to 
            ref.setTurnAroundTime(curTime - ref.getArrivalTime());
            ref.setWaitTime(curTime - ref.holdBurst - ref.getArrivalTime() - ((ref.getArrivalNum() -1)*2));
            calculateAvgs(ref);
            done++;
            std::cout << "Job: " << ref.getPID() << " finished at " << curTime << "\n"; 
        }
        //if the job is not finished then need to schedule a new arrival for the job
            else {
                FEL.push_back(Event(eventNum, curTime + 2, 1, ref.getPID(), ref));
                eventNum++;
            }
        //next need to make sure the availability of the cpu that was occupied is now free
        if (one.getJobId() == ref.getPID()){ 
            one.setAvailable(true); }
                else { two.setAvailable(true); }
        //upon departure check if the first RR queue is empty, if not then execute if statement
        if ( !lists.RR.empty() ){
            //get the first job in line from the first RR queue
            //Job RRone = lists.RR.front();
            //check which CPU is available and set the availability to false and jobID to the PID of the occupying job
            if (one.getAvailable() != false){
                one.setAvailable(false);
                one.setJobid(lists.RR.front().getPID());
                }
                else {
                    two.setAvailable(false);
                    two.setJobid(lists.RR.front().getPID());
                }
            if (lists.RR.front().getBurstTime() <= 10){
                FEL.push_back(Event(eventNum, curTime + lists.RR.front().getBurstTime(), 2, lists.RR.front().getPID(), lists.RR.front()));
                eventNum++;
                lists.RR.front().subBurstTime(lists.RR.front().getBurstTime());
                lists.RR.front().addWaitTime(curTime - lists.RR.front().getRecArrival());
            }
            
            //this is if the burst time is greater than the maximum 10ms
            lists.RR.front().subBurstTime(5);
            lists.RR.front().subBurstTime(5);
            lists.RR.front().addWaitTime(curTime - lists.RR.front().getRecArrival());
            lists.RR.front().setResponseTime(curTime - lists.RR.front().getArrivalTime());
            FEL.push_back(Event(eventNum, curTime + 10, 2, ref.getPID(), lists.RR.front()));
            eventNum++;
            lists.RR.pop();
            
        }
        //if the first RR queue is empty, check the second RR queue, if not empty execute
        else if ( !lists.RRfiveten.empty() ){
            //get the first job in line from the second RR queue if the first RR queue is empty
            //Job RR2one = lists.RRfiveten.front();
            //check which CPU is available and set the availability to false and jobID to the PID of the occupying job
            if (one.getAvailable() != false){
                one.setAvailable(false);
                one.setJobid(lists.RRfiveten.front().getPID());
                }
                else {
                    two.setAvailable(false);
                    two.setJobid(lists.RRfiveten.front().getPID());
                }
            if (lists.RRfiveten.front().getBurstTime() <= 15){
                FEL.push_back(Event(eventNum, curTime + lists.RRfiveten.front().getBurstTime(), 2, lists.RRfiveten.front().getPID(), lists.RRfiveten.front()));
                eventNum++;
                lists.RRfiveten.front().subBurstTime(lists.RRfiveten.front().getBurstTime());
                lists.RRfiveten.front().addWaitTime(curTime - lists.RRfiveten.front().getRecArrival());
            }
            
            //this is if the burst time is greater than the maximum 15ms
            lists.RRfiveten.front().subBurstTime(15);
            lists.RRfiveten.front().addWaitTime(curTime - lists.RRfiveten.front().getRecArrival());
            FEL.push_back(Event(eventNum, curTime + 10, 2, ref.getPID(), lists.RRfiveten.front()));
            eventNum++;
            lists.RRfiveten.pop();
            
        }
        //if both RR queues are empty then check the FCFS queue to see if it's empty, if not then execute else if statement
        else if ( !lists.FCFS.empty() ){
            //get the first job in line from the first RR queue
            //Job fcfs = lists.FCFS.front();
            //check which CPU is available and set the availability to false and jobID to the PID of the occupying job
            if (one.getAvailable() != false){
                one.setAvailable(false);
                one.setJobid(lists.FCFS.front().getPID());
                }
                else {
                    two.setAvailable(false);
                    two.setJobid(lists.FCFS.front().getPID());
                }
            
            FEL.push_back(Event(eventNum, curTime + lists.FCFS.front().getBurstTime(), 2, lists.FCFS.front().getPID(), lists.FCFS.front()));
            eventNum++;
            lists.FCFS.front().zeroBurst();
            lists.FCFS.front().addWaitTime(curTime - lists.FCFS.front().getRecArrival());
            lists.FCFS.pop();
            
            
        }
        

}
//the calculate Avgs function is used to add information to the totals for all of the jobs to be used after the simulation is done
void calculateAvgs(Job done){
    //calculate turnaround time
    int turnAround = done.getArrivalTime() + done.getFinishTime();
    total_turnAround += turnAround;
    //calculate wait time
    int wait = done.getWaitTime();
    total_wait += wait;
    //calculate response time
    int response = done.getResponseTime();
    total_response += response;
    return;
}
void scheduleNew(){
        if (!lists.Ready.empty()){
            lists.Ready.pop(); //because this is the first time this job is being added to a queue we need to remove it from the ready queue
            FEL.push_back(Event(eventNum, curTime+randArrivalTime(), 1, lists.Ready.front().getPID(), lists.Ready.front())); //create arrival for the next job
            eventNum++;
            //lists.Ready.pop(); //because this is the first time this job is being added to a queue we need to remove it from the ready queue 
        }
        return;
}
// void dataPrint(Job recentEvent){
//     dataFile << "Pid: " << recentEvent.getPID();
//     dataFile << ", Arrival Time: " << curTime;
//     dataFile << ", Wait Time: " << recentEvent.getWaitTime() << "ms";
//     dataFile << ", Turn Around Time: " << recentEvent.getTurnAround() << "ms";
//     dataFile << ", Remaining Burst Time: " << recentEvent.getBurstTime() << "ms\n";
//     return;
// }void dataPrint(Job recentEvent){
//     dataFile << "Pid: " << recentEvent.getPID();
//     dataFile << ", Arrival Time: " << curTime;
//     dataFile << ", Wait Time: " << recentEvent.getWaitTime() << "ms";
//     dataFile << ", Turn Around Time: " << recentEvent.getTurnAround() << "ms";
//     dataFile << ", Remaining Burst Time: " << recentEvent.getBurstTime() << "ms\n";
//     return;
// }
