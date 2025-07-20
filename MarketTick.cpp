//
// Created by Sultan Mahmud on 20/07/2025.
//

#include "includes/MarketTick.hpp"
#include <random>
#include <cstring>
#include <chrono>

MarketTick generateMarketTick(const uint64_t id) {
    thread_local std::mt19937_64 rng(std::random_device{}());
    thread_local std::uniform_real_distribution price_dist(100.0, 500.0);
    thread_local std::uniform_int_distribution qty_dist(1, 1000);
    thread_local const char* symbols[] = {"AMPL", "GOOG", "MSFT", "AMZN"};

    MarketTick tick{};
    tick.timestamp = std::chrono::steady_clock::now().time_since_epoch().count();
    tick.price = price_dist(rng);
    tick.quantity = qty_dist(rng);

    const char* sym = symbols[id % 4];
    std::memset(tick.product, 0, sizeof(tick.product));
    std::memcpy(tick.product, sym, std::strlen(sym));

    return tick;
}

