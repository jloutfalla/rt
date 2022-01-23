/* material_list.c - List of every material in the scene

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

#include "material_list.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

struct _material_list
{
  material *data;
  size_t size;
  size_t capacity;
};

material_list *
material_list_init ()
{
  material_list *list = (material_list *)malloc (sizeof (material_list));
  list->data = NULL;
  list->size = 0;
  list->capacity = 0;
  return list;
}

void
material_list_release (material_list *list)
{
  size_t i;
  
  if (list == NULL)
    return;

  for (i = 0; i < list->size; ++i)
    material_release (&list->data[i]);

  if (list->data != NULL)
    free (list->data);
  
  free (list);
}

size_t
material_list_size (const material_list *list)
{
  if (list == NULL)
    return 0;

  return list->size;
}

int
material_list_add (material_list *list, material mat)
{
  int ret;
  
  if (list == NULL)
    return -1;

  if (list->capacity <= list->size)
    {
      if (list->capacity == 0)
        list->capacity = 1;
      else
        list->capacity = list->capacity * 2;

      list->data = realloc (list->data, list->capacity * sizeof (list->data[0]));
      if (list->data == NULL)
        {
          fprintf(stderr,
                  "An error occured while adding an object to a `material_list`: %s\n",
                  strerror (errno));
          errno = 0;
          exit (EXIT_FAILURE);
        }
    }

  ret = list->size;
  list->data[list->size] = mat;
  list->size++;

  return ret;
}

material
material_list_get (const material_list *list, int index)
{
  if (list == NULL || index < 0 || list->size <= (size_t)index)
    return (material){ 0 };

  return list->data[index];
}
