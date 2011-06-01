/** Copyright (c) 2011, Johan Kotlinski

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

#include "kbd.h"

#include <cbm.h>

#include "global.h"

U8 KEY_SHIFT;
U8 KEY_SPACE;

void handle_key(U8 key); // defined in kweeca.c

#define ROW(i) CIA1.pra = 0xffu - (1 << i)
#define COL(i) !(CIA1.prb & (1 << i))

void poll_special_keys()
{
    U8 right_shift;
    static U8 s_last_tick_was_tweak;

    ROW(6);
    right_shift = COL(4);

    ROW(1);
    KEY_SHIFT = right_shift || COL(7); // left shift
    
    ROW(7);
    KEY_SPACE = COL(4);

    if (KEY_SPACE)
    {
        U8 tweak_key = 0;
        ROW(5);
        if (COL(5)) tweak_key = CH_TWEAK_LEFT; // :
        else if (COL(6)) tweak_key = CH_TWEAK_UP; // @
        else {
            ROW(6);
            if (COL(2)) tweak_key = CH_TWEAK_DOWN; // ;
            else if (COL(5)) tweak_key = CH_TWEAK_RIGHT; // =
        }
      
        if (tweak_key)
        {
            if (!s_last_tick_was_tweak) // No key repeat for now, change this later?
            {
                handle_key(tweak_key);
                s_last_tick_was_tweak = 1;
            }
        }
        else
            s_last_tick_was_tweak = 0;
    }
    else
        s_last_tick_was_tweak = 0;
    
    CIA1.pra = 0;
}


