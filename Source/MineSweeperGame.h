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

#ifndef MINE_SWEEPER_GAME_H
#define MINE_SWEEPER_GAME_H

#include <cstdlib>

namespace MineSweeper {

enum Progress { READY, UNDERWAY, FAIL, SUCCESS };

enum State    { UNDUG, FLAG, HOLE, EXPLOSION };


//! Mine sweepr game
template <unsigned WIDTH, unsigned HEIGHT>
class Game
{
public:
   Game(unsigned number_of_mines_)
      : number_of_mines(number_of_mines_)
   {
      reset();
   }

   Progress getProgress() const      { return progress; }

   unsigned getNumberOfFlags() const { return number_of_flags; }

   unsigned getNumberOfTicks() const { return number_of_ticks; }

   State getPlotState(unsigned x, unsigned y, bool& mine) const
   {
      const Plot* p = getPlot(x, y);
      mine = p->mine;
      return p->state;
   }

   unsigned getNumberOfAdjacentMines(signed x, signed y) const
   {
      unsigned count = 0;

      for(signed scan_y = y-1; scan_y <= (y + 1); ++scan_y)
      {
         for(signed scan_x = x-1; scan_x <= (x + 1); ++scan_x)
         {
            const Plot* plot = getPlot(scan_x, scan_y);
            if (plot && plot->mine) ++count;
         }
      }

      return count;
   }

   //! Reset ready for new game
   void reset()
   {
      for(unsigned y=0; y<HEIGHT; ++y)
      {
         for(unsigned x=0; x<WIDTH; ++x)
         {
            Plot* plot = getPlot(x, y);

            plot->state = UNDUG;
            plot->mine  = false;
         }
      }

      for(unsigned i=0; i<number_of_mines; ++i)
      {
         unsigned x = rand() % WIDTH;
         unsigned y = rand() % HEIGHT;

         Plot* plot = getPlot(x, y);
         if (plot->mine)
         {
            --i;
         }
         else
         {
            plot->mine = true;
         }
      }

      number_of_flags = number_of_mines;
      number_of_holes = 0;
      number_of_ticks = 0;
      progress        = READY;
   }


   //! Plant or unplant a flag in an undug plot
   void plantUnplantFlag(unsigned x, unsigned y)
   {
      if (progress != UNDERWAY)
      {
         return;
      }

      Plot* plot = getPlot(x, y);
      if ((plot->state == UNDUG) && (number_of_flags > 0))
      {
         plot->state = FLAG;
         --number_of_flags;
      }
      else if (plot->state == FLAG)
      {
         plot->state = UNDUG;
         ++number_of_flags;
      }
   }

   //! Dig a hole in an undug plot
   void digHole(unsigned x, unsigned y)
   {
      if (progress == READY)
      {
         progress = UNDERWAY;
      }
      else if (progress != UNDERWAY)
      {
         return;
      }

      Plot* plot = getPlot(x, y);
      if (plot->state != UNDUG) return;

      if (plot->mine)
      {
         plot->state = EXPLOSION;
         showMines();
         progress = FAIL;
      }
      else
      {
         tryDig(x, y);
         if ((number_of_holes + number_of_mines - number_of_flags) == (WIDTH * HEIGHT))
         {
            progress = SUCCESS;
         }
      }
   }

   //! Increment game timer
   void tick()
   {
      if (progress == UNDERWAY)
      {
         ++number_of_ticks;
      }
   }

private:
   struct Plot
   {
      State  state;
      bool   mine;
   };

   unsigned  number_of_mines;
   unsigned  number_of_flags;
   unsigned  number_of_holes;
   unsigned  number_of_ticks;
   Progress  progress;
   Plot      plot[WIDTH][HEIGHT];

   void tryDig(signed x, signed y)
   {
      Plot* plot = getPlot(x, y);
      if (plot)
      {
         if ((plot->state == UNDUG) && !plot->mine)
         {
            plot->state = HOLE;
            ++number_of_holes;

            if (getNumberOfAdjacentMines(x, y) == 0)
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
   }

   void showMines()
   {
      for(unsigned y=0; y<HEIGHT; ++y)
      {
         for(unsigned x=0; x<WIDTH; ++x)
         {
            Plot* plot = getPlot(x, y);
            if (plot->mine && (plot->state != EXPLOSION))
            {
               plot->state = HOLE;
            }
         }
      }
   }

   Plot* getPlot(signed x, signed y)
   {
      if ((x<0) || (x>=WIDTH) || (y<0) || (y>=WIDTH)) return nullptr;

      return &plot[x][y];
   }

   const Plot* getPlot(signed x, signed y) const
   {
      if ((x<0) || (x>=WIDTH) || (y<0) || (y>=WIDTH)) return nullptr;

      return &plot[x][y];
   }
};

} // MineSweeper

#endif