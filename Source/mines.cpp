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

#include "STB/ConsoleApp.h"

#include "MineSweeperGUI.h"


#define  PROGRAM         "Mine Sweeper"
#define  DESCRIPTION     "An old game"
#define  COPYRIGHT_YEAR  "2016"
#define  AUTHOR          "John D. Haughton"
#define  VERSION         PROJ_VERSION


class MinesApp : public STB::ConsoleApp
{
private:
   STB::Option<unsigned> level{'l', "level", "Level of difficulty [1..3]", 1};

   virtual int startConsoleApp() override
   {
      switch(level)
      {
      case 1: return MineSweeperGUI<9, 9>(10).eventLoop();
      case 2: return MineSweeperGUI<16, 16>(40).eventLoop();
      case 3: return MineSweeperGUI<30, 16>(99).eventLoop();
      }

      return 0;
   }

public:
   MinesApp(int argc, const char* argv[])
      : ConsoleApp(PROGRAM, AUTHOR, DESCRIPTION, VERSION, COPYRIGHT_YEAR)
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[]) { MinesApp(argc, argv); }
