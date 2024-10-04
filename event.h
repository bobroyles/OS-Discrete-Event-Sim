#include <iostream>

class Event {
    public:
        int eventNum;
        int time;
        int type;
        int jobID;
        Job task;

    //Constructor
        Event(int Enum, int _time, int _type, int ID, Job _task){
            eventNum = Enum;
            time = _time;
            type = _type;
            jobID = ID;
            task = _task;
        };
    //getters and setters
        int getEnum() { return eventNum; };
        int getTime() { return time; };
        int getType() { return type; };
        int getJobID() { return jobID; };
        Job getJob() { return task; };

        void setTask(Job _task) { task = _task; };
        void setEnum(int Enum) { eventNum = Enum; };
        void setTime(int _time) { time = _time; };
        void setType(int _type) { type = _type; };
        void setJobID(int id) { jobID = id; };

};