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


int main(const int argc, char const * const * argv) {
    
    // Check that a file path is provided
    if (argc < 2) {
        std::cerr << "Please provide an input file path." << std::endl;
        return 1;
    }

    // Path to the output file
    std::string outputFilePath = "output.txt";
    
    // Open the output file in write mode
    std::ofstream outputFile(outputFilePath);
    
    // Check if the output file is open
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file." << std::endl;
        return 1;
    }

    // Redirect std::cout and std::cerr to the output file
    std::streambuf* originalCoutBuf = std::cout.rdbuf(outputFile.rdbuf());
    std::streambuf* originalCerrBuf = std::cerr.rdbuf(outputFile.rdbuf());

    // Path to the input file
    std::string filePath = argv[1];
    
    // Open the input file
    std::ifstream inputFile(filePath);
    
    if (!inputFile.is_open()) {
        std::cerr << "Error opening input file." << std::endl;
        return 1;
    }
    
    // Read each line from the file
    std::string line;
    while (std::getline(inputFile, line)) {
        // Process each line here
        std::istringstream iss(line);
        int numProducers, numConsumers;
        if (!(iss >> numProducers >> numConsumers)) {
            std::cerr << "Error reading input file." << std::endl;
            return 1;
        }

        // Create producer threads
        for (int i = 0; i < numProducers; ++i) {
            std::thread producerThread(ProducerThread{});
            producerThread.detach();
        }

        // Create consumer threads
        for (int i = 0; i < numConsumers; ++i) {
            std::thread consumerThread(ConsumerThread{});
            consumerThread.detach();
        }
    }

    // Close the input file
    inputFile.close();
    
    // Reset std::cout and std::cerr to their original states
    std::cout.rdbuf(originalCoutBuf);
    std::cerr.rdbuf(originalCerrBuf);

    // Close the output file
    outputFile.close();

    return 0;
}
