/* ray.c - Ray representation

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

#include "ray.h"

inline ray
ray_init (vec3 origin, vec3 dir)
{
  return (ray){
    .origin = origin,
    .dir = dir
  };
}

inline point
ray_at (ray r, double t)
{
  return vec3_add (r.origin, vec3_multval (r.dir, t));
}
