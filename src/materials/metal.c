/* metal.c - Metal material functionalities

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

#include "materials/metal.h"

#include <stdint.h>
#include <stdlib.h>

static bool
metal_scatter (const void *data, ray r_in, hit_record rec, color *attenuation, ray *scattered)
{
  vec3 reflected, tmp;
  double fuzz;
  const uint8_t *d = (uint8_t *)data;

  if (data == NULL || attenuation == NULL || scattered == NULL)
    return false;

  *attenuation = *((const color *)d);
  d += sizeof (color);
  fuzz = *((const double *)d);
  
  reflected = reflect (normalize (r_in.dir), rec.normal);
  tmp = vec3_multval (vec3_random_sphere (), fuzz);
  *scattered = ray_init (rec.p, vec3_add (reflected, tmp));

  return (dot (scattered->dir, rec.normal) > 0.0);
}

metal metal_create (color albedo, double fuzz)
{
  metal m;
  uint8_t *data = NULL;

  m.data = malloc (sizeof (albedo) + sizeof (fuzz));
  m.scatter = metal_scatter;
  
  data = (uint8_t *)m.data;
  *((color *)data) = albedo;
  data += sizeof (color);
  *((double *)data) = fuzz;

  return m;
}
