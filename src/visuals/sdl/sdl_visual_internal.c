#include "sdl_visual.h"

int
sdl_visual_internal_lock(SDL_Surface *screen)
{
  if(SDL_MUSTLOCK(screen))
  {
    if(SDL_LockSurface(screen) < 0)
    {
      return -1;
    }
  }
  
  return 0;
}

void
sdl_visual_internal_unlock(SDL_Surface *screen)
{
  if(SDL_MUSTLOCK(screen))
  {
    SDL_UnlockSurface(screen);
  }
}

void
sdl_visual_internal_put_pixel(SDL_Surface *screen, int x, int y, Uint32 color)
{
  switch(screen->format->BytesPerPixel)
  {
  case 1: /* Assuming 8-bpp */
  {
    Uint8 *bufp;
    
    bufp = (Uint8 *)screen->pixels + y*screen->pitch + x;
    *bufp = color;
  }
  break;
  
  case 2: /* Probably 15-bpp or 16-bpp */
  {
    Uint16 * bufp;
    
    bufp = (Uint16 *)screen->pixels + y*screen->pitch/2 + x;
    *bufp = color;
  }
  break;
  
  case 3: /* Slow 24-bpp mode, usually not used */
  {
    Uint8 * bufp;
    
    bufp = (Uint8 *)screen->pixels + y*screen->pitch + x * 3;
    if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
    {
      bufp[0] = color;
      bufp[1] = color >> 8;
      bufp[2] = color >> 16;
    }
    else
    {
      bufp[2] = color;
      bufp[1] = color >> 8;
      bufp[0] = color >> 16;
    }
  }
  break;
  
  case 4: /* Probably 32-bpp */
  {
    Uint32 *bufp;
    
    bufp = (Uint32 *)screen->pixels + y*screen->pitch/4 + x;
    *bufp = color;
  }
  break;
  }
}
