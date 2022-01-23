/* camera.c - Simple camera representation

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

#include "camera.h"

camera
camera_init (point lookfrom, point lookat, vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist)
{
  camera cam = { 0 };

  double theta = RAD (vfov);
  double h = tan (theta / 2.0);
  double viewport_height = 2.0 * h;
  double viewport_width = aspect_ratio * viewport_height;

  cam.w = normalize (vec3_sub (lookfrom, lookat));
  cam.u = normalize (cross (vup, cam.w));
  cam.v = normalize (cross (cam.w, cam.u));

  cam.origin = lookfrom;
  cam.horizontal = vec3_multval (cam.u, viewport_width * focus_dist);
  cam.vertical = vec3_multval (cam.v, viewport_height * focus_dist);
  {
    vec3 h2 = vec3_multval (cam.horizontal, 0.5);
    vec3 v2 = vec3_multval (cam.vertical, 0.5);
    vec3 tmp = vec3_sub (cam.origin, h2);
    vec3 w = vec3_multval (cam.w, focus_dist);
    tmp = vec3_sub (tmp, v2);
    cam.lower_left_corner = vec3_sub (tmp, w);
  }

  cam.lens_radius = aperture / 2.0;

  return cam;
}

ray
camera_get_ray (camera cam, double s, double t)
{
  vec3 rd = vec3_multval(vec3_random_unit_disk (), cam.lens_radius);
  vec3 offset = vec3_multval (cam.u, rd.x);
  offset = vec3_add (offset, vec3_multval (cam.v, rd.y));
  
  vec3 sh = vec3_multval (cam.horizontal, s);
  vec3 tv = vec3_multval (cam.vertical, t);
  vec3 tmp = vec3_add (cam.lower_left_corner, sh);
  tmp = vec3_add (tmp, tv);

  vec3 new_orig = vec3_add (cam.origin, offset);
  return ray_init (new_orig,
                   vec3_sub (tmp, new_orig));
}
