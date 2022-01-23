/* hittable.c - Generic hittable object representation

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

#include "hittable.h"

#include <stdlib.h>

void
hittable_release (hittable *obj)
{
  if (obj == NULL)
    return;

  free (obj->data);
  obj->data = NULL;
}

bool
hit (hittable obj, ray r, double t_min, double t_max, hit_record *rec)
{
  if (rec != NULL)
    rec->material_index = obj.material_index;
  
  return obj.hit (obj.data, r, t_min, t_max, rec);
}

inline void
set_record_normal (hit_record *rec, ray r, vec3 out_normal)
{
  if (rec == NULL)
    return;

  rec->front_face = dot (r.dir, out_normal) < 0.0;
  rec->normal = rec->front_face ? out_normal : vec3_negate (out_normal);
}
