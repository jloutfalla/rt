/* lambertian.c - Lambertian material functionalities

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

#include "materials/lambertian.h"

#include <stdint.h>
#include <stdlib.h>

static bool
lambertian_scatter (const void *data, ray r_in, hit_record rec, color *attenuation, ray *scattered)
{
  (void)r_in;
  vec3 scatter_direction;

  if (data == NULL || attenuation == NULL || scattered == NULL)
    return false;
  
  scatter_direction = vec3_add (rec.normal, vec3_random_unit_sphere ());
  if (near_zero (scatter_direction))
    scatter_direction = rec.normal;
  
  *scattered = ray_init (rec.p, scatter_direction);
  *attenuation = *((const color*)data);
  
  return true;
}

lambertian
lambertian_create (color albedo)
{
  lambertian l = { 0 };
  color *c;

  c = (color *)malloc (sizeof (albedo));
  *c = albedo;
  l.data = (void *)c;
  l.scatter = lambertian_scatter;

  return l;
}
