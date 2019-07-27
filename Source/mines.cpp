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

#include <cstdio>

#include "STB/ConsoleApp.h"

#include "MineSweeperGUI.h"


#define  PROGRAM         "Mine Sweeper"
#define  DESCRIPTION     "An old game"
#define  LINK            "https://github.com/AnotherJohnH/Platform"
#define  AUTHOR          "John D. Haughton"
#define  COPYRIGHT_YEAR  "2016"


class MineSweeperApp : public STB::ConsoleApp
{
private:
   STB::Option<uint32_t> level{'l', "level", "Level of difficulty 1..3", 1};

   virtual int startConsoleApp() override
   {
      switch(level)
      {
      case 1: return MineSweeperGUI<9, 9>(10).eventLoop();
#if !defined(SMALL_MEMORY)
// Space on the stack will be allocated for the largest game
      case 2: return MineSweeperGUI<16, 16>(40).eventLoop();
      case 3: return MineSweeperGUI<30, 16>(99).eventLoop();
#endif
      }

      return 0;
   }

public:
   MineSweeperApp(int argc, const char* argv[])
      : ConsoleApp(PROGRAM, DESCRIPTION, LINK, AUTHOR, COPYRIGHT_YEAR)
   {
      parseArgsAndStart(argc, argv);
   }
};


int main(int argc, const char* argv[])
{
   MineSweeperApp(argc, argv);
}
