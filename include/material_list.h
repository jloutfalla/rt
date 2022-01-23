/* material_list.h - List of every material in the scene

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

#ifndef _MATERIAL_LIST_
#define _MATERIAL_LIST_

typedef struct _material_list material_list;

#include "material.h"

material_list *	material_list_init ();
void		material_list_release (material_list *list);
size_t		material_list_size (const material_list *list);
int		material_list_add (material_list *list, material mat);
material	material_list_get (const material_list *list, int index);
bool		material_list_scatter (const material_list *list, ray r_in, hit_record rec, color *attenuation, ray *scattered);

#endif /* _MATERIAL_LIST_ */
