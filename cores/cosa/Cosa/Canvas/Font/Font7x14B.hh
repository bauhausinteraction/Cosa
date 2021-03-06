/**
 * @file Font7x14B.hh
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2014, jediunix
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 */

#ifndef COSA_FONT_FONT7X14B_HH
#define COSA_FONT_FONT7X14B_HH

#include "Cosa/Canvas/Font.hh"

/**
 * Font size 7x14B.
 */
class Font7x14B : public Font {
public:
  /**
   * Construct font (7x14B) singleton.
   */
  Font7x14B() :
    Font(width, height, first, last, bitmap)
  {}

private:
  static const uint8_t bitmap[] PROGMEM;
  static const uint8_t width;
  static const uint8_t height;
  static const uint8_t first;
  static const uint8_t last;
};

extern Font7x14B font7x14B;

#endif
