#include <queue>
#include <atomic>
#include <condition_variable>


void work(std::queue<int> & numbers, std::condition_variable & cv, std::mutex & m, std::atomic<bool> & workdone);
void generateNumbers(std::queue<int> & numbers, std::condition_variable & cv, std::mutex & m, std::atomic<bool> & workdone);
void record_and_save ();