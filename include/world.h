/* world.h - List of hittable objects in the scene

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

#ifndef _WORLD_H_
#define _WORLD_H_

#include "hittable.h"

typedef struct _world world_t;

world_t * world_init ();
void      world_release (world_t *world);
size_t    world_size (const world_t *world);
int       world_add (world_t *world, hittable h);
hittable  world_get (const world_t *world, int index);
bool      world_hit (const world_t *world, ray r, double t_min, double t_max, hit_record *rec);

#endif /* _WORLD_H_ */
