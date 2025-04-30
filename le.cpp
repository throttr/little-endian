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
#include <cstdint>
#include <chrono>
#include <iostream>
#include <cstring>

#include "struct.hpp"

constexpr size_t N = 100'000'000;

int main() {
    alignas(8) uint8_t raw[28] = {};
    constexpr request_insert_header init = {
        0x01,              // request_type_
        123456789,         // quota_
        987654321,         // usage_
        0x02,              // ttl_type_
        5555555555,        // ttl_
        16,                // consumer_id_size_
        20                 // resource_id_size_
    };

    std::memcpy(raw, &init, sizeof(init));

    const auto t1 = std::chrono::high_resolution_clock::now();
    volatile uint64_t accumulator = 0;

    for (size_t e = 0; e < 1'000; ++e) {
        accumulator = 0;

        for (size_t i = 0; i < N; ++i) {
            const auto* h = reinterpret_cast<const request_insert_header*>(&raw);
            accumulator += h->quota_ + h->usage_ + h->ttl_;
        }
    }

    const auto t2 = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();

    std::cout << "Time: " << elapsed << " ns\n";
    return 0;
}