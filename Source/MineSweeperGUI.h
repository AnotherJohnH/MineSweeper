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

#ifndef MINE_SWEEPER_GUI_H
#define MINE_SWEEPER_GUI_H

#include <cassert>

#include "GUI/Font/Teletext.h"
#include "PLT/Gui.h"

#include "MineSweeperGame.h"


template <unsigned GAME_COLS, unsigned GAME_ROWS>
class MineSweeperGUI : public PLT::Gui
{
private:
   // Event code fields
   static const unsigned EV_DIG   = 1 << 16;
   static const unsigned EV_FLAG  = 1 << 17;
   static const unsigned EV_X_LSB = 0;
   static const unsigned EV_Y_LSB = 8;
   static const unsigned EV_HELP  = 1;
   static const unsigned EV_RESET = 2;
   static const unsigned EV_TICK  = 3;

   // GUI components
   GUI::Row         gui_menu;
   GUI::TextButton  gui_help;
   GUI::Bar         gui_bar;
   GUI::Row         gui_top;
   GUI::Col         gui_btm;
   GUI::Field       gui_flags;
   GUI::TextButton  gui_reset;
   GUI::Field       gui_time;
   GUI::TextButton* gui_btn[GAME_COLS][GAME_ROWS];

   // Game state
   MineSweeper::Game<GAME_COLS, GAME_ROWS> game;

   //! Handle events from the GUI
   virtual void appEvent(Widget*, unsigned code) override
   {
      if(code == EV_RESET)
      {
         game.reset();
      }
      else if(code == EV_TICK)
      {
         game.tick();
      }
      else
      {
         unsigned x = (code >> EV_X_LSB) & 0xFF;
         unsigned y = (code >> EV_Y_LSB) & 0xFF;

         if(code & EV_FLAG)
         {
            game.plantUnplantFlag(x, y);
         }
         else if(code & EV_DIG)
         {
            game.digHole(x, y);
         }
      }

      refresh();
   }


   //! Refresh the GUI state
   void refresh()
   {
      char text[4];

      sprintf(text, "%3d", game.getNumberOfFlags());
      gui_flags.setValue(text);

      sprintf(text, "%3d", game.getNumberOfTicks());
      gui_time.setValue(text);

      switch(game.getProgress())
      {
      case MineSweeper::READY:    gui_reset.text.setText(":-S"); break;
      case MineSweeper::UNDERWAY: gui_reset.text.setText(":-|"); break;
      case MineSweeper::FAIL:     gui_reset.text.setText(":-("); break;
      case MineSweeper::SUCCESS:  gui_reset.text.setText(":-)"); break;
      }

      for(unsigned y = 0; y < GAME_ROWS; ++y)
      {
         for(unsigned x = 0; x < GAME_COLS; ++x)
         {
            GUI::TextButton* b = gui_btn[x][y];

            GUI::Colour fg = GUI::FOREGROUND;
            GUI::Colour bg = GUI::FACE;

            bool mine;

            switch(game.getPlotState(x, y, mine))
            {
            case MineSweeper::UNDUG:
               b->text.setText(' ');
               b->setSelect(false);
               break;

            case MineSweeper::FLAG:
               b->text.setText('F');
               b->setSelect(false);
               break;

            case MineSweeper::HOLE:
               if(mine)
               {
                  b->text.setText('*');
               }
               else
               {
                  unsigned n = game.getNumberOfAdjacentMines(x, y);

                  b->text.setText(n == 0 ? ' ' : '0' + n);

                  switch(n)
                  {
                  case 0: fg = 0x000000; break;
                  case 1: fg = 0x0000C0; break;
                  case 2: fg = 0x008000; break;
                  case 3: fg = 0xC00000; break;
                  case 4: fg = 0x000040; break;
                  case 5: fg = 0x400000; break;
                  case 6: fg = 0x008080; break;
                  case 7: fg = 0x000000; break;
                  case 8: fg = 0x808080; break;
                  default: assert(!"Not possible"); break;
                  }
               }
               b->setSelect(true);
               break;

            case MineSweeper::EXPLOSION:
               b->text.setText('*');
               bg = 0xE00000;
               break;
            }

            b->text.setForegroundColour(fg);
            b->text.setBackgroundColour(bg);
            b->setBackgroundColour(bg);
         }
      }
   }

public:
   MineSweeperGUI(unsigned num_mines)
      : PLT::Gui("Mine Sweeper", &GUI::font_teletext15)
      , gui_menu(this)
      , gui_help(&gui_menu, EV_HELP, "Help")
      , gui_bar(this)
      , gui_top(this, 8)
      , gui_btm(this, 0)
      , gui_flags(&gui_top, 1, 3, "")
      , gui_reset(&gui_top, EV_RESET, " X ")
      , gui_time(&gui_top, 1, 3, "")
      , game(num_mines)
   {
      gui_top.setAlign(GUI::CENTER, GUI::CENTER);

      gui_help.setFlat();

      // Construct the minefield
      for(unsigned y = 0; y < GAME_ROWS; ++y)
      {
         GUI::Row* row = new GUI::Row(&gui_btm);
         row->setAutoDelete();

         for(unsigned x = 0; x < GAME_COLS; ++x)
         {
            unsigned code = (y << EV_Y_LSB) | (x << EV_X_LSB);

            GUI::TextButton* b = new GUI::TextButton(row, EV_DIG | code, " ");
            b->setAutoDelete();

            b->setAltCode(false, EV_FLAG | code);
            b->text.setWidth(2);

            gui_btn[x][y] = b;
         }
      }

      // Sync GUI state with initial game state
      refresh();

      // Layout and draw the gui
      show();

      // one second tick timer
      setTimer(EV_TICK, 1000);
   }
};

#endif
