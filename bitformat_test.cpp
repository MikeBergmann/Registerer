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

#include "bitformat.h"

#include <gtest/gtest.h>

using namespace registerer;

TEST(Bitformat, Parse)
{
    EXPECT_TRUE(parse("0b1010") == (Value{ .number = 10, .base = Base::Bin }));
    EXPECT_TRUE(parse("0xFF") == (Value{ .number = 255, .base = Base::Hex }));
    EXPECT_TRUE(parse(" 42 ") == (Value{ .number = 42, .base = Base::Dec }));
    EXPECT_TRUE(parse("0B11") == (Value{ .number = 3, .base = Base::Bin }));
}

TEST(Bitformat, ParseRejectsInvalid)
{
    EXPECT_FALSE(parse(""));
    EXPECT_FALSE(parse("   "));
    EXPECT_FALSE(parse("0b12")); // 2 is not a binary digit
    EXPECT_FALSE(parse("12ab")); // trailing garbage
    EXPECT_FALSE(parse("-1")); // unsigned only
    EXPECT_FALSE(parse("0x1ffffffffffffffff")); // 65 bits: refused, not truncated
}

TEST(Bitformat, FullRangeRoundTrips)
{
    const Value max{ .number = UINT64_MAX, .base = Base::Hex };
    EXPECT_EQ(format(max), "0xffffffffffffffff");
    EXPECT_TRUE(parse(format(max)) == max);
}

TEST(Bitformat, BaseCycle)
{
    EXPECT_EQ(next(Base::Dec), Base::Hex);
    EXPECT_EQ(next(Base::Hex), Base::Bin);
    EXPECT_EQ(next(Base::Bin), Base::Dec);
}

TEST(Bitformat, Toggle)
{
    EXPECT_EQ(format({ .number = toggle(0, 31), .base = Base::Hex }), "0x80000000");
    EXPECT_EQ(format({ .number = toggle(0, 15), .base = Base::Bin }), "0b1000000000000000");
    EXPECT_EQ(toggle(toggle(0xF0, 3), 3), 0xF0u);
    EXPECT_EQ(mask(63), std::uint64_t{ 1 } << 63);
    EXPECT_EQ(mask(64), 0u);
    EXPECT_EQ(mask(-1), 0u);
}

TEST(Bitformat, Layout)
{
    const Rows r = layout(0xAA, 3);
    EXPECT_EQ(r.bin, "1010 1010 ");
    EXPECT_EQ(r.pos, "     ^    ");
    EXPECT_EQ(r.hex, "   a    a ");
    EXPECT_EQ(r.dec, "      170");
    EXPECT_EQ(r.dec.size(), r.bin.size() - 1);
}

TEST(Bitformat, LayoutEdgeCases)
{
    EXPECT_EQ(layout(0, 0).bin, "0000 ");
    EXPECT_EQ(layout(0, 0).pos, "   ^ ");

    EXPECT_EQ(layout(0x1FF, 8).bin, "0001 1111 1111 ");
    EXPECT_EQ(layout(0x1FF, 8).pos, "   ^           ");

    EXPECT_EQ(layout(1, 40).pos, "     ");
}

TEST(Bitformat, LayoutWidest)
{
    // Widest case: every column stays aligned.
    const Rows wide = layout(UINT64_MAX, 63);
    EXPECT_EQ(wide.bin.size(), 16u * 5);
    EXPECT_EQ(wide.pos.size(), wide.bin.size());
    EXPECT_EQ(wide.hex.size(), wide.bin.size());
    EXPECT_EQ(wide.pos.front(), '^');
    EXPECT_EQ(wide.hex,
              std::string(3, ' ')
                      + "f    f    f    f    f    f    f    f"
                        "    f    f    f    f    f    f    f"
                        "    f ");
    EXPECT_EQ(wide.dec.size(), wide.bin.size() - 1);
    EXPECT_TRUE(wide.dec.ends_with("18446744073709551615"));
}
