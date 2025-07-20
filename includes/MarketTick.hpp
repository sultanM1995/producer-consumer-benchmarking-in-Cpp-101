#pragma once
#include <cstdint>

struct MarketTick {
    uint64_t timestamp; // 8 bytes
    double price; // 8 bytes
    char product[8]; // Assuming product is a fixed-length string of 8 characters, 8 bytes
    uint32_t quantity; // 4 bytes
};

MarketTick generateMarketTick(uint64_t id);