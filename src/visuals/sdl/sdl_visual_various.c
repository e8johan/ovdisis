/*
** ggi_visual_various.c
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
#include "sdl_visual.h"
#include "sdl_visual_various.h"
#include "sdl_visual_internal.h"

void
sdl_visual_inquire(void *        vis,
                   Visual_Attr * attr)
{
  DEBUG3("sdl_visual_inquire entered\n");
  attr->x_res = VISUAL_T(vis)->w;
  attr->y_res = VISUAL_T(vis)->h;
  attr->palette_size = 256 /* FIXME */;
  attr->number_of_colours = 2 << 16 /* FIXME */;
  attr->bits_per_pixel = 16 /* FIXME */;
  DEBUG3("sdl_visual_inquire exited\n");
}

int
sdl_visual_get_pixel (VWKREF vwk,
		      int    x,
		      int    y)
{
  DEBUG0("Implement sdl_visual_get_pixel\n");
  return 0;
}


void
sdl_visual_put_pixel(VWKREF vwk,
                     int    x,
                     int    y,
                     int    c)
{
  Uint32        color;
  SDL_Surface * screen;

  DEBUG3("sdl_visual_put_pixel entered\n");
  
  screen = VISUAL_T(vwk->visual->private);

  /* printf("c = %d ", c);*/
  color = PENS(vwk->visual->private)[c];
  /*  printf("color = 0x%x\n", color);*/

  if (sdl_visual_internal_lock(screen))
    return;
  
  if( WRITE_MODE(vwk->visual->private) == MD_XOR ) {
    printf("bpp = %d\n", screen->format->BytesPerPixel);
  }
  
  sdl_visual_internal_put_pixel(screen, x, y, color);

  sdl_visual_internal_unlock(screen);

  SDL_UpdateRect(screen, x, y, 1, 1);
  DEBUG3("sdl_visual_put_pixel exited\n");
}


void sdl_visual_put_pixels( VWKREF vwk, int n,  /* The number of pixels to draw */
			    Pixel *pixel ) {
  int minx = 100000, miny = 1000000, maxx = 0, maxy = 0;
  int i;
  
  Uint32        color;
  SDL_Surface * screen;
  
  screen = VISUAL_T(vwk->visual->private);
  
  sdl_visual_internal_lock(screen);
  
  if( WRITE_MODE(vwk->visual->private) == MD_XOR ) {
    printf("bpp = %d\n", screen->format->BytesPerPixel);
  }
  
  for( i = 0 ; i < n ; i++ ) {
    color = PENS(vwk->visual->private)[pixel[i].color];
    sdl_visual_internal_put_pixel(screen, pixel[i].x, pixel[i].y, color);
    
    if (minx > pixel[i].x)
      minx = pixel[i].x;
    if (miny > pixel[i].y)
      miny = pixel[i].y;
    if (maxx < pixel[i].x)
      maxx = pixel[i].x;
    if (maxy < pixel[i].y)
      maxy = pixel[i].y;
  }
  
  sdl_visual_internal_unlock(screen);
  
  if (minx < maxx && miny < maxy)
    SDL_UpdateRect(screen, minx, miny, maxx-minx+1, maxy-miny+1);
}


void
sdl_visual_hline(VWKREF vwk,
                 int    x1,
                 int    x2,
                 int    y,
                 int    c)
{
  int           xmin;
  int           xmax;
  Uint32        color;
  SDL_Surface * screen;
  SDL_Rect      area;

  DEBUG3("sdl_visual_hline entered\n");

  screen = VISUAL_T(vwk->visual->private);

  /* printf("c = %d ", c);*/
  color = PENS(vwk->visual->private)[c];
  /*  printf("color = 0x%x\n", color);*/

  if (sdl_visual_internal_lock(screen))
    return;

  if(x1 < x2)
  {
    xmin = x1;
    xmax = x2;
  }
  else
  {
    xmin = x2;
    xmax = x1;
  }

  area.x = xmin;
  area.y = y;
  area.w = xmax - xmin + 1;
  area.h = 1;

  SDL_FillRect(screen, &area, color);

  sdl_visual_internal_unlock(screen);
  
  SDL_UpdateRect(screen, xmin, y, xmax-xmin+1, 1);

  DEBUG3("sdl_visual_hline exited\n");
}


#define MIN(a,b) ((a < b) ? a : b)
#define MAX(a,b) ((a > b) ? a : b)

void
sdl_visual_line(VWKREF vwk,
                int    a1,
                int    b1,
                int    a2,
                int    b2,
                int    col)
{
  int16_t x, y, xend, yend, dx, dy, c1, c2, p;
  int16_t step;
  Uint32        color;
  SDL_Surface * screen;
  
  DEBUG3("sdl_visual_line entered\n");

  screen = VISUAL_T(vwk->visual->private);
  
  sdl_visual_internal_lock(screen);

  color = PENS(vwk->visual->private)[col];

  dx = abs(a2-a1);
  dy = abs(b2-b1);
  
  if (dx > dy) { /* slope < 1 => step in x direction */
    x = MIN(a1,a2);
    xend = MAX(a1,a2);
    if (x == a1) {
      y = b1;
      step = ((b2-y)<0 ? -1 : 1);
    } else {
      y = b2;
      step = ((b1-y)<0 ? -1 : 1);
    }

    p = 2*dy - dx;
    c1 = 2*dy;
    c2 = 2*(dy-dx);
    
    sdl_visual_internal_put_pixel(screen, x, y, color);
    while (x < xend) {
      x++;
      if (p < 0) {
        p += c1;
      } else {
        y += step;
        p += c2;
      }
      sdl_visual_internal_put_pixel(screen, x, y, color);
    }
  } else { /* slope > 1 => step in y direction */
    y = MIN(b1,b2);
    yend = MAX(b1,b2);
    if (y == b1) {
      x = a1;
      step = ((a2-x)<0 ? -1 : 1);
    } else {
      x = a2;
      step = ((a1-x)<0 ? -1 : 1);
    }
    
    p = 2*dx - dy;
    c1 = 2*dx;
    c2 = 2*(dx-dy);
    
    sdl_visual_internal_put_pixel(screen, x, y, color);
    while (y < yend) {
      y++;
      if (p < 0) {
        p += c1;
      } else {
        x += step;
        p += c2;
      }
      sdl_visual_internal_put_pixel(screen, x, y, color);
    }
  }

  x = MIN(a1,a2);
  xend = MAX(a1,a2);
  y = MIN(b1,b2);
  yend = MAX(b1,b2);

  sdl_visual_internal_unlock(screen);
  
  SDL_UpdateRect(screen, x, y, xend-x+1, yend-y+1);

  DEBUG3("sdl_visual_line exited\n");
}
