/*
** sdl_visual_mouse.c
**
** Copyright 2001 Christer Gustavsson <cg@nocrew.org>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**  
** Read the file COPYING for more information.
**
*/

#define DEBUGLEVEL 0

#include <SDL/SDL.h>
#include <ovdisis.h>

#include "sdl_visual.h"
#include "sdl_visual_mouse.h"

static int x_last = 0;
static int y_last = 0;
static unsigned char mouse_background_buffer[16*16*4];

/*
** Description
** Save background before drawing mouse
*/
void
sdl_visual_save_mouse_bg(void * fb,
                         int    x,
                         int    y)
{
  int iy;
  unsigned char *ptr = mouse_background_buffer;
  unsigned char *pixel;
  int stride;
  SDL_Surface * screen = VISUAL_T(fb);
  
  x_last = x;
  y_last = y;
  
  if (x < screen->w-16)
    stride = 16*screen->format->BytesPerPixel;
  else
    stride = (screen->w-x)*screen->format->BytesPerPixel;
  
  for(iy = 0; iy<16 && iy+y<screen->h; iy++)
  {
    pixel = (unsigned char*)(screen->pixels + (iy+y)*screen->pitch + x*screen->format->BytesPerPixel);
    memcpy(ptr, pixel, stride);
    ptr += stride;
  }
}


/*
** Description
** Restore mouse background
*/
void
sdl_visual_restore_mouse_bg(void * fb)
{
  int iy;
  unsigned char *ptr = mouse_background_buffer;
  unsigned char *pixel;
  int stride;
  SDL_Surface * screen = VISUAL_T(fb);

  if(SDL_MUSTLOCK(screen))
  {
    if(SDL_LockSurface(screen) < 0)
    {
      printf("COULD NOT LOCK\n");
      return;
    }
  }

  if (x_last < screen->w-16)
    stride = 16*screen->format->BytesPerPixel;
  else
    stride = (screen->w-x_last)*screen->format->BytesPerPixel;
  
  for(iy = 0; iy<16 && iy+y_last<screen->h; iy++)
  {
    pixel = (unsigned char*)(screen->pixels + (iy+y_last)*screen->pitch + x_last*screen->format->BytesPerPixel);
    memcpy(pixel, ptr, stride);
    ptr += stride;
  }
  
  if(SDL_MUSTLOCK(screen))
  {
    SDL_UnlockSurface(screen);
  }

  SDL_UpdateRect(screen, x_last, y_last, (x_last+16>screen->w)?(screen->w-x_last):16, (y_last+16>screen->h)?(screen->h-y_last):16);
}
