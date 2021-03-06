/*
 * vdi_marker.h
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

#ifndef _VDI_MARKER_H_
#define _VDI_MARKER_H_

extern void vdi_v_pmarker(VDI_Workstation *);

extern void vdi_vsm_color(VDI_Workstation *);
extern void vdi_vsm_height(VDI_Workstation *);
extern void vdi_vsm_type(VDI_Workstation *);

extern void vdi_vqm_attributes(VDI_Workstation *);

#endif /* _VDI_MARKER_H_ */
