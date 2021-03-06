/*
 * vdi_marker.c
 *
 * Copyright 1998 Tomas Berndtsson <tomas@nocrew.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *  
 * Read the file COPYING for more information.
 *
 */

#include <stdio.h>

#include "ovdisis.h"
#include "various.h"
#include "vdi_marker.h"


#define LOWERODD(a) ((a)&0x1?(a):(a)-1)

void vdi_v_pmarker(VDI_Workstation *vwk)
{
  int n,ni;
  unsigned long col;
  RECT cor;

  /* Lock visual before operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_LOCK);

  /* Setup write mode */
  VISUAL_SET_WRITE_MODE(vwk, vwk->write_mode);

  ni = gem2tos_color(vwk->inq.attr.planes, vwk->marker_a.color);
  col = get_color(vwk, ni);
  
  switch(vwk->marker_a.type)
  {
  case M_DOT:
    for(n=0 ; n < (int)vdipb->contrl[N_PTSIN] ; n++)
    {
      int x,y;
      x = (int)vdipb->ptsin[0+n*2];
      y = (int)vdipb->ptsin[1+n*2];
      if(do_pointclip(x, y, &vwk->clip))
        VISUAL_PUT_PIXEL(vwk, x, y, col);
    }
    break;
  case M_PLUS:
    for(n=0 ; n < (int)vdipb->contrl[N_PTSIN] ; n++)
    {
      cor.x1 = (int)vdipb->ptsin[0+n*2];
      cor.y1 = (int)vdipb->ptsin[1+n*2]-vwk->marker_a.height/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2];
      cor.y2 = (int)vdipb->ptsin[1+n*2]+vwk->marker_a.height/2;
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y1 = cor.y2 = (int)vdipb->ptsin[1+n*2];
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_HLINE(vwk, cor.x1, cor.x2, cor.y1, col);
    }
    break;
  case M_ASTERISK:
    for(n=0 ; n < (int)vdipb->contrl[N_PTSIN] ; n++)
    {
      cor.x1 = (int)vdipb->ptsin[0+n*2];
      cor.y1 = (int)vdipb->ptsin[1+n*2]-vwk->marker_a.height/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2];
      cor.y2 = (int)vdipb->ptsin[1+n*2]+vwk->marker_a.height/2;
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y1 = cor.y2 = (int)vdipb->ptsin[1+n*2];
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_HLINE(vwk, cor.x1, cor.x2, cor.y1, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]-(int)(((float)vwk->marker_a.width*0.8)/2);
      cor.y1 = (int)vdipb->ptsin[1+n*2]-(int)(((float)vwk->marker_a.height*0.8)/2);
      cor.x2 = (int)vdipb->ptsin[0+n*2]+(int)(((float)vwk->marker_a.width*0.8)/2);
      cor.y2 = (int)vdipb->ptsin[1+n*2]+(int)(((float)vwk->marker_a.height*0.8)/2);
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]+(int)(((float)vwk->marker_a.width*0.8)/2);
      cor.y1 = (int)vdipb->ptsin[1+n*2]-(int)(((float)vwk->marker_a.height*0.8)/2);
      cor.x2 = (int)vdipb->ptsin[0+n*2]-(int)(((float)vwk->marker_a.width*0.8)/2);
      cor.y2 = (int)vdipb->ptsin[1+n*2]+(int)(((float)vwk->marker_a.height*0.8)/2);
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
    }
    break;
  case M_SQUARE:
    for(n=0 ; n < (int)vdipb->contrl[N_PTSIN] ; n++)
    {
      cor.x1 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.y1 = (int)vdipb->ptsin[1+n*2]-vwk->marker_a.height/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.y2 = (int)vdipb->ptsin[1+n*2]+vwk->marker_a.height/2; 
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y1 = (int)vdipb->ptsin[1+n*2]-vwk->marker_a.height/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y2 = (int)vdipb->ptsin[1+n*2]+vwk->marker_a.height/2; 
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y1 = cor.y2 = (int)vdipb->ptsin[1+n*2]-vwk->marker_a.height/2; 
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_HLINE(vwk, cor.x1, cor.x2, cor.y1, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y1 = cor.y2 = (int)vdipb->ptsin[1+n*2]+vwk->marker_a.height/2;
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_HLINE(vwk, cor.x1, cor.x2, cor.y1, col);
    }
    break;
  case M_CROSS:
    for(n=0 ; n < (int)vdipb->contrl[N_PTSIN] ; n++)
    {
      cor.x1 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.y1 = (int)vdipb->ptsin[1+n*2]-vwk->marker_a.height/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y2 = (int)vdipb->ptsin[1+n*2]+vwk->marker_a.height/2;
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y1 = (int)vdipb->ptsin[1+n*2]-vwk->marker_a.height/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.y2 = (int)vdipb->ptsin[1+n*2]+vwk->marker_a.height/2;
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
    }
    break;
  case M_DIAMOND:
    for(n=0 ; n < (int)vdipb->contrl[N_PTSIN] ; n++)
    {
      cor.x1 = (int)vdipb->ptsin[0+n*2];
      cor.y1 = (int)vdipb->ptsin[1+n*2]-vwk->marker_a.height/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.y2 = (int)vdipb->ptsin[1+n*2];
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]-vwk->marker_a.width/2;
      cor.y1 = (int)vdipb->ptsin[1+n*2];
      cor.x2 = (int)vdipb->ptsin[0+n*2];
      cor.y2 = (int)vdipb->ptsin[1+n*2]+vwk->marker_a.height/2;
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2];
      cor.y1 = (int)vdipb->ptsin[1+n*2]+vwk->marker_a.height/2;
      cor.x2 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y2 = (int)vdipb->ptsin[1+n*2];
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
      
      cor.x1 = (int)vdipb->ptsin[0+n*2]+vwk->marker_a.width/2;
      cor.y1 = (int)vdipb->ptsin[1+n*2];
      cor.x2 = (int)vdipb->ptsin[0+n*2];
      cor.y2 = (int)vdipb->ptsin[1+n*2]-vwk->marker_a.height/2;
      if(do_lineclip(&cor, &vwk->clip))
        VISUAL_LINE(vwk, cor.x1, cor.y1, cor.x2, cor.y2, col);
    }
    break;
  default:
    EDEBUG("v_pmarker: Unknown marker type %d found!",vwk->marker_a.type);
    break;
  }

  /* Unlock visual after operation */
  VISUAL_MUTEX(vwk, VISUAL_MUTEX_UNLOCK);

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_vsm_color(VDI_Workstation *vwk)
{
  vwk->marker_a.color=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->marker_a.color;

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 1;
}

