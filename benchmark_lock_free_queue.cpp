#include <iostream>
#include <thread>
#include <chrono>
#include <deque>
#include <atomic>
#include "includes/MarketTick.hpp"
#include "includes/LockFreeSPSCQueue.hpp"

using std::cout;
using std::endl;
using std::thread;
using std::deque;
using std::atomic;



constexpr uint32_t MAX_BUFFER_SIZE = 8192;
constexpr uint64_t MAX_PRODUCER_COUNT = 1000000000; // 1 billion

LockFreeSPSCQueue<MarketTick> queue(MAX_BUFFER_SIZE);

atomic producer_done{false};
atomic<uint64_t> producer_count(0);
atomic<uint64_t> consumer_count(0);


void produce() {
    uint64_t id = 0;
    while (id < MAX_PRODUCER_COUNT) {
        MarketTick tick = generateMarketTick(id);
        if (queue.enqueue(tick)) {
            ++producer_count;
            ++id;
        }
    }
    producer_done = true;
}

void consume() {
    while (true) {
        MarketTick tick{};
        if (queue.dequeue(tick)) {
            // In real world, I would process the tick here
            ++consumer_count;
        } else if (producer_done.load() && queue.empty()) {
            // If producer is done and queue is empty, exit
            break;
        }
    }
}



int main()
{
    const auto start = std::chrono::high_resolution_clock::now();
    thread producer(produce);
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