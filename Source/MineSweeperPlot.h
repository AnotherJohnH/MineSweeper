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

#ifndef MINE_SWEEPER_PLOT_H
#define MINE_SWEEPER_PLOT_H

#include <cassert>
#include <cstdint>

namespace MineSweeper {

//! Visible state of a square plot of land
enum State : uint8_t
{
   UNDUG,
   FLAG,
   HOLE,
   EXPLOSION
};

//! Manage all state for a square element of land
class Plot
{
public:
   Plot() = default;

   //! Check if plot has not been dug
   bool isUndug() const { return state == UNDUG; }

   //! Check if plot is mined
   bool isMined() const { return mine; }

   //! Get plot state
   State getState(bool& mine_) const
   {
      mine_ = mine;
      return state;
   }

   //! Reset plot of land to undug and empty
   void reset()
   {
      state = UNDUG;
      mine  = false;
   }

   //! Plant a mine
   bool plantMine()
   {
      if(mine) return false;
      mine = true;
      return true;
   }

   //! Toggle flag
   bool toggleFlag(uint16_t& number_of_flags)
   {
      if((state == UNDUG) && (number_of_flags > 0))
      {
         state = FLAG;
         --number_of_flags;
      }
      else if(state == FLAG)
      {
         state = UNDUG;
         ++number_of_flags;
      }
      return state == FLAG;
   }

   //! Start to Dig a hole
   bool startDig()
   {
      assert(isUndug());

      if(!mine) return true;
      state = EXPLOSION;
      return false;
   }

   //! Continue to dig hole
   bool continueDig()
   {
      if((state != UNDUG) || mine) return false;
      state = HOLE;
      return true;
   }

   //!
   void reveal()
   {
      if(mine && (state != EXPLOSION))
      {
         state = HOLE;
      }
   }

private:
   State state{UNDUG};
   bool  mine{false};
};

} // namespace MineSweeper

#endif
