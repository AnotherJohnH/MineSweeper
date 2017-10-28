//------------------------------------------------------------------------------
// Copyright (c) 2017 John D. Haughton
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

#ifndef LED_DISPLAY_H
#define LED_DISPLAY_H

#include "GUI/Font/LED.h"
#include "PLT/Gui.h"

class LEDDisplay : public GUI::Text
{
private:
   virtual void eventSize() override
   {
      GUI::Text::eventSize();

      size.x += 4;
      size.y += 4;
   }

public:
   LEDDisplay(GUI::Widget* parent, unsigned cols)
      : GUI::Text(parent, "")
   {
      setCols(cols);
      setFont(&GUI::font_led22);
      setBackgroundColour(0x400000);
      setForegroundColour(0xFF0000);
   }
};

#endif
