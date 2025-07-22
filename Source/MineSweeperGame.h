//------------------------------------------------------------------------------
// Copyright (c) 2016 John D. Haughton
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

#pragma once

#include <array>
#include <cassert>

#include "MineSweeperPlot.h"

namespace MineSweeper {

//! Overall game state
enum Progress : uint8_t
{
   RESET,
   CLEARING,
   DETONATED,
   CLEARED
};

//! Mine sweeper game
template <unsigned WIDTH, unsigned HEIGHT>
class Game
{
public:
   Game(unsigned number_of_mines_)
      : number_of_mines(number_of_mines_)
   {
      reset();
   }

   //! Return current game state
   Progress getProgress() const { return progress; }

   //! Number of available flags
   unsigned getNumberOfFlags() const { return number_of_flags; }

   //! Number of ticks that the game has been underway
   unsigned getNumberOfTicks() const { return number_of_ticks; }

   //! State of plot at the given location
   State getPlotState(unsigned x, unsigned y, bool& mine) const
   {
      return getPlot(x, y).getState(mine);
   }

   //! Total number of mines adjacent to the given location
   unsigned getNumberOfAdjacentMines(signed x, signed y) const
   {
      unsigned count = 0;

      for(signed scan_y = std::max(y - 1, 0);
          scan_y <= std::min(y + 1, signed(HEIGHT - 1));
          ++scan_y)
      {
         for(signed scan_x = std::max(x - 1, 0);
             scan_x <= std::min(x + 1, signed(WIDTH - 1));
             ++scan_x)
         {
            if(getPlot(scan_x, scan_y).isMined()) ++count;
         }
      }

      return count;
   }

   //! Reset ready for new game
   void reset()
   {
      for(auto& column : field)
      {
         for(auto& plot : column)
         {
            plot.reset();
         }
      }

      for(unsigned planted = 0; planted < number_of_mines;)
      {
         unsigned x = rand() % WIDTH;
         unsigned y = rand() % HEIGHT;

         if(getPlot(x, y).plantMine())
         {
            planted++;
         }
      }

      number_of_flags = number_of_mines;
      number_of_holes = 0;
      number_of_ticks = 0;
      progress        = RESET;
   }

   //! Plant or unplant a flag in an undug plot
   void plantUnplantFlag(unsigned x, unsigned y)
   {
      if(progress != CLEARING)
      {
         return;
      }

      if(getPlot(x, y).toggleFlag(number_of_flags))
      {
         checkIfCleared();
      }
   }

   //! Dig a hole in an undug plot
   void digHole(unsigned x, unsigned y)
   {
      Plot& plot = getPlot(x, y);

      if(progress == RESET)
      {
         while(!plot.startDig())
         {
            // re-plant if the first dig fails
            reset();
         }

         tryDig(x, y);
         progress = CLEARING;
         return;
      }
      else if(progress != CLEARING)
      {
         return;
      }

      if(plot.isUndug())
      {
         if(plot.startDig())
         {
            tryDig(x, y);
            checkIfCleared();
         }
         else
         {
            showMines();
            progress = DETONATED;
         }
      }
   }

   //! Increment game timer
   void tick()
   {
      if(progress == CLEARING)
      {
         ++number_of_ticks;
      }
   }

private:
   static bool isValidPlot(signed x, signed y)
   {
      return (x >= 0) && (x < signed(WIDTH)) &&
             (y >= 0) && (y < signed(HEIGHT));
   }

   void checkIfCleared()
   {
      if((number_of_holes + number_of_mines - number_of_flags) == (WIDTH * HEIGHT))
      {
         progress = CLEARED;
      }
   }

   void tryDig(signed x, signed y)
   {
      if(!isValidPlot(x, y)) return;

      if(getPlot(x, y).continueDig())
      {
         ++number_of_holes;

         if(getNumberOfAdjacentMines(x, y) == 0)
         {
            tryDig(x - 1, y - 1);
            tryDig(x,     y - 1);
            tryDig(x + 1, y - 1);

            tryDig(x - 1, y);
            tryDig(x + 1, y);

            tryDig(x - 1, y + 1);
            tryDig(x,     y + 1);
            tryDig(x + 1, y + 1);
         }
      }
   }

   void showMines()
   {
      for(auto& column : field)
      {
         for(auto& plot : column)
         {
            plot.reveal();
         }
      }
   }

   Plot& getPlot(signed x, signed y)
   {
      assert(isValidPlot(x, y));

      return field[x][y];
   }

   const Plot& getPlot(signed x, signed y) const
   {
      assert(isValidPlot(x, y));

      return field[x][y];
   }

   Progress progress;
   uint16_t number_of_mines;
   uint16_t number_of_flags;
   uint16_t number_of_holes;
   uint32_t number_of_ticks;

   std::array<std::array<Plot, HEIGHT>, WIDTH> field;
};

} // namespace MineSweeper
