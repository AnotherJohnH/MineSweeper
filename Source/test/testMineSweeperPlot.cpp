//------------------------------------------------------------------------------
// Copyright (c) 2019 John D. Haughton
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//------------------------------------------------------------------------------

#include "../MineSweeperPlot.h"

#include "gtest/gtest.h"

TEST(MineSweeperPlot, constructor)
{
    MineSweeper::Plot plot;

    EXPECT_TRUE(plot.isUndug());

    EXPECT_FALSE(plot.isMined());

    bool mine{true};
    EXPECT_EQ(plot.getState(mine), MineSweeper::UNDUG);
    EXPECT_FALSE(mine);
}

TEST(MineSweeperPlot, state_changes)
{
    MineSweeper::Plot plot;

    // plantMine()
    bool mine;
    EXPECT_TRUE(plot.plantMine());
    EXPECT_TRUE(plot.isMined());
    mine = false;
    EXPECT_EQ(plot.getState(mine), MineSweeper::UNDUG);
    EXPECT_TRUE(mine);

    EXPECT_FALSE(plot.plantMine());
    mine = false;
    EXPECT_EQ(plot.getState(mine), MineSweeper::UNDUG);
    EXPECT_TRUE(mine);

    // toggleFlag()
    uint16_t flags_left{2};
    EXPECT_TRUE(plot.toggleFlag(flags_left));
    EXPECT_EQ(plot.getState(mine), MineSweeper::FLAG);
    EXPECT_EQ(flags_left, 1);

    EXPECT_FALSE(plot.toggleFlag(flags_left));
    EXPECT_EQ(plot.getState(mine), MineSweeper::UNDUG);
    EXPECT_EQ(flags_left, 2);

    // cannot plant a flag if there are no flags available
    flags_left = 0;
    EXPECT_FALSE(plot.toggleFlag(flags_left));
    EXPECT_EQ(plot.getState(mine), MineSweeper::UNDUG);
    EXPECT_EQ(flags_left, 0);
}
