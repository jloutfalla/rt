/* image.h - Image related functions utilities

   Copyright (C) 2022 Jean-Baptiste Loutfalla <jb.loutfalla@orange.fr>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <stdint.h>

#include "vec3.h"

uint8_t *write_pixel (uint8_t *buffer, color px, int samples);
void     write_color (FILE *out, color px, int samples);
void	 save_png (const char *name, int width, int height, uint8_t *data);

#endif /* _IMAGE_H_ */
