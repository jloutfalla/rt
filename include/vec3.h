/* vec3.h - Vector utilities functions

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

#ifndef _VEC3_H_
#define _VEC3_H_

#include <stdbool.h>
#include <stdio.h>

typedef struct
{
  double x, y, z;
} vec3;

typedef vec3 point;
typedef vec3 color;

vec3    vec3_init (double x, double y, double z);
vec3    vec3_val (double x);
vec3    vec3_zero ();
vec3    vec3_one ();
vec3    vec3_negate (vec3 v);
vec3    vec3_add (vec3 v1, vec3 v2);
vec3    vec3_addval (vec3 v, double d);
vec3    vec3_sub (vec3 v1, vec3 v2);
vec3    vec3_subval (vec3 v, double d);
vec3    vec3_mult (vec3 v1, vec3 v2);
vec3    vec3_multval (vec3 v, double n);
vec3    vec3_div (vec3 v1, vec3 v2);
vec3    vec3_divval (vec3 v, double n);
vec3    vec3_sqrt (vec3 v);

double  vec3_length (vec3 v);
double  vec3_length2 (vec3 v);

double  dot (vec3 v1, vec3 v2);
vec3    cross (vec3 v1, vec3 v2);
vec3    normalize (vec3 v);
vec3    reflect (vec3 v1, vec3 v2);
vec3    refract (vec3 uv, vec3 n, double etai_over_etat);
bool    near_zero (vec3 v);
void    print_vec3 (FILE *out, const vec3 v);

vec3    vec3_lerp (vec3 vstart, vec3 vend, double t);

vec3    vec3_random01 ();
vec3    vec3_random (double min, double max);
vec3    vec3_random_sphere ();
vec3    vec3_random_unit_sphere ();
vec3    vec3_random_hemisphere (vec3 normal);
vec3    vec3_random_unit_disk ();

#endif /* _VEC3_H_ */
