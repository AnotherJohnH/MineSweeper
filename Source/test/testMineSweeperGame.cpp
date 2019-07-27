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

#include "../MineSweeperGame.h"

#include "gtest/gtest.h"

static const size_t WIDTH  = 10;
static const size_t HEIGHT = 10;
static const size_t MINES  = 10;

TEST(MineSweeperGame, initial_state)
{
   MineSweeper::Game<WIDTH,HEIGHT>  game{/* num_of_mines */ MINES};

   EXPECT_EQ(game.getProgress(), MineSweeper::RESET);

   EXPECT_EQ(game.getNumberOfFlags(), MINES);

   EXPECT_EQ(game.getNumberOfTicks(), 0);

   // Prepare a mine map
   bool mines[WIDTH + 2][HEIGHT + 2];

   for(size_t y=0; y<(HEIGHT + 2); ++y)
   {
      for(size_t x=0; x<(WIDTH + 2); ++x)
      {
         mines[x][y] = false;
      }
   }

   // Plot the mine map
   size_t num_of_mines = 0;

   for(size_t y=0; y<HEIGHT; ++y)
   {
      for(size_t x=0; x<WIDTH; ++x)
      {
         bool mine{false};
         EXPECT_EQ(game.getPlotState(x, y, mine), MineSweeper::UNDUG);
         if (mine)
         {
            mines[x + 1][y + 1] = true;
            ++num_of_mines;
         }
      }
   }

   EXPECT_EQ(num_of_mines, MINES);

   // Check adjacent mines count
   for(size_t y=0; y<HEIGHT; ++y)
   {
      for(size_t x=0; x<WIDTH; ++x)
      {
         size_t num_adjacent = 0;

         for(signed ly=-1; ly<=+1; ++ly)
         {
            for(signed lx=-1; lx<=+1; ++lx)
            {
               if (mines[x + 1 + lx][y + 1 + ly])
               {
                  ++num_adjacent;
               }
            }
         }

         EXPECT_EQ(num_adjacent, game.getNumberOfAdjacentMines(x, y));
      }
   }
}

TEST(MineSweeperGame, play)
{
   // TODO
}
