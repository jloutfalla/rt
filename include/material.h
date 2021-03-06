/* material.h - Generic material representation

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

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "hittable.h"

#include <stdbool.h>

typedef bool (*scatter_func) (const void *data, ray r_in, hit_record rec, color *attenuation, ray *scattered);
typedef struct
{
  void *data;
  scatter_func scatter;
} material;
void material_release (material *mat);

bool scatter (material mat, ray r_in, hit_record rec, color *attenuation, ray *scattered);

#endif /* _MATERIAL_H_ */
