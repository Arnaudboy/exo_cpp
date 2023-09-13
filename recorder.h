#include <queue>
#include <atomic>
#include <condition_variable>


void record_user_input(std::queue<int> & numbers, std::condition_variable & cv, std::mutex & m, std::atomic<bool> & workdone, std::string&& file_name);
void capture_user_input(std::queue<int> & numbers, std::condition_variable & cv, std::mutex & m, std::atomic<bool> & workdone);
void record_and_save(std::string&& file_name);