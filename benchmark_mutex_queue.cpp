#include <iostream>
#include <thread>
#include <chrono>
#include <deque>
#include <mutex>
#include <atomic>
#include <condition_variable>

#include "includes/MarketTick.hpp"

using std::cout;
using std::endl;
using std::thread;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::deque;
using std::atomic;


mutex mtx;
condition_variable cond;

deque<MarketTick> deq;
constexpr uint32_t MAX_BUFFER_SIZE = 8192;
constexpr uint64_t MAX_PRODUCER_COUNT = 1000000000; // 1 billion

atomic producer_done{false};
atomic<uint64_t> producer_count(0);
atomic<uint64_t> consumer_count(0);


void produce(const uint64_t total_ticks) {
    uint64_t id = 0;
    while (id < total_ticks) {
        MarketTick tick = generateMarketTick(id);
        unique_lock lock(mtx);
        cond.wait(lock, []() -> bool {
            return deq.size() < MAX_BUFFER_SIZE;
        });

        deq.push_front(tick);
        ++producer_count;
        ++id;
        cond.notify_one();
    }
    producer_done = true;
    cond.notify_one();
}

void consume() {
    while (true) {
        unique_lock lock(mtx);
        cond.wait(lock, []() -> bool {
            return !deq.empty() || producer_done.load();
        });

        // Exit if producer is done and queue is empty
        if (producer_done.load() && deq.empty()) {
            break;
        }
        deq.pop_back();
        ++consumer_count;
        cond.notify_one();
    }
}



int main()
{
    const auto start = std::chrono::high_resolution_clock::now();
    thread producer(produce, MAX_PRODUCER_COUNT);
    thread consumer(consume);

    producer.join();
    consumer.join();

    const auto end = std::chrono::high_resolution_clock::now();

    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Buffer size: " << MAX_BUFFER_SIZE << endl;
    cout << "Producer count: " << producer_count << endl;
    cout << "Consumer count: " << consumer_count << endl;
    cout << "Total time taken: " << duration.count() << " milliseconds" << endl;
}