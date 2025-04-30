// Copyright (C) 2025 Ian Torres
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include <fstream>
#include <cstring>
#include <cstdint>
#include <chrono>
#include <iostream>
#include <bit>

#include "struct.hpp"

constexpr size_t N = 100'000'000;

uint64_t bswap64(uint64_t x) {
    return __builtin_bswap64(x);
}

request_insert_header parse_be(const uint8_t* ptr) {
    using std::bit_cast;
    request_insert_header h;
    h.request_type_ = ptr[0];
    h.quota_ = bswap64(bit_cast<uint64_t>(*reinterpret_cast<const uint64_t*>(ptr + 1)));
    h.usage_ = bswap64(bit_cast<uint64_t>(*reinterpret_cast<const uint64_t*>(ptr + 9)));
    h.ttl_type_ = ptr[17];
    h.ttl_ = bswap64(bit_cast<uint64_t>(*reinterpret_cast<const uint64_t*>(ptr + 18)));
    h.consumer_id_size_ = ptr[26];
    h.resource_id_size_ = ptr[27];
    return h;
}

int main() {
    alignas(8) uint8_t raw[28] = {};
    constexpr uint64_t quota = 123456789;
    constexpr uint64_t usage = 987654321;
    constexpr uint64_t ttl   = 5555555555;

    raw[0] = 0x01;
    const uint64_t quota_be = bswap64(quota);
    const uint64_t usage_be = bswap64(usage);
    const uint64_t ttl_be   = bswap64(ttl);
    std::memcpy(raw + 1,  &quota_be, 8);
    std::memcpy(raw + 9,  &usage_be, 8);
    raw[17] = 0x02;
    std::memcpy(raw + 18, &ttl_be, 8);
    raw[26] = 16;
    raw[27] = 20;

    const auto t1 = std::chrono::high_resolution_clock::now();
    volatile uint64_t accumulator = 0;

    for (size_t e = 0; e < 100; ++e) {
        accumulator = 0;

        for (size_t i = 0; i < N; ++i) {
            const auto h = parse_be(raw);
            accumulator += h.quota_ + h.usage_ + h.ttl_;
        }
    }
    const auto t2 = std::chrono::high_resolution_clock::now();

    const auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
    std::cout << "Time: " << elapsed << " ns\n";
    return 0;
}
