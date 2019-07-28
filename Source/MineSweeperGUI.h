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
#include "GUI/GUI.h"

#include "LEDDisplay.h"
#include "MineSweeperGame.h"


// 2 char "Mine" and "Flag" font
static const uint8_t font_mines_data[] =
{
   0x08, 0x00, 0x88, 0x80, 0x5D, 0x00, 0x3E, 0x00, 0x67, 0x00, 0xEF, 0xC0, 0x7F, 0x00, 0x3E, 0x00,
   0x5D, 0x00, 0x88, 0x80, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x02, 0x00, 0x0E, 0x00, 0x3E, 0x00, 0x7E, 0x00, 0x3E, 0x00, 0x0E, 0x00, 0x02, 0x00, 0x02, 0x00,
   0x02, 0x00, 0x07, 0x00, 0x3F, 0xC0, 0x3F, 0xC0, 0x00, 0x00, 0x00, 0x00
};
static const GUI::Font font_mines = {{10, 15}, 0x30, 0x31, 1, font_mines_data};


template <unsigned GAME_COLS, unsigned GAME_ROWS>
class MineSweeperGUI : public GUI::App
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
   GUI::Row        gui_menu{this};
   GUI::TextButton gui_help{&gui_menu, EV_HELP, "Help"};
   GUI::Bar        gui_bar{this};
   GUI::Row        gui_top{this, 8};
   LEDDisplay      gui_flags{&gui_top, 3};
   GUI::TextButton gui_reset{&gui_top, EV_RESET, " X "};
   LEDDisplay      gui_time{&gui_top, 3};
   GUI::Col        gui_btm{this, 0};
   GUI::Row        gui_row[GAME_ROWS];
   GUI::TextButton gui_btn[GAME_COLS][GAME_ROWS];

   char text_flags[4];
   char text_time[4];

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
      sprintf(text_flags, "%3d", game.getNumberOfFlags());
      gui_flags.setText(text_flags);

      sprintf(text_time, "%3d", game.getNumberOfTicks());
      gui_time.setText(text_time);

      switch(game.getProgress())
      {
      case MineSweeper::RESET:     gui_reset.text.setText(":-S"); break;
      case MineSweeper::CLEARING:  gui_reset.text.setText(":-|"); break;
      case MineSweeper::DETONATED: gui_reset.text.setText(":-("); break;
      case MineSweeper::CLEARED:   gui_reset.text.setText(":-)"); break;
      }

      for(unsigned y = 0; y < GAME_ROWS; ++y)
      {
         for(unsigned x = 0; x < GAME_COLS; ++x)
         {
            GUI::TextButton* b = &gui_btn[x][y];

            STB::Colour fg = GUI::FOREGROUND;
            STB::Colour bg = GUI::FACE;

            bool mine;

            switch(game.getPlotState(x, y, mine))
            {
            case MineSweeper::UNDUG:
               b->text.setText(" ");
               b->setSelect(false);
               break;

            case MineSweeper::FLAG:
               b->text.setFont(&font_mines);
               b->text.setText("1");
               b->setSelect(false);
               break;

            case MineSweeper::HOLE:
               if(mine)
               {
                  b->text.setFont(&font_mines);
                  b->text.setText("0");
               }
               else
               {
                  b->text.setFont(nullptr);
                  unsigned n = game.getNumberOfAdjacentMines(x, y);

                  switch(n)
                  {
                  case 0: fg = 0x000000; b->text.setText(" "); break;
                  case 1: fg = 0x0000C0; b->text.setText("1"); break;
                  case 2: fg = 0x008000; b->text.setText("2"); break;
                  case 3: fg = 0xC00000; b->text.setText("3"); break;
                  case 4: fg = 0x000040; b->text.setText("4"); break;
                  case 5: fg = 0x400000; b->text.setText("5"); break;
                  case 6: fg = 0x008080; b->text.setText("6"); break;
                  case 7: fg = 0x000000; b->text.setText("7"); break;
                  case 8: fg = 0x808080; b->text.setText("8"); break;
                  default: assert(!"Not possible"); break;
                  }
               }
               b->setSelect(true);
               break;

            case MineSweeper::EXPLOSION:
               b->text.setFont(&font_mines);
               b->text.setText("0");
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
      : GUI::App("Mine Sweeper", &GUI::font_teletext15)
      , game(num_mines)
   {
      gui_top.setAlign(GUI::Align::CENTER, GUI::Align::CENTER);

      gui_help.setFlat();

      // Construct the minefield
      for(unsigned y = 0; y < GAME_ROWS; ++y)
      {
         gui_btm.pushBack(&gui_row[y]);

         for(unsigned x = 0; x < GAME_COLS; ++x)
         {
            unsigned code = (y << EV_Y_LSB) | (x << EV_X_LSB);

            GUI::TextButton* b = &gui_btn[x][y];

            gui_row[y].pushBack(b);

            b->setCode(EV_DIG | code);
            b->setAltCode(false, EV_FLAG | code);
            b->text.setText(" ");
            b->text.setCols(2);
            b->text.setAlign(GUI::Align::CENTER);
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
