/* image.c - Image related functions utilities

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

#include "image.h"

#include "utils.h"

#include "stb_image_write.h"

uint8_t *
write_pixel (uint8_t *buffer, color px, int samples)
{
  px = vec3_divval (px, (double)samples);
  px = vec3_sqrt (px);
  
  *buffer = (uint8_t)(256 * clamp (px.x, 0.0, 0.999));
  buffer++;
  *buffer = (uint8_t)(256 * clamp (px.y, 0.0, 0.999));
  buffer++;
  *buffer = (uint8_t)(256 * clamp (px.z, 0.0, 0.999));
  buffer++;
  
  return buffer;
}

void
write_color (FILE *out, color px, int samples)
{
  px = vec3_divval (px, (double)samples);
  px = vec3_sqrt (px);
  fprintf (out, "%d %d %d\n",
           (int)(256 * clamp (px.x, 0.0, 0.999)),
           (int)(256 * clamp (px.y, 0.0, 0.999)),
           (int)(256 * clamp (px.z, 0.0, 0.999)));
}

void
save_png (const char *name, int width, int height, uint8_t *data)
{
  stbi_write_png (name, width, height, 3, data, 0);
}
