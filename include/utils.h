/* utils.h - Utilities functions

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

#ifndef _UTILS_
#define _UTILS_

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif /* M_PI */

#define RAD(X) (((X) * M_PI) / 180.0)

#define EPSILON 0.001
#define INF DBL_MAX

#include "vec3.h"
#include "ray.h"
#include "world.h"
#include "material_list.h"

double randd01 ();
double randd (double min, double max);
double clamp (double x, double min, double max);

color ray_color (ray r, const material_list *list, const world_t *world, int depth);

#endif /* _UTILS_ */
