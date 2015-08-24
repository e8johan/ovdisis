/*
** sdl_visual_internal.h
**
** Copyright 2015 Johan Thelin <e8johan@gmail.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#include "sdl_visual.h"

int sdl_visual_internal_lock(SDL_Surface *screen);
void sdl_visual_internal_unlock(SDL_Surface *screen);
void sdl_visual_internal_put_pixel(SDL_Surface *screen, int x, int y, Uint32 color);
