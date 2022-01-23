/* utils.c - Utilities functions

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

#include "utils.h"

#include <stdlib.h>

inline double
randd01 ()
{
  return rand () / (RAND_MAX + 1.0);
}

inline double
randd (double min, double max)
{
  return min + (max - min) * randd01 ();
}

inline double
clamp (double x, double min, double max)
{
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

color
ray_color (ray r, const material_list *list, const world_t *world, int depth)
{
  if (depth <= 0)
    return vec3_zero ();

  hit_record rec = { 0 };
  if (world_hit (world, r, EPSILON, INF, &rec))
    {
      ray scattered = { 0 };
      color attenuation = { 0 };
      material mat = material_list_get (list, rec.material_index);
      
      if (scatter (mat, r, rec, &attenuation, &scattered))
        return vec3_mult (attenuation, ray_color (scattered, list, world, depth - 1));
      
      return vec3_zero ();
    }

  const vec3 udir = normalize (r.dir);
  double t = 0.5 * (udir.y + 1.0);
  return vec3_lerp (vec3_one (), vec3_init (0.5, 0.7, 1.0), t);
}
