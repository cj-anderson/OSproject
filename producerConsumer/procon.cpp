#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
#include <sstream>


std::vector<int> buffer;
int bufferSize = 5;
std::mutex bufferMutex;

class ProducerThread {
public:
    void operator()() {
        while (true) {
            {
                std::lock_guard<std::mutex> lock(bufferMutex);
                if (buffer.size() < bufferSize) 
                {
                    int item = rand() % 100 + 1;
                    buffer.push_back(item);
                    std::cout << "Producer produced: " << item << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 5000 + 1000));
        }
    }
};

class ConsumerThread {
public:
    void operator()() {
        while (true) {
            {
                std::lock_guard<std::mutex> lock(bufferMutex);
                if (!buffer.empty()) 
                {
                    int item = buffer.front();
                    buffer.erase(buffer.begin());
                    std::cout << "Consumer consumed: " << item << std::endl;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(rand() % 5000 + 1000));
        }
    }
};

int main() {
    std::string filePath = "producerConsumer/datafile.txt";
    std::ifstream file(filePath);

    if (!file.is_open()) 
    {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) 
    {
        std::istringstream iss(line);
        int numProducers, numConsumers;
        if (!(iss >> numProducers >> numConsumers)) 
        {
            std::cerr << "Error reading file." << std::endl;
            return 1;
        }

        for (int i = 0; i < numProducers; ++i) 
        {
            std::thread producerThread(ProducerThread{});
            producerThread.detach();
        }

        for (int i = 0; i < numConsumers; ++i) 
        {
            std::thread consumerThread(ConsumerThread{});
            consumerThread.detach();
        }
    }

    file.close();
    return 0;
}