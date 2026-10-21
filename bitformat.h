/*
  Copyright 2013-2026 Mike Bergmann

  This file is part of Registerer.

  Registerer is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 2 of the License, or
  (at your option) any later version.

  Registerer is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Registerer.  If not, see <http://www.gnu.org/licenses/>.
*/

// Pure number <-> text logic

#pragma once

#include <algorithm>
#include <bit>
#include <charconv>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <system_error>

namespace registerer {

inline constexpr int kBits = 64;

enum class Base : int { Bin = 2, Dec = 10, Hex = 16 };

struct Value
{
    std::uint64_t number = 0;
    Base base = Base::Dec;

    friend bool operator==(const Value &, const Value &) = default;
};

inline std::string digits(std::uint64_t n, Base base)
{
    char buf[kBits + 1];
    const auto [end, ec] = std::to_chars(buf, buf + sizeof buf, n, static_cast<int>(base));
    return std::string(buf, end); // to_chars cannot overflow a 65 byte buffer
}

inline std::string format(Value v)
{
    const std::string_view prefix = v.base == Base::Bin ? "0b" : v.base == Base::Hex ? "0x" : "";
    return std::string(prefix) + digits(v.number, v.base);
}

inline std::optional<Value> parse(std::string_view text)
{
    constexpr std::string_view kSpace = " \t";
    const auto last = text.find_last_not_of(kSpace);
    if (last == std::string_view::npos)
        return std::nullopt; // empty or all blanks
    const auto first = text.find_first_not_of(kSpace);
    text = text.substr(first, last - first + 1);

    Base base = Base::Dec;
    if (text.starts_with("0b") || text.starts_with("0B")) {
        base = Base::Bin;
        text.remove_prefix(2);
    } else if (text.starts_with("0x") || text.starts_with("0X")) {
        base = Base::Hex;
        text.remove_prefix(2);
    }

    std::uint64_t n = 0;
    const auto [end, ec] =
            std::from_chars(text.data(), text.data() + text.size(), n, static_cast<int>(base));
    if (ec != std::errc{} || end != text.data() + text.size())
        return std::nullopt;
    return Value{ .number = n, .base = base };
}

constexpr Base next(Base base)
{
    return base == Base::Dec ? Base::Hex : base == Base::Hex ? Base::Bin : Base::Dec;
}

constexpr std::uint64_t mask(int bit)
{
    return bit >= 0 && bit < kBits ? std::uint64_t{ 1 } << bit : 0;
}

constexpr std::uint64_t toggle(std::uint64_t n, int bit)
{
    return n ^ mask(bit);
}

struct Rows
{
    std::string bin, pos, hex, dec;
};

inline Rows layout(std::uint64_t value, int caret)
{
    constexpr std::string_view kDigits = "0123456789abcdef";
    const int nibbles = std::max(1, (std::bit_width(value) + 3) / 4);

    Rows r;
    for (int n = nibbles - 1; n >= 0; --n) {
        for (int b = 3; b >= 0; --b) {
            const int bit = 4 * n + b;
            r.bin += static_cast<char>('0' + ((value >> bit) & 1));
            r.pos += bit == caret ? '^' : ' ';
            r.hex += b == 0 ? kDigits[(value >> (4 * n)) & 0xF] : ' ';
        }
        r.bin += ' ';
        r.pos += ' ';
        r.hex += ' ';
    }

    r.dec = digits(value, Base::Dec);
    const std::size_t width = r.bin.size() - 1; // ignore the trailing group gap
    if (r.dec.size() < width)
        r.dec.insert(0, width - r.dec.size(), ' ');
    return r;
}

} // namespace registerer
