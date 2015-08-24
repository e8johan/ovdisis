/*
** sdl_visual_text.c
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

#include "ovdisis.h"
#include "sdl_visual_text.h"
#include "sdl_visual_various.h"
#include "sdl_visual_internal.h"

extern int  fontheight_system_8x16;
extern int  fontwidth_system_8x16;

extern unsigned char fontdata_system_8x16[];

static char * set_chardata = NULL;
static int    set_charwidth = 0;
static int    set_charheight = 0;

void
sdl_visual_put_char (VWKREF vwk,
		     int    x,
		     int    y,
		     int    fgcol,
		     int    ch)
{
  int ox, oy;
  register unsigned char * chardata = &set_chardata[set_charheight * ch];
  register unsigned char   data;
  register char            light = vwk->text_a.effects & LIGHT;
  Uint32        color;
  SDL_Surface * screen;

  screen = VISUAL_T(vwk->visual->private);

  /* printf("c = %d ", c);*/
  color = PENS(vwk->visual->private)[fgcol];
  /*  printf("color = 0x%x\n", color);*/

  if (sdl_visual_internal_lock(screen))
    return;  

  for (oy = 0; oy < set_charheight; oy++)
  {
    if(light)
    {
      if(oy & 1)
      {
        data = *chardata & 0x55;
      }
      else
      {
        data = *chardata & 0xaa;
      }
    }
    else
    {
      data = *chardata;
    }

    chardata++;

    for (ox = 0; ox < set_charwidth; ox++)
    {
      if (data & 0x80)
      {
        sdl_visual_internal_put_pixel(screen, x + ox, y + oy, color);
      }

      data <<= 1;
    }
  }
  
  sdl_visual_internal_unlock(screen);
  
  SDL_UpdateRect(screen, x, y, set_charwidth, set_charheight);
}


void
sdl_visual_set_font (void * vis,
		     void * data,
		     int    width,
		     int    height)
{
  set_chardata = data;
  set_charwidth = width;
  set_charheight = height;
}