void vdi_vsm_height(VDI_Workstation *vwk)
{
  vwk->marker_a.width=LOWERODD((int)vdipb->ptsin[1]);  /* marker_width==marker_height? */
  vwk->marker_a.height=LOWERODD((int)vdipb->ptsin[1]); /* Not sure about that */
  vdipb->ptsout[0]=(short)vwk->marker_a.width;
  vdipb->ptsout[1]=(short)vwk->marker_a.height;

  vdipb->contrl[N_PTSOUT] = 1;
  vdipb->contrl[N_INTOUT] = 0;
}

void vdi_vsm_type(VDI_Workstation *vwk)
{
  vwk->marker_a.type=(int)vdipb->intin[0];
  vdipb->intout[0]=(short)vwk->marker_a.type;

  vdipb->contrl[N_PTSOUT] = 0;
  vdipb->contrl[N_INTOUT] = 1;
}

void vdi_vqm_attributes(VDI_Workstation *vwk)
{
  vdipb->ptsout[0]=(short)vwk->marker_a.width;
  vdipb->ptsout[1]=(short)vwk->marker_a.height;

  vdipb->intout[0]=(short)vwk->marker_a.type;
  vdipb->intout[1]=(short)vwk->marker_a.color;
  vdipb->intout[2]=(short)vwk->write_mode;

  vdipb->contrl[N_PTSOUT]=1;
  vdipb->contrl[N_INTOUT]=3;
}



