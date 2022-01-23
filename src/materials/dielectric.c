/* dielectric.c - Dielectric material functionalities

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

#include "materials/dielectric.h"

#include "utils.h"

static double
reflectance (double cosine, double ref_idx)
{
  /* Use Schlick's approximation for reflectance */
  double r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow ((1 - cosine), 5);
}

static bool
dielectric_scatter (const void *data, ray r_in, hit_record rec, color *attenuation, ray *scattered)
{
  bool cannot_refract;
  double *ir, refraction_ratio, cos_theta, sin_theta;
  vec3 unit_dir, neg_unit_dir, dir;
  
  if (data == NULL || attenuation == NULL || scattered == NULL)
    return false;

  ir = (double *)data;
  
  *attenuation = vec3_one ();
  refraction_ratio = rec.front_face ? (1.0 / *ir) : *ir;
  
  unit_dir = normalize (r_in.dir);
  neg_unit_dir = vec3_negate (unit_dir);
  cos_theta = fmin (dot (neg_unit_dir, rec.normal), 1.0);
  sin_theta = sqrt (1.0 - cos_theta * cos_theta);

  cannot_refract = refraction_ratio * sin_theta > 1.0;
  if (cannot_refract || reflectance (cos_theta, refraction_ratio) > randd01 ())
    dir = reflect(unit_dir, rec.normal);
  else
    dir = refract (unit_dir, rec.normal, refraction_ratio);

  *scattered = ray_init (rec.p, dir);
  return true;
}

dielectric
dielectric_create (double refraction)
{
  dielectric d = { 0 };
  double *ref;

  ref = (double *)malloc (sizeof (double));
  *ref = refraction;
  d.data = (void *)ref;
  d.scatter = dielectric_scatter;

  return d;
}
