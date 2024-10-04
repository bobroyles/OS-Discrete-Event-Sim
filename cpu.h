#include <iostream>



class CPU {

    public:
        bool available = true;
        int jobid;

        //constructor, dont need an argumented constructor since the availability will always be 0 (available) to start
        CPU() { available = true;} 

        int getAvailable() { return available; }
        int getJobId() { return jobid; };

        void setAvailable(int _available) { available = _available; };
        void setJobid(int id) { jobid = id; };


};