#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

// Define a structure to represent a process
struct Process {
    int pid; // Process ID
    int arrivalTime; // Arrival time
    int priority; // Priority
    int burstTime; // CPU burst time
    int startTime; // Start time
    int waitingTime; // Waiting time
    int turnaroundTime; // Turnaround time

    Process(int id, int at, int bt, int prio) : pid(id), arrivalTime(at), burstTime(bt), priority(prio),
        startTime(0), waitingTime(0), turnaroundTime(0) {}
};

//read simulated processes data from file and store them in a queue
void readProcessesFromFile(queue<Process>& processes) {
    ifstream inputFile("datafile.txt");
    if (!inputFile) {
        cerr << "Error: Unable to open file!" << endl;
        exit(1);
    }

    int id = 0, arrivalTime, burstTime, priority;
    while (inputFile >> arrivalTime >> burstTime >> priority) {
        processes.push(Process(id, arrivalTime, burstTime, priority));
        ++id;

        //cout << processes.size() << "\n";
    }
    inputFile.close();
}


// Compares processes based on their arrival times
struct ArrivalComparator {
    bool operator()(const Process& p1, const Process& p2) const {
        return p1.arrivalTime > p2.arrivalTime;
    }
};

// Compares processes based on their burst times for SJF
struct BurstTimeComparator {
    bool operator()(const Process& p1, const Process& p2) const {
        return p1.burstTime > p2.burstTime;
    }
};

// Compares processes based oon priorities for Priority Scheduling
struct PriorityComparator {
    bool operator()(const Process& p1, const Process& p2) const {
        return p1.priority > p2.priority;
    }
};

// Function to simulate the CPU scheduler using FIFO algorithm
static queue<Process> simulateFIFO(queue<Process>& processes) {
    std::queue<Process> completed;
    
    int currentTime = 0;
    while (!processes.empty()) {
        Process currentProcess = processes.front();
        processes.pop();
        if (currentProcess.arrivalTime > currentTime) {
            currentTime = currentProcess.arrivalTime;
        }
        currentProcess.startTime = currentTime;
        currentProcess.turnaroundTime = currentTime + currentProcess.burstTime - currentProcess.arrivalTime;
        currentProcess.waitingTime = currentProcess.startTime - currentProcess.arrivalTime;
        currentTime += currentProcess.burstTime;

        completed.push(currentProcess);
    }

    return completed;
}

// Function to simulate the CPU scheduler using SJF algorithm (Non-preemptive)
static queue<Process> simulateSJF(queue<Process>& processes) {

    std::queue<Process> completed2;
    int currentTime = 0;
    priority_queue<Process, vector<Process>, BurstTimeComparator> readyQueue;
    

    while (!processes.empty() || !readyQueue.empty()) {
        while (!processes.empty() && processes.front().arrivalTime <= currentTime) {
            readyQueue.push(processes.front());
            processes.pop();
        }
        if (!readyQueue.empty()) 
        {
            Process currentProcess = readyQueue.top();
            readyQueue.pop();
            currentProcess.startTime = currentTime;
            currentProcess.turnaroundTime = currentTime + currentProcess.burstTime - currentProcess.arrivalTime;
            currentProcess.waitingTime = currentProcess.startTime - currentProcess.arrivalTime;
            currentTime += currentProcess.burstTime;
            completed2.push(currentProcess);
        } 
        else 
        {
            currentTime = processes.front().arrivalTime;
        }
    }

    return completed2;
}

// Function to simulate the CPU scheduler using Priority Scheduling algorithm (Preemptive)
static queue<Process> simulatePriority(queue<Process>& processes) 
{
    
    std::queue<Process> completed3;
    int currentTime = 0;
    priority_queue<Process, vector<Process>, PriorityComparator> readyQueue;
    while (!processes.empty() || !readyQueue.empty()) 
    {
        while (!processes.empty() && processes.front().arrivalTime <= currentTime) {
            readyQueue.push(processes.front());
            processes.pop();
        }
        if (!readyQueue.empty()) {
            Process currentProcess = readyQueue.top();
            readyQueue.pop();
            currentProcess.startTime = currentTime;
            currentProcess.turnaroundTime = currentTime + currentProcess.burstTime - currentProcess.arrivalTime;
            currentProcess.waitingTime = currentProcess.startTime - currentProcess.arrivalTime;
            currentTime += currentProcess.burstTime;
            completed3.push(currentProcess);
        } else {
            currentTime = processes.front().arrivalTime;
        }

        
    }
    return completed3;
}


void printStatistics(const queue<Process>& processes, ofstream& outFile) {
    int totalElapsed = 0;
    int totalBurstTime = 0;
    int totalWaitingTime = 0;
    int totalTurnaroundTime = 0;
    int totalResponseTime = 0;
    int numProcesses = processes.size();

    queue<Process> tempQueue = processes;
    while (!tempQueue.empty()) {
        Process process = tempQueue.front();
        totalBurstTime += process.burstTime;
        totalElapsed = max(totalElapsed, process.arrivalTime + process.burstTime);
        totalWaitingTime += (totalElapsed - process.arrivalTime - process.burstTime);
        totalTurnaroundTime += (totalElapsed - process.arrivalTime);
        totalResponseTime += (totalElapsed - process.arrivalTime);
        tempQueue.pop();
    }

    double throughput = static_cast<double>(totalBurstTime) / numProcesses;
    double cpuUtilization = static_cast<double>(totalBurstTime) / totalElapsed;
    double avgWaitingTime = static_cast<double>(totalWaitingTime) / numProcesses;
    double avgTurnaroundTime = static_cast<double>(totalTurnaroundTime) / numProcesses;
    double avgResponseTime = static_cast<double>(totalResponseTime) / numProcesses;

    // Print statistics to terminal
    cout << "Number of processes: " << numProcesses << endl;
    cout << "Total elapsed time: " << totalElapsed << endl;
    cout << "Throughput: " << throughput << endl;
    cout << "CPU utilization: " << cpuUtilization << endl;
    cout << "Average waiting time: " << avgWaitingTime << endl;
    cout << "Average turnaround time: " << avgTurnaroundTime << endl;
    cout << "Average response time: " << avgResponseTime << endl;

    // Write statistics to output file
    outFile << "Number of processes: " << numProcesses << endl << endl;
    outFile << "Total elapsed time: " << totalElapsed << endl << endl;
    outFile << "Throughput: " << throughput << endl << endl;
    outFile << "CPU utilization: " << cpuUtilization << endl << endl;
    outFile << "Average waiting time: " << avgWaitingTime << endl << endl;
    outFile << "Average turnaround time: " << avgTurnaroundTime << endl << endl;
    outFile << "Average response time: " << avgResponseTime << endl << endl;
}




