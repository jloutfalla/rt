/* camera.h - Simple camera representation

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

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "utils.h"

typedef struct
{
  point origin;
  point lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u;
  vec3 v;
  vec3 w;
  double lens_radius;
} camera;

camera camera_init (point lookfrom, point lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist);
ray camera_get_ray (camera cam, double s, double t);

#endif /* _CAMERA_H_ */
