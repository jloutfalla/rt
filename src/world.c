/* world.c - List of hittable objects in the scene

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

#include "world.h"

#include "hittable.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _world
{
  hittable *data;
  size_t size;
  size_t capacity;
};

world_t *
world_init ()
{
  world_t *world = (world_t *)malloc (sizeof (world_t));
  world->data = NULL;
  world->size = 0;
  world->capacity = 0;
  return world;
}

void
world_release (world_t *world)
{
  if (world == NULL)
    return;

  for (size_t i = 0; i < world->size; ++i)
    hittable_release (&world->data[i]);

  if (world->data != NULL)
    free (world->data);
  
  free (world);
}

size_t
world_size (const world_t *world)
{
  if (world == NULL)
    return 0;

  return world->size;
}

int
world_add (world_t *world, hittable h)
{
  int ret;
  
  if (world == NULL)
    return -1;

  if (world->capacity <= world->size)
    {
      if (world->capacity == 0)
        world->capacity = 1;
      else
        world->capacity = world->capacity * 2;

      world->data = realloc (world->data, world->capacity * sizeof (world->data[0]));
      if (world->data == NULL)
        {
          fprintf(stderr,
                  "An error occured while adding an object to a `world_t`: %s\n",
                  strerror (errno));
          errno = 0;
          exit (EXIT_FAILURE);
        }
    }

  ret = world->size;
  world->data[world->size] = h;
  world->size++;

  return ret;
}

hittable
world_get (const world_t *world, int index)
{
  if (world == NULL || index < 0 || world->size <= (size_t)index)
    return (hittable){ 0 };

  return world->data[index];
}

bool
world_hit (const world_t *world, ray r, double t_min, double t_max, hit_record *rec)
{
  if (world == NULL)
    return false;
  
  hit_record tmp_rec = { 0 };
  bool hit_anything = false;
  double closet_so_far = t_max;

  for (size_t i = 0; i < world->size; ++i)
    {
      hittable obj = world_get (world, i);
      if (hit (obj, r, t_min, closet_so_far, &tmp_rec))
        {
          hit_anything = true;
          closet_so_far = tmp_rec.t;
          *rec = tmp_rec;
        }
    }

  return hit_anything;
}
