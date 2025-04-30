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

#ifndef STRUCT_HPP
#define STRUCT_HPP

#include <cstdint>

#pragma pack(push, 1)
struct request_insert_header {
    uint8_t request_type_;
    uint64_t quota_;
    uint64_t usage_;
    uint8_t ttl_type_;
    uint64_t ttl_;
    uint8_t consumer_id_size_;
    uint8_t resource_id_size_;
};
#pragma pack(pop)

#endif //STRUCT_HPP
