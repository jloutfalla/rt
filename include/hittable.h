/* hittable.h - Generic hittable object representation

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

#ifndef _HITTABLE_H_
#define _HITTABLE_H_

#include "ray.h"

#include <stdbool.h>

typedef struct
{
  point p;
  vec3 normal;
  int material_index;
  double t;
  bool front_face;
} hit_record;

typedef bool (*hit_func) (const void *data, ray r, double t_min, double t_max, hit_record *rec);
typedef struct {
  void *data;
  int material_index;
  hit_func hit;
} hittable;
void hittable_release (hittable *obj);

bool hit (hittable obj, ray r, double t_min, double t_max, hit_record *rec);

void set_record_normal (hit_record *rec, ray r, vec3 out_normal);

#endif /* _HITTABLE_H_ */
