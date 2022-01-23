/* vec3.c - Vector utilities functions

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

#include "vec3.h"

#include <math.h>
#include <stdbool.h>

#include "utils.h"

inline vec3
vec3_init (double x, double y, double z)
{
  return (vec3){ .x = x, .y = y, .z = z };
}

inline vec3
vec3_val (double x)
{
  return vec3_init(x, x, x);
}

inline vec3
vec3_zero ()
{
  return vec3_val (0.0);
}

inline vec3
vec3_one ()
{
  return vec3_val (1.0);
}

inline vec3
vec3_negate (vec3 v)
{
  return vec3_init (-v.x, -v.y, -v.z);
}

inline vec3
vec3_add (vec3 v1, vec3 v2)
{
  return vec3_init (v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline vec3
vec3_addval (vec3 v, double d)
{
  return vec3_add (v, vec3_val (d));
}

inline vec3
vec3_sub (vec3 v1, vec3 v2)
{
  return vec3_init (v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline vec3
vec3_subval (vec3 v, double d)
{
  return vec3_sub (v, vec3_val (d));
}

inline vec3
vec3_mult (vec3 v1, vec3 v2)
{
  return vec3_init (v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline vec3
vec3_multval (vec3 v, double n)
{
  return vec3_mult (v, vec3_val (n));
}

inline vec3
vec3_div (vec3 v1, vec3 v2)
{
  return vec3_init (v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

inline vec3
vec3_divval (vec3 v, double n)
{
  return vec3_div (v, vec3_val (n));
}

inline vec3
vec3_sqrt (vec3 v)
{
  return vec3_init (sqrt (v.x), sqrt (v.y), sqrt (v.z));
}

inline double
vec3_length (vec3 v)
{
  return sqrt (vec3_length2 (v));
}

inline double
vec3_length2 (vec3 v)
{
  return v.x * v.x + v.y * v.y + v.z * v.z;
}

inline double
dot (vec3 v1, vec3 v2)
{
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vec3
cross (vec3 v1, vec3 v2)
{
  return vec3_init (v1.y * v2.z - v2.y * v1.z,
                    v1.z * v2.x - v2.z * v1.x,
                    v1.x * v2.y - v2.x * v1.y);
}

inline vec3
normalize (vec3 v)
{
  return vec3_divval (v, vec3_length (v));
}

inline vec3
reflect (vec3 v1, vec3 v2)
{
  return vec3_sub (v1, vec3_multval (v2, 2.0 * dot (v1, v2)));
}

inline vec3
refract (vec3 uv, vec3 n, double etai_over_etat)
{
  double cos_theta, tmpd;
  vec3 neg_uv, tmpv, r_out_perp, r_out_parallel;
  
  neg_uv = vec3_negate (uv);
  cos_theta = fmin (dot (neg_uv, n), 1.0);
  tmpv = vec3_multval (n, cos_theta);
  tmpv = vec3_add (uv, tmpv);
  r_out_perp = vec3_multval (tmpv, etai_over_etat);

  tmpd = fabs (1.0 - vec3_length2 (r_out_perp));
  r_out_parallel = vec3_multval (n, -sqrt (tmpd));

  return vec3_add (r_out_perp, r_out_parallel);
}

inline bool
near_zero (vec3 v)
{
  const double s = 1e-8;
  return (fabs (v.x) < s) && (fabs (v.y) < s) && (fabs(v.z) < s);
}

inline void
print_vec3 (FILE *out, vec3 v)
{
  fprintf (out, "(%f, %f, %f)", v.x, v.y, v.z );
}

inline vec3
vec3_lerp (vec3 vstart, vec3 vend, double t)
{
  const vec3 tmp1 = vec3_multval (vstart, (1.0 - t));
  const vec3 tmp2 = vec3_multval (vend, t);
  return vec3_add (tmp1, tmp2);
}

inline vec3
vec3_random01 ()
{
  return vec3_init (randd01 (), randd01 (), randd01 ());
}

inline vec3
vec3_random (double min, double max)
{
  return vec3_init (randd (min, max), randd (min, max), randd (min, max));
}

inline vec3
vec3_random_sphere ()
{
  while (true)
    {
      vec3 p = vec3_random (-1.0, 1.0);
      if (vec3_length2 (p) >= 1.0) continue;
      return p;
    }
}

inline vec3
vec3_random_unit_sphere ()
{
  return normalize (vec3_random_sphere ());
}

inline vec3
vec3_random_hemisphere (vec3 normal)
{
  vec3 us = vec3_random_unit_sphere ();
  if (dot (us, normal) > 0.0)
    return us;
  else
    return vec3_negate (us);
}

inline vec3
vec3_random_unit_disk ()
{
  vec3 p;
  
  while (true)
    {
      p = vec3_init (randd (-1.0, 1.0), randd (-1.0, 1.0), 0.0);
      if (vec3_length2 (p) >= 1.0)
        continue;
      return p;
    }
}
