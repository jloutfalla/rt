/* sphere.c - Sphere object functionalities

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

#include "hittables/sphere.h"

#include <math.h>
#include <stdint.h>
#include <stdlib.h>

static bool
sphere_hit (const void *data, ray r, double t_min, double t_max, hit_record *rec)
{
  const uint8_t *d = (const uint8_t *)data;
  vec3 center;
  double radius;

  center = *((vec3 *)d);
  d += sizeof (center);
  radius = *((double *)d);

  vec3 oc = vec3_sub (r.origin, center);
  double a = vec3_length2 (r.dir);
  double half_b = dot (oc, r.dir);
  double c = vec3_length2 (oc) - radius * radius;

  double discriminant = half_b * half_b - a * c;
  if (discriminant < 0)
    return false;

  double sqrtd = sqrt (discriminant);

  /* Find the nearest root that lies in the acceptable range */
  double root = (-half_b - sqrtd) / a;
  if (root < t_min || t_max < root)
    {
      root = (-half_b + sqrtd) / a;
      if (root < t_min || t_max < root)
        return false;
    }

  if (rec != NULL)
    {
      rec->t = root;
      rec->p = ray_at (r, root);

      vec3 outward_normal = vec3_sub (rec->p, center);
      outward_normal = vec3_divval (outward_normal, radius);
      set_record_normal (rec, r, outward_normal);
    }

  return true;
}

sphere
sphere_create (vec3 center, double radius, int material_index)
{
  sphere s;
  uint8_t *data = NULL;
  
  s.data = malloc (sizeof (center) + sizeof (radius));;
  s.material_index = material_index;
  s.hit = sphere_hit;
  
  data = (uint8_t *)s.data;
  *((vec3 *)data) = center;
  data += sizeof (center);
  *((double *)data) = radius;
  
  return s;
}
