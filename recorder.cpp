#include <iostream>
#include <string>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <memory>
#include "recorder.h"
#include "FileRecorder.h"

// counts every number that is added to the queue
static long long producer_count = 0;
// counts every number that is taken out of the queue
static long long consumer_count = 0;

void capture_user_input(std::queue<int> &numbers, std::condition_variable &cv, std::mutex &m, std::atomic<bool> &workdone)
{
    while (!workdone.load())
    {
        std::unique_lock<std::mutex> lk(m);
        std::string value;
        std::cout << "Input value: ";
        getline(std::cin, value);
        if (value == "stop")
        {
            workdone = true;
            cv.notify_one();
            break;
        }
        else
        {
            int val = 0;
            try
            {
                val = std::stoi(value);
                value.clear();
                std::cout << "Understood value: " << val << std::endl;
                numbers.push(val);
                producer_count++;
            }
            catch (std::invalid_argument e)
            {
                std::cerr << e.what();
            }
        }
        cv.notify_one(); // Notify worker
        cv.wait(lk);     // Wait for worker to complete
    }
}

void record_user_input(std::queue<int> &numbers, std::condition_variable &cv, std::mutex &m, std::atomic<bool> &workdone, std::string &&file_name)
{
    std::chrono::time_point<std::chrono::system_clock> start;
    bool bStarted = false;
    FileRecorder fileRecorder(std::forward<std::string &&>(file_name));
    std::unique_lock<std::mutex> lk(m);
    while (!workdone.load())
    {
        if (numbers.size() > 0)
        {
            std::cout << "Received: " << numbers.front() << std::endl;
            if (!bStarted)
            {
                start = std::chrono::system_clock::now();
                bStarted = true;
            }
            else
            {
                auto current = std::chrono::system_clock::now();
                auto current_time = std::chrono::duration_cast<std::chrono::seconds>(current - start);
                fileRecorder.add_measure(numbers.front(), current_time);
                numbers.pop();
            }
            consumer_count++;
        }
        cv.notify_one(); // Notify generator (placed here to avoid waiting for the lock)
        cv.wait(lk);     // Wait for the generator to complete
    }
    if (numbers.size() > 0)
    {
        auto current = std::chrono::system_clock::now();
        auto current_time = std::chrono::duration_cast<std::chrono::seconds>(current - start);
        fileRecorder.add_measure(numbers.front(), current_time);
        numbers.pop();
        assert(numbers.size() == 0);
    }
    fileRecorder.end_record();
}

void record_and_save(std::string &&file_name)
{
    std::condition_variable cv;
    std::mutex m;
    std::atomic<bool> workdone(false);
    std::queue<int> numbers;

    // start threads
    std::thread producer(capture_user_input, std::ref(numbers), std::ref(cv), std::ref(m), std::ref(workdone));
    std::thread consumer(record_user_input, std::ref(numbers), std::ref(cv), std::ref(m), std::ref(workdone), std::forward<std::string &&>(file_name));

    producer.join();
    consumer.join();

    // output the counters
    std::cout << "Produced " << producer_count << " measurements" << std::endl;
    std::cout << "Consumed " << consumer_count << " measurements" << std::endl;
}
