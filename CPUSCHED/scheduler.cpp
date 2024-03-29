#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

//Structure to represent a process
struct Process
{
    //process ID
    int pid; 
    //Arrival Time
    int arrivalTime;
    int priority;
    int burstTime;
    int startTime;
    int waitingTime;
    int turnaroundTime;

    Process (int id, int at, int prio, int bt) : pid(id), arrivalTime(at), priority(prio), burstTime(bt),
        startTime(0), waitingTime(0), turnaroundTime(0) {}
};

//Comparator for priority queue based on arrival time
struct ArrivalComparator
{
    bool operator()(const Process& p1, const Process& p2) const
    {
        return p1.arrivalTime > p2.arrivalTime;
    }
};

//priority queue based on burst time for SJF
struct BurstTimeComparator
{
    bool operator()(const Process& p1, const Process& p2) const
    {
        return p1.priority > p2.priority;
    }

};

void simulateFIFO(queue<Process>& processes)
{
    int currentTime = 0;
    while(!processes.empty())
    {
        Process currentProcess = processes.front();
        processes.pop();
        
        if (currentProcess.arrivalTime > currentTime)
        {
            currentTime = currentProcess.arrivalTime;
        }

        currentProcess.startTime = currentTime;
        //equation?
        currentProcess.turnaroundTime = currentTime + currentProcess.burstTime - currentProcess.arrivalTime;
        currentProcess.waitingTime = currentProcess.startTime - currentProcess.arrivalTime;
        currentTime += currentProcess.burstTime;
    }
}

void simulateSJF(queue<Process>& processes)
{
    int currentTime = 0;
    priority_queue<Process, vector<Process>, BurstTimeComparator> readyQueue;
    
    while (!processes.empty() || !readyQueue.empty())
    {
        
    }
}
