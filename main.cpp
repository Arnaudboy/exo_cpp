#include <iostream>
#include <string>
#include <thread>
#include <condition_variable>
#include <queue>
#include <cstdlib>
#include <chrono>
#include <ctime>
#include <random>
#include <atomic>
#include "cmd_ops.h"

//counts every number that is added to the queue
static long long producer_count = 0;
//counts every number that is taken out of the queue
static long long consumer_count = 0;

void generateNumbers(std::queue<int> & numbers, std::condition_variable & cv, std::mutex & m, std::atomic<bool> & workdone)
{
    while(!workdone.load())
    {   
        std::unique_lock<std::mutex> lk(m);
        string value;
        std::cout << "Input temperature: ";
        getline(std::cin, value);
        int val = 0;
        try {
            val = std::stoi(value);
            value.clear();
            std::cout << "Understood value: " << val << endl;
            numbers.push(val);
            producer_count++;
        } catch (std::invalid_argument e) {
            std::cerr << e.what();
        }
        cv.notify_one(); // Notify worker
        cv.wait(lk); // Wait for worker to complete
     }
}

void work(std::queue<int> & numbers, std::condition_variable & cv, std::mutex & m, std::atomic<bool> & workdone)
{
    while(!workdone.load())
    {
        std::unique_lock<std::mutex> lk(m);
        if (numbers.size() > 0) {
            std::cout << "Received: " << numbers.front() << std::endl;
            numbers.pop();
            consumer_count++;
            if (consumer_count >= 4) {
                std::cout << "That's enough" << endl;
                workdone.store(true);
                cv.notify_one();
                break;
            }
        }
        cv.notify_one(); // Notify generator (placed here to avoid waiting for the lock)
        cv.wait(lk); // Wait for the generator to complete
     }
}

void record_and_save () {
    std::condition_variable cv;
    std::mutex m;
    std::atomic<bool> workdone(false);
    std::queue<int> numbers;

    //start threads
    std::thread producer(generateNumbers, std::ref(numbers), std::ref(cv), std::ref(m), std::ref(workdone));
    std::thread consumer(work, std::ref(numbers), std::ref(cv), std::ref(m), std::ref(workdone));


    //wait for 3 seconds, then join the threads
    std::this_thread::sleep_for(std::chrono::seconds(4));
    // workdone = true;
    // cv.notify_all(); // To prevent dead-lock

    producer.join();
    consumer.join();

    //output the counters
    std::cout << producer_count << std::endl;
    std::cout << consumer_count << std::endl;

}

int main(int argc, const char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage record_reader [-pr] [-j][num] -f [file]" << endl << endl
        << "\t-f file" << endl
        << "\t\tbinary file containing the record" << endl << endl
        << "\t-p" << endl
        << "\t\tplay record" << endl << endl
        << "\t-r" << endl
        << "\t\trun record" << endl << endl
        << "\t-j num" << endl
        << "\t\trun with option -p" << endl
        << "\t\tjump at num percent of the record and play from there" << endl;
        return 1;
    }
    struct MyOpts
    {
        string fileOpt{};
        int intOpt{};
        bool doRecord{};
        bool doPlay{};
    };

    auto parser = CmdOpts<MyOpts>::Create({
        {"-f", &MyOpts::fileOpt },
        {"-j", &MyOpts::intOpt},
        {"-r", &MyOpts::doRecord },
        {"-p", &MyOpts::doPlay }});

    auto myopts = parser->parse(argc, argv);

    std::cout << "stringOpt = " << myopts.fileOpt << endl;
    std::cout << "doRecord = " << myopts.doRecord << endl;
    if (myopts.doRecord) {
        record_and_save();
    }
    return 0;
}